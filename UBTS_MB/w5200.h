
#ifndef W5200_H_
#define W5200_H_

#include "defines.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "w5200_defines.h"
#include "spi.h"
#include "usart.h"
//#include "util.h"
//#include "PROGM_.h"

void w5200_readData(uint16_t addr,uint16_t length,uint8_t* buffer);
void w5200_writeData(uint16_t addr,uint16_t length,uint8_t* buff);
void w5200_writeByte(uint16_t addr, uint8_t data);
uint8_t w5200_readByte(uint16_t addr);
uint16_t getSn_RegValue(uint16_t reg);
uint16_t w5200_recvData(uint8_t ch,uint8_t *);
void w5200_sendData(uint8_t ch,uint8_t *buff, uint16_t length);
void w5200_sendDataPGM(uint8_t ch,const uint8_t *buff);
void w5200_init(void);


void readSpiSendUart(USART_t* usart, uint8_t length, uint8_t* buffer, uint16_t addr);
uint8_t w5200_readStatus(uint8_t ch);
uint8_t w5200_readSocketInt(uint8_t ch);
uint8_t w5200_readInterChann();
uint8_t w5200_readInterrupt();
void w5200_openSocket(uint8_t ch);
void w5200_closeSocket(uint8_t ch);
void w5200_discSocket(uint8_t ch);
void ledBlink();
uint8_t utils_returnOrderedNum(uint8_t* temp);


#endif /* W5200_H_ */