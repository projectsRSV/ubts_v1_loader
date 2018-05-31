#include "ISR.h"
#include "defines.h"



void ISR_init(void){
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = PMIC_IVSEL_bm;
}

/***************interrupt vectors******************/
/*ISR(PORTF_INT0_vect){
	uint8_t ch,interReg,socketInt;

//blinkFuncPtr = (fpStatusLed)(blinkPtrTable[0]);				//speed of led blinking

	interReg = w5200_readInterChann()&0x01;
	while(interReg){
		ch=utils_returnOrderedNum(&interReg);
		socketInt=w5200_readSocketInt(ch);
		switch (socketInt) {
			case 0x02:{														//disconnect
				w5200_writeByte(Sn_IR(ch), socketInt);
				w5200_discSocket(ch);
				w5200_openSocket(ch);
				break;
			}
			case 0x04:{														//receive
				uint16_t buffLength;
				buffLength=w5200_recvData(MAIN_CH,buffer);
				//usart_sendData_toAscii(&USARTE0,buffLength,buffer);			//logging
				loaderAction = commandDecoder(buffLength,buffer);
				w5200_writeByte(Sn_IR(ch), socketInt);
				break;
			}
			default:{
				w5200_writeByte(Sn_IR(ch), socketInt);
				break;
			}
		}
	}
}*/

