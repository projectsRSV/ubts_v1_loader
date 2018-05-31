#include "initDevice.h"

void init_clock(){
	/*OSC.CTRL=OSC_RC32MEN_bm;
	OSC.PLLCTRL=OSC_PLLSRC_RC32M_gc;
	while ((OSC.STATUS&OSC_RC32MRDY_bm)==0);
	CCP=CCP_IOREG_gc;
	CLK.CTRL=CLK_SCLKSEL_RC32M_gc;*/
	OSC.XOSCCTRL=OSC_FRQRANGE_12TO16_gc|OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL=OSC_XOSCEN_bm;
	OSC.PLLCTRL=OSC_PLLSRC_XOSC_gc;
	while ((OSC.STATUS&OSC_XOSCRDY_bm)==0);
	CCP=CCP_IOREG_gc;
	CLK.CTRL=CLK_SCLKSEL_XOSC_gc;
}
void init_port(){
	
	/*PORTA.DIR=0xff;
	PORTA.OUT=0x00;
	PORTB.DIR=0xff;
	PORTB.OUT=0x00;*/
	
	PORTC.DIR=0xff;
	PORTC.OUT=0x00;
	/*PORTC.DIRSET=PIN3_bm;					//uart tx
	PORTC.OUTSET=PIN3_bm;					//1
	PORTC.DIRCLR=PIN2_bm;					//uart rx
	PORTC.OUTCLR=PIN2_bm;		*/			//0
	//PORTC.DIRCLR=PIN6_bm;
	
	PORTD.DIR=0xff;
	PORTD.OUT=0x00;
	//PORTD.OUTSET=PIN4_bm;		//ss
	//PORTD.DIRCLR=PIN6_bm;		//miso
	
	PORTE.DIR=0xff;
	PORTE.OUT=0x00;
	PORTE.OUTSET=PIN4_bm;						//set 1 for spi cs w5200
	PORTE.DIRSET=PIN7_bm|PIN5_bm|PIN4_bm;		//set pins as output spi outputs w5200
	PORTE.DIRCLR=PIN6_bm;						//set pin as input for miso w5200
	/*PORTE.DIRSET=PIN3_bm;					//uart tx
	PORTE.OUTSET=PIN3_bm;					//1
	PORTE.DIRCLR=PIN2_bm;					//uart rx
	PORTE.OUTCLR=PIN2_bm;					//0*/
	
	//PORTE.OUT=0x00;
	//PORTCFG.CLKEVOUT=PORTCFG_CLKOUT_PE7_gc;
	PORTF.DIR=0xff;
	PORTF.OUT=0x00;
	//PORTF.INTCTRL = PORT_INT0LVL_MED_gc;			//set input for interrupt from w5200
	//PORTF.DIRCLR=PIN2_bm;						
	//PORTF.INT0MASK=PIN2_bm;
	//PORTF.PIN2CTRL=PORT_OPC_PULLUP_gc | PORT_ISC_LEVEL_gc;
	PORTF.OUTSET=PIN3_bm;						//set 1 for reset w5200
	PORTF.PIN3CTRL=PORT_OPC_PULLUP_gc;			//reset w5200
	
	PORTF.DIRSET = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
	PORTF.OUTSET &= ~(PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);
	
	PORTJ.DIR=0xff;
	PORTJ.OUT=0x00;
	
	PORTH.DIR=0xff;
	PORTH.OUT=0x00;
	
	PORTK.DIR=0xff;
	PORTK.OUT=0x00;
	
	PORTQ.DIR=0xff;
	PORTQ.OUT=0x00;
}
void init_all(){
	cli();
	ISR_init();
	init_clock();
	init_port();
	spi_init(&SPIC, 1000);			//mcu ds0 (pc5,pc6 - pa_onoff,
	spi_init(&SPID, 1000);			//mcu ds1 (face led,
	spi_init(&SPIE, 1000000);		//w5200
	spi_resetRegs();
	usart_init(&USARTE0, BAUD_RATE);
	
	usart_rx_disable(&USARTC0);
	usart_rx_disable(&USARTD0);
	usart_tx_disable(&USARTC0);
	usart_tx_disable(&USARTD0);
	
	twi_init(&TWIC);		//temp board
	twi_init(&TWID);		//temp pa0, pa1
	twi_init(&TWIE);		//ina sensors
	twi_init(&TWIF);		//temp pa2, pa3
	
	reset_w5200();
	w5200_init();
	sei();
}
void reset_w5200(){
	RESET_w5200_on;
	_delay_us(2);
	RESET_w5200_off;
	_delay_ms(150);
}


