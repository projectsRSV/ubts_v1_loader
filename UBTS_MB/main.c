#include <avr/io.h>
#include "initDevice.h"
#include "sp_driver.h"
#include "eeprom_driver.h"
#include "PROGM_.h"
#include "CRC.h"
#include "Decoder.h"
#include "reads.h"


//void ledBlink();
uint8_t commandDecoder(uint8_t length, uint8_t* buff);

void flushBuff(uint8_t* buff);

void writeSn();
void readSn();
void writePr();

void requestToPC(uint8_t loaderStatus, uint16_t pageCounter,uint16_t pageSize);
uint8_t responseFromPC(uint16_t* pageCounter,uint8_t* buff,uint16_t pageSize,uint8_t disableDecoder);
void writeFlash(uint16_t progAddress,uint8_t* buff);
void readFlash(uint16_t readAddress,uint8_t* buff);
void writeEEPROM(uint8_t* buff);
void readEEPROM(uint8_t* buff);

void (*funcPtr)();

uint8_t buffer[BUFFER_SIZE];
uint8_t tempBuffFlashPage[Flash_Page_Size];
uint8_t loaderAction=0;
uint8_t actions = GO_FROM_LOADER;
//bool isEnd = true;

uint8_t POLYNOM[] = {0x93,0xda,0x16,0xea};
//uint8_t buff[BUFFER_SIZE];
uint8_t ledFanState;
uint8_t loaderStatus;

typedef void (*fpFanLed)();
fpFanLed fanLedFuncPtr;
void greenBLink();
fpFanLed lightPtrTable[] = {greenBLink};

