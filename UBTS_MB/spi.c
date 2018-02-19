#include "spi.h"

void spi_init(SPI_t *spi, uint32_t baudrate){
	spi_xmega_set_baud_div(spi,baudrate,F_CPU);
	spi_enable_master_mode(spi);
	spi_enable(spi);
}
void spi_xmega_set_baud_div(SPI_t *spi, uint32_t baudrate, uint32_t clkper_hz){
	uint32_t divisor;
	uint8_t divisor_8bit;
	uint8_t ctrl;

	divisor = (clkper_hz + baudrate - 1) / baudrate;

	divisor_8bit = divisor;

	if (divisor_8bit > 64) {
		ctrl = SPI_PRESCALER_DIV128_gc;
	}
	else if (divisor_8bit > 32) {
		ctrl = SPI_PRESCALER_DIV64_gc;
	}
	else if (divisor_8bit > 16) {
		ctrl = SPI_CLK2X_bm | SPI_PRESCALER_DIV64_gc;
	}
	else if (divisor_8bit > 8) {
		ctrl = SPI_PRESCALER_DIV16_gc;
	}
	else if (divisor_8bit > 4) {
		ctrl = SPI_CLK2X_bm | SPI_PRESCALER_DIV16_gc;
	}
	else if (divisor_8bit > 2) {
		ctrl = SPI_PRESCALER_DIV4_gc;
	}
	else {
		ctrl = SPI_CLK2X_bm | SPI_PRESCALER_DIV4_gc;
	}

	spi->CTRL = (spi->CTRL & ~(SPI_CLK2X_bm | SPI_PRESCALER_gm)) | ctrl;
}
void spi_setReg(SPI_t* spi,PORT_t *port,uint8_t data, uint8_t pinDS){
	spi_sendData(spi,data);
	
	_delay_us(1);
	port->OUTSET=pinDS;
	_delay_us(5);
	port->OUTCLR=pinDS;
}
void spi_resetRegs(){
	spi_sendData(&SPIC,0);
	spi_sendData(&SPID,0);
	
	PORTF.OUTSET=PIN0_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
	_delay_us(1);
	PORTF.OUTCLR=PIN0_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
	//nm,gps,wifi,1Hz,10MHz,PPS
	PORTK.OUTSET=PIN6_bm | PIN7_bm;
	_delay_us(1);
	PORTK.OUTCLR=PIN6_bm | PIN7_bm;
	
	PORTQ.OUTSET=PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	_delay_us(1);
	PORTQ.OUTCLR=PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	
	PORTJ.OUTSET=PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN5_bm | PIN6_bm | PIN7_bm;
	_delay_us(1);
	PORTJ.OUTCLR=PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN5_bm | PIN6_bm | PIN7_bm;
	
	PORTH.OUTSET=PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
	_delay_us(1);
	PORTH.OUTCLR=PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
	
}