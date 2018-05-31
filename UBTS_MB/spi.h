
#ifndef _SPI_H_
#define _SPI_H_

#include <avr/io.h>
#include "defines.h"
#include <util/delay.h>

void spi_xmega_set_baud_div(SPI_t *spi, uint32_t baudrate, uint32_t clkper_hz);
void spi_init(SPI_t *spi, uint32_t baudrate);
void spi_setReg(SPI_t* spi,PORT_t *port,uint8_t data, uint8_t pinDS);
void spi_resetRegs();


static inline void spi_enable(SPI_t *spi){
	spi->CTRL |= SPI_ENABLE_bm;
}

static inline void spi_enable_master_mode(SPI_t *spi){
	spi->CTRL |= SPI_MASTER_bm | SPI_MODE_0_gc;
	spi->INTCTRL = 0;
}

static inline void spi_disable(SPI_t *spi){
	spi->CTRL &= ~SPI_ENABLE_bm;
}

static inline uint8_t spi_sendData(SPI_t *spi, uint8_t data){
	spi->DATA = data;
	while((spi->STATUS & SPI_IF_bm) != SPI_IF_bm) {};
	return spi->DATA;
}

//uint8_t spi_send_w5200(uint8_t byte);

//static inline uint8_t spiRecvData(SPI_t *spi){
	//return spi->DATA;
//}

#endif  // _SPI_H_
