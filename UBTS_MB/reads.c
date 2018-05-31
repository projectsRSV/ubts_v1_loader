#include "reads.h"


fpStatusLed blinkPtrTable[] = {utils_fastBlink, utils_middleBlink, utils_slowBlink};



void read_sendW5200(){
	uint8_t status;
	//uint16_t buffLength;
	
	status = w5200_readStatus(MAIN_CH);
	switch(status){
		case _SOCK_CLOSED:{
			w5200_openSocket(MAIN_CH);
			break;
		}
		case _SOCK_LISTEN:{
			//blinkFuncPtr = (fpStatusLed)(blinkPtrTable[2]);
			break;
		}
		case _SOCK_ESTABL:{
			w5200_recvData(MAIN_CH, buffer);
			//usart_sendData_toAscii(&USARTE0,buffLength,buffer);			//logging
			break;
		}
		case _SOCK_CLOSE_WAIT:{
			blinkFuncPtr = (fpStatusLed)(blinkPtrTable[0]);
			w5200_discSocket(MAIN_CH);
			break;
		}
	}
}

void utils_fastBlink(){
	static uint16_t i=0;
	if (i++ >= 0x3df){
		TGL_RED_LED_left;
		TGL_RED_LED_right;
		i=0;
	}
}
void utils_middleBlink(){
	static uint16_t i=0;
	if (i++ >= 0x07df){
		TGL_RED_LED_left;
		TGL_RED_LED_right;
		i=0;
	}
}
void utils_slowBlink(){
	static uint16_t i=0;
	if (i++ >= 0x0fdf){
		TGL_RED_LED_left;
		TGL_RED_LED_right;
		i=0;
	}
}