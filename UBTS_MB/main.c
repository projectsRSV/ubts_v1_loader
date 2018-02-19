#include <avr/io.h>
#include "initDevice.h"
#include "sp_driver.h"
#include "eeprom_driver.h"
#include "PROGM_.h"
#include "CRC.h"
#include "Decoder.h"



//void ledBlink();
uint8_t commandDecoder(uint8_t length, uint8_t* buff);

void flushBuff(uint8_t* buff);

void writeSn(void);
void readSn(void);
void writePr(void);

void requestToPC(uint8_t loaderStatus, uint16_t pageCounter,uint16_t pageSize);
uint8_t responseFromPC(uint16_t* pageCounter,uint8_t* buff,uint16_t pageSize,uint8_t disableDecoder);
void writeFlash(uint16_t progAddress,uint8_t* buff);
void readFlash(uint16_t readAddress,uint8_t* buff);
void writeEEPROM(uint8_t* buff);
void readEEPROM(uint8_t* buff);

void (*funcPtr)(void);

uint8_t buffer[BUFFER_SIZE];
uint8_t tempBuffFlashPage[Flash_Page_Size];
uint8_t loaderAction=0;

uint8_t POLYNOM[] = {0x93,0xda,0x16,0xea};
//uint8_t buff[BUFFER_SIZE];
uint8_t ledFanState;


typedef void (*fpFanLed)(void);
fpFanLed fanLedFuncPtr;
void greenBLink();
fpFanLed lightPtrTable[] = {greenBLink};

