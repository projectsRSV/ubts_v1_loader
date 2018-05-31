#include "w5200.h"

void w5200_init(void){
	uint8_t buffer_RST=0x80;
	uint8_t buffer_IMR=0x00;
	uint8_t buffer_IMR2=0x00;
	uint8_t buffer_MR=0x01;

	
	uint8_t buffer_GATE[] = {192,168,1,1};
	uint8_t buffer_Subnet[] = {255,255,255,0};
	uint8_t buffer_MAC[] = {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
	uint8_t buffer_IP_source[] = {192,168,1,3};
	uint8_t buffer_SOCKET_main[]={MAIN_PORT >> 8, (uint8_t)MAIN_PORT};	//5000
	
	w5200_writeData(MR_W5200,1,&buffer_RST);							//reset
	_delay_ms(1);
	w5200_writeData(IMR_W5200,1,&buffer_IMR);
	w5200_writeData(IMR2_W5200,1,&buffer_IMR2);
	w5200_writeData(PHY_STATUS,1,&buffer_IMR2);
	
	w5200_writeData(GATEWAY,4,buffer_GATE);
	w5200_writeData(SUBNET,4,buffer_Subnet);
	w5200_writeData(MAC,6,buffer_MAC);
	w5200_writeData(IP_source,4,buffer_IP_source);
	
	w5200_writeData(Sn_MR(MAIN_CH),1,&buffer_MR);							//0x01 set TCP mode
	w5200_writeData(Sn_PORT(MAIN_CH),2,buffer_SOCKET_main);					//set source port number (socket)
	
	//w5200_openSocket(MAIN_CH);
}

void w5200_readData(uint16_t addr,uint16_t length, uint8_t* buff){
	CS_ON;
	spi_sendData(&SPIE,(addr)>>8);					//addr upper byte
	spi_sendData(&SPIE,(addr));						//addr lower byte
	spi_sendData(&SPIE,_READ|(length>>8));			//data length upper byte
	spi_sendData(&SPIE,(length));					//data length lower byte
	for (uint16_t i=0;i<length;i++){
		buff[i]=spi_sendData(&SPIE,0x00);
	}
	CS_OFF;
}
void w5200_writeData(uint16_t addr,uint16_t length,uint8_t* buff){
	CS_ON;
	spi_sendData(&SPIE,(addr)>>8);				//addr upper byte
	spi_sendData(&SPIE,addr);					//addr lower byte
	spi_sendData(&SPIE,(_WRITE)|(length>>8));	//data length upper byte
	spi_sendData(&SPIE,(length));				//data length lower byte
	for (uint16_t i=0;i<length;i++){
		spi_sendData(&SPIE,buff[i]);
	}
	CS_OFF;
}
void w5200_writeByte(uint16_t addr, uint8_t data){
	CS_ON;
	spi_sendData(&SPIE,(addr)>>8);				//addr upper byte
	spi_sendData(&SPIE,addr);					//addr lower byte
	spi_sendData(&SPIE,(_WRITE));				//data length upper byte
	spi_sendData(&SPIE,(0x01));					//data length lower byte
	spi_sendData(&SPIE,data);
	CS_OFF;
}
uint8_t w5200_readByte(uint16_t addr){
	uint8_t data;
	CS_ON;
	spi_sendData(&SPIE,(addr)>>8);				//addr upper byte
	spi_sendData(&SPIE,addr);					//addr lower byte
	spi_sendData(&SPIE,(_READ));				//data length upper byte
	spi_sendData(&SPIE,(0x01));					//data length lower byte
	data=spi_sendData(&SPIE,0x00);
	CS_OFF;
	return data;
}
uint16_t getSn_RegValue(uint16_t reg){
	uint16_t val=0,val1=0;
	do
	{
		val = w5200_readByte(reg);
		val = (val << 8) + w5200_readByte(reg + 1);
		if (val != 0){
			val1 = w5200_readByte(reg);
			val1 = (val1 << 8) + w5200_readByte(reg + 1);
		}
	} while (val != val1);
	return val;
}
void w5200_sendData(uint8_t ch, uint8_t *buff, uint16_t length){
	uint16_t ptr;
	uint16_t upperSizeByte;
	uint16_t dst_ptr;
	uint16_t dst_mask;
	
	
	while (getSn_RegValue(Sn_TX_FSR(ch)) < length){};
		
	dst_mask = getSn_RegValue(Sn_TX_WR(ch)) & RX_MASK;
	dst_ptr = TX_BASE(ch) + dst_mask;								//physical start address
	
	if( (dst_mask + length) > (RX_MASK + 1)){
		upperSizeByte = (RX_MASK + 1) - dst_mask;
		w5200_writeData(dst_ptr, upperSizeByte ,buff);
		buff += upperSizeByte;
		upperSizeByte = length - upperSizeByte;
		w5200_writeData(TX_BASE(ch), upperSizeByte, buff);
	}
	else{
		w5200_writeData(dst_ptr, length, buff);
	}
	ptr = getSn_RegValue(Sn_TX_WR(ch)) + length;
	
	w5200_writeByte(Sn_TX_WR(ch), ptr>>8);
	w5200_writeByte(Sn_TX_WR(ch) + 1, ptr);
	w5200_writeByte(Sn_CR(ch), _SEND_COMMAND);									//send command
	//_delay_ms(5);
}
uint16_t w5200_recvData(uint8_t ch,uint8_t *buff){
	uint16_t ptr;
	uint16_t length;
	uint16_t upperSizeByte;
	uint16_t src_ptr;
	uint16_t src_mask;

	length = getSn_RegValue(Sn_RX_RSR(ch));
	
	if(length > 0){
		src_mask = getSn_RegValue(Sn_RX_RD(ch)) & RX_MASK;
		src_ptr = RX_BASE(ch) + src_mask;								//physical start address
		
		if( (src_mask + length) > (RX_MASK+1)){
			upperSizeByte = (RX_MASK+1) - src_mask;
			w5200_readData(src_ptr, upperSizeByte, buff);
			buff += upperSizeByte;
			upperSizeByte = length - upperSizeByte;
			w5200_readData(RX_BASE(ch) ,upperSizeByte, buff);
		}
		else{
			w5200_readData(src_ptr, length, buff);
		}
		
		ptr = getSn_RegValue(Sn_RX_RD(ch)) + length;;
		
		w5200_writeByte(Sn_RX_RD(ch), ptr>>8);
		w5200_writeByte(Sn_RX_RD(ch) + 1, ptr);
		w5200_writeByte(Sn_CR(ch), _RECV_COMMAND);									//receive command
	}
	return length;
}
void w5200_closeSocket(uint8_t ch){
	w5200_writeByte(Sn_CR(ch),0x10);									//close command
}
void w5200_discSocket(uint8_t ch){
	w5200_writeByte(Sn_CR(ch),0x08);									//disconnect command
}
void w5200_openSocket(uint8_t ch){
	uint8_t temp;
	
	w5200_writeByte(Sn_CR(ch), _OPEN_COMMAND);															//open
	temp = w5200_readStatus(ch);
	switch(temp){
		case _SOCK_INIT:{
			w5200_writeByte(Sn_CR(ch), _LISTEN_COMMAND);												//listen
			temp = w5200_readStatus(ch);
			if (temp != _SOCK_LISTEN) w5200_writeByte(Sn_CR(ch), _CLOSE_COMMAND);						//close
			break;
		}
		case _SOCK_LISTEN:{
			break;
		}
		default:{
			w5200_writeByte(Sn_CR(ch), _CLOSE_COMMAND);													//close
			break;
		}
	}
}
uint8_t w5200_readSocketInt(uint8_t ch){
	return w5200_readByte(Sn_IR(ch));
}
uint8_t w5200_readInterChann(){
	return w5200_readByte(IR2_W5200);
}
uint8_t w5200_readInterrupt(){
	return w5200_readByte(IR_W5200);
}
uint8_t w5200_readStatus(uint8_t ch){
	return w5200_readByte(Sn_SR(ch));
}
void readSpiSendUart(USART_t* usart, uint8_t length, uint8_t* buff, uint16_t addr){
	w5200_readData(addr,length,buff);
	usart_sendData_toAscii(usart,length,buff);
}
uint8_t utils_returnOrderedNum(uint8_t* interReg){
	uint8_t count=0,k=1;
	for (uint8_t i=0; i<8; i++){
		if ((*interReg & (k<<i))){
			*interReg &= ~(k<<i);
			break;
		}else count++;
	}
	return count;
}