int main(void){
	const uint32_t COUNT = 60000;
	uint32_t count = 0;
	
	init_all();
	EEPROM_FlushBuffer();
	EEPROM_DisableMapping();
	
	blinkFuncPtr = (fpStatusLed)(blinkPtrTable[2]);				//speed of led blinking
	fanLedFuncPtr = (fpFanLed)(lightPtrTable[0]);				//green led blink
	
	while (1){
		read_sendW5200();
		blinkFuncPtr();
		fanLedFuncPtr();
		
		loaderAction = commandDecoder(6, buffer);
		
		switch (loaderAction){
			case WRITE_Pr:{
				loaderAction=0x00;
				writePr();
				break;
			}
			case WRITE_Sn:{
				loaderAction=0x00;
				writeSn();
				actions = STAY_IN_LOADER;
				break;
			}
			case READ_Sn:{
				loaderAction=0x00;
				readSn();
				if (actions == SUCC_WRITE) {
					count = 52000;
					actions = GO_FROM_LOADER;
				}
				else actions = STAY_IN_LOADER;
				break;
			}
			default:{
				if (count++ >= COUNT){
					if (actions == GO_FROM_LOADER){
						EEPROM_EnableMapping();
						funcPtr = (void(*)())0x0000;
						EIND = 0;
						funcPtr();
					}
					else if (actions == CRASH_WRITE){
						funcPtr = (void(*)())0x10000;
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
	uint8_t loaderSt = 0;
	uint8_t count = 0;
	
	actions = CRASH_WRITE;
	
	while(1){
		requestToPC(loaderSt, pageCounter, Flash_Page_Size / 2);
		count++;
		if(loaderSt) {
			if (loaderSt == Last_Pack) actions = SUCC_WRITE;
			else actions = CRASH_WRITE;
			break;
		}
		_delay_ms(10);
		read_sendW5200();
		loaderSt = responseFromPC(&pageCounter, tempBuffFlashPage, Flash_Page_Size, 0);
		if ((loaderSt == Last_Pack) || (loaderSt == Next_Pack)){
			writeFlash(pageAddress, tempBuffFlashPage);
			pageCounter++;
			pageAddress = pageCounter * Flash_Page_Size;
		}
		else {
			loaderSt = MEM_Err;
		}
		
		if (count != pageCounter) {
			loaderSt = CRC_Err;
		}
	}
}
void readSn(){
	uint16_t pageCounter=0;
	uint8_t loaderSt=0;

	while(1){
		requestToPC(loaderSt, pageCounter, Flash_Page_Size / 2);
		_delay_ms(50);
		read_sendW5200();
		if (loaderSt){
			w5200_sendData(MAIN_CH, tempBuffFlashPage, Flash_Page_Size);
			break;
		}
		loaderSt = responseFromPC(&pageCounter, tempBuffFlashPage, Flash_Page_Size, 1);
		
		if (loaderSt == Last_Pack){
			readEEPROM(tempBuffFlashPage);
		}
		else loaderSt=CRC_Err;
	}
}
void writeSn(){
	uint16_t pageCounter=0;
	uint8_t loaderSt=0;

	while(1){
		requestToPC(loaderSt, pageCounter, Flash_Page_Size/2);
		_delay_ms(50);
		if(loaderSt) break;
		read_sendW5200();
		loaderSt = responseFromPC(&pageCounter, tempBuffFlashPage, Flash_Page_Size,1);
		if (loaderSt==Last_Pack || loaderSt==Next_Pack){
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
	SP_EraseFlashBuffer();
	EEPROM_WaitForNVM();
	SP_LoadFlashPage(buff);
	SP_EraseWriteApplicationPage(progAddress);
	//EEPROM_WaitForNVM();
}
uint8_t responseFromPC(uint16_t* pageCounter,uint8_t* buff,uint16_t pageSize,uint8_t disableDecoder){
	uint16_t lengthBuff = 0;
	uint8_t state=0, loaderSt=0, type_byte=0;
	bool exit = false;
	uint16_t count = 0;
	uint8_t CRC_HIGH = 0x95;
	uint8_t CRC_LOW = 0x67;

	do{
		switch (state){
			case 0:{
				state = commandDecoder(3, buffer);
				count=3;
				break;
			}
			case 1:{
				loaderSt = buffer[count++];
				*pageCounter=buffer[count++];							//page counter low byte
				*pageCounter |= (uint16_t)(buffer[count++] << 8);		//page counter upper byte
				state=2;
				break;
			}
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
						if(CRC_LOW != buffer[count++]) {
							loaderSt = CRC_Err;
							exit = true;
						}
					}
					else{
						if(CRC_HIGH != buffer[count++]) {
							loaderSt = CRC_Err;
							exit = true;
						}
						exit = true;
					}
				}
				break;
			}
			default: {
				loaderSt = MEM_Err;
				exit = true;
				break;
			}
		}
	}while(!exit);
	return loaderSt;
}
void requestToPC(uint8_t loaderSt, uint16_t pageC, uint16_t pageS){
	uint8_t buff[]={0xfe, 0xfe, loaderSt, pageC, pageC>>8, pageS, pageS >> 8};
	//uint8_t buff[]={0xfe, 0xfe, loaderSt, pageC, pageC>>8, 0xaa, 0xbb};
	w5200_sendData(MAIN_CH, buff, 0x07);
}
uint8_t commandDecoder(uint8_t length, uint8_t* buff){
	uint8_t counter=0;
	for (uint8_t i=0; i<length; i++){
		switch (buff[i]){
			case 'S':{
				if(counter==0x02) counter++;
				break;
			}
			case 'N':{
				if(counter==0x03) counter++;
				break;
			}
			case 'G':{
				if(counter==0x02) counter++;
				break;
			}
			case 'E':{
				if(counter==0x03) counter++;
				break;
			}
			case 'T':{
				if(counter==0x04) counter++;
				break;
			}
			case 'P':{
				if(counter==0x02) counter++;
				break;
			}
			case 0xfe:{
				counter++;
				break;
			}
			case 0x63:{
				if (counter==2) return 0x01;
				else return 0xff;
				break;
			}
			case 0xff:{
				if (counter==0x04){
					return WRITE_Sn;
				}
				else if (counter==0x05){
					return READ_Sn;
				}
				else	if (counter==0x03){
					return WRITE_Pr;
				}
				break;
			}
			default: {
				return 0x00;
				//break;
			}
		}
	}
	return 0x00;
}
void flushBuff(uint8_t* buff){
	for (uint16_t i=0;i<BUFFER_SIZE;i++) buff[i]=0x00;
}
void greenBLink(){
	static uint16_t i;
	if (i++ == 0x0fff){
		spi_setReg(&SPIC, &PORTQ, ledFanState ^= (1 << 5), PIN0_bm);
		i=0;
	}
}