int main(void){
	uint32_t count=0;
	uint8_t endVar=0;
	uint8_t wasWrite=0;
	
	init_all();
	EEPROM_FlushBuffer();
	EEPROM_DisableMapping();
	
	blinkFuncPtr = (fpStatusLed)(blinkPtrTable[2]);				//speed of led blinking
	fanLedFuncPtr = (fpFanLed)(lightPtrTable[0]);				//green led blink
	
	while (1){
		blinkFuncPtr();
		fanLedFuncPtr();
		
		switch (loaderAction){
			case WRITE_Pr:{
				loaderAction=0x00;
				writePr();
				endVar=0x00;
				wasWrite=1;
				break;
			}
			case WRITE_Sn:{
				loaderAction=0x00;
				writeSn();
				break;
			}
			case READ_Sn:{
				loaderAction=0x00;
				readSn();
				_delay_ms(1);
				if (!wasWrite) endVar=0x01;
				else count = 2000000;
				break;
			}
			default:{
				if (count++ == 2200000){
					if (!endVar) {
						EEPROM_EnableMapping();
						funcPtr = (void(*)())0x0000;
						EIND = 0;
						funcPtr();
					}
					count=0;
				}
				break;
			}
		}
	}
}
void writePr(){
	uint16_t pageCounter=0;
	uint16_t pageAddress=0;
	uint8_t loaderStatus=0;

	while(1){
		requestToPC(loaderStatus,pageCounter,Flash_Page_Size/2);
		_delay_ms(20);
		if(loaderStatus) break;
		loaderStatus=responseFromPC(&pageCounter,tempBuffFlashPage,Flash_Page_Size,0);
		if (loaderStatus==Last_Pack || loaderStatus==Next_Pack){
			writeFlash(pageAddress,tempBuffFlashPage);
			pageCounter++;
			pageAddress=pageCounter * Flash_Page_Size;
		}
	}
}
void readSn(){
	uint16_t pageCounter=0;
	uint8_t loaderStatus=0;

	while(1){
		requestToPC(loaderStatus,pageCounter,(Flash_Page_Size/2));
		_delay_ms(20);
		if (loaderStatus){
			w5200_sendData(MAIN_CH,tempBuffFlashPage,Flash_Page_Size);
			break;
		}
		loaderStatus=responseFromPC(&pageCounter,tempBuffFlashPage,Flash_Page_Size, 1);
		
		if (loaderStatus==Last_Pack){
			readEEPROM(tempBuffFlashPage);
		}
		else loaderStatus=CRC_Err;
	}
}
void writeSn(){
	uint16_t pageCounter=0;
	uint8_t loaderStatus=0;

	while(1){
		requestToPC(loaderStatus,pageCounter,Flash_Page_Size/2);
		_delay_ms(20);
		if(loaderStatus) break;
		loaderStatus=responseFromPC(&pageCounter,tempBuffFlashPage,Flash_Page_Size,1);
		if (loaderStatus==Last_Pack || loaderStatus==Next_Pack){
			writeEEPROM(tempBuffFlashPage);
		}
	}
}
void writeEEPROM(uint8_t* buff){
	EEPROM_LoadPage(buff);
	//EEPROM_AtomicWritePage(EEPR_PAGE_ADDR);
	EEPROM_ErasePage(EEPR_PAGE_ADDR);
	EEPROM_SplitWritePage(EEPR_PAGE_ADDR);
}
void readEEPROM(uint8_t* buff){
	for (uint8_t i=0; i<EEPROM_PAGESIZE; i++){
		buff[i]=EEPROM_ReadByte(EEPR_PAGE_ADDR,i);
	}
}
void readFlash(uint16_t readAddress,uint8_t* buff){
	nvm_wait_until_ready();
	//SP_EraseFlashBuffer();
	//EEPROM_WaitForNVM();
	SP_ReadFlashPage(buff,readAddress);
	//EEPROM_WaitForNVM();
}
void writeFlash(uint16_t progAddress,uint8_t* buff){
	//SP_EraseFlashBuffer();
	EEPROM_WaitForNVM();
	SP_LoadFlashPage(buff);
	SP_EraseWriteApplicationPage(progAddress);
	//EEPROM_WaitForNVM();
}
uint8_t responseFromPC(uint16_t* pageCounter,uint8_t* buff,uint16_t pageSize,uint8_t disableDecoder){
	uint16_t lengthBuff = 0;
	uint8_t state=0, status_loader=0, type_byte=0;
	volatile bool exit = false;
	volatile uint16_t count = 0;
	uint8_t CRC_HIGH = 0x95;
	uint8_t CRC_LOW = 0x67;

	do{
		switch (state){
			case 0:{
				state=commandDecoder(3,buffer);
				count=3;
			}
			break;
			case 1:{
				status_loader=buffer[count++];
				*pageCounter=buffer[count++];						//page counter low byte
				*pageCounter |=(uint16_t)(buffer[count++]<<8);		//page counter upper byte
				state=2;
			}
			break;
			case 2:{
				type_byte ^= 0x01;
				if (lengthBuff != pageSize){
					if (disableDecoder) *buff++ = buffer[count];
					else *buff++ = Decoder_Byte(&buffer[count], type_byte, POLYNOM);
					calcCRC(buffer[count++], &CRC_HIGH, &CRC_LOW);
					lengthBuff++;
					//count++;
				}
				else{
					if (type_byte){
						if(CRC_LOW != buffer[count++]) status_loader = CRC_Err;
					}
					else{
						if(CRC_HIGH != buffer[count++]) status_loader = CRC_Err;
						exit = true;
					}
				}
			}
			break;
			default: {
				status_loader = MEM_Err;
				exit = true;
				break;
			}
		}
	}while(!exit);
	return status_loader;
}
void requestToPC(uint8_t loaderStatus, uint16_t pageCounter,uint16_t pageSize){
	uint8_t buff[]={0xfe,0xfe,loaderStatus,pageCounter,pageCounter>>8,pageSize,pageSize>>8};
	w5200_sendData(MAIN_CH,buff,0x07);
}
uint8_t commandDecoder(uint8_t length, uint8_t* buff){
	uint8_t counter=0;

	for (uint8_t i=0; i<length; i++){
		switch (buff[i]){
			case 'S':{
				if(counter==0x02) counter++;
			}
			break;
			case 'N':{
				if(counter==0x03) counter++;
			}
			break;
			case 'G':{
				if(counter==0x02) counter++;
			}
			break;
			case 'E':{
				if(counter==0x03) counter++;
			}
			break;
			case 'T':{
				if(counter==0x04) counter++;
			}
			break;
			case 'P':{
				if(counter==0x02) counter++;
			}
			break;
			case 0xfe:{
				counter++;
			}
			break;
			case 0x63:{
				if (counter==2) return 1;
				else return -1;
			}
			break;
			case 0xff:{
				if (counter==0x04){
					return WRITE_Sn;
				}
				if (counter==0x05){
					return READ_Sn;
				}
				if (counter==0x03){
					return WRITE_Pr;
				}
			}
			break;
			default: counter=0x00;
		}
	}
	return 0x00;
}
void flushBuff(uint8_t* buff){
	for (uint16_t i=0;i<BUFFER_SIZE;i++)	buff[i]=0x00;
}
void greenBLink(){
	static uint16_t i;
	if (i++ == 0xcfff){
		spi_setReg(&SPIC, &PORTQ, ledFanState ^= (1 << 5), PIN0_bm);
		i=0;
	}
}