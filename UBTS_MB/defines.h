#include <avr/io.h>
#include "ISR.h"

#define F_CPU				14745600
//#define F_CPU				32000000
//#define spi_buadrate		2000000

#define MAIN_CH			0
#define MAIN_PORT		5000


#define CS_ON				PORTE.OUTCLR=PIN4_bm
#define CS_OFF				PORTE.OUTSET=PIN4_bm
#define RESET_w5200_on		PORTF.OUTCLR=PIN3_bm
#define RESET_w5200_off		PORTF.OUTSET=PIN3_bm
#define INT_w5200			PORTF.IN&PIN2_bm

/*#define MOSI				P1 ^ 0; / * this declaration assigns pins of * /
#define MISO				P1 ^ 1; / * Port 1 as SPI pins * /
#define SCK					P1 ^ 2;*/

#define TGL_RED_LED_left		PORTC.OUTTGL=PIN4_bm
#define TGL_RED_LED_right		PORTD.OUTTGL=PIN4_bm
#define ON_RED_LED_left			PORTC.OUTCLR=PIN4_bm
#define ON_RED_LED_right		PORTD.OUTCLR=PIN4_bm
#define OFF_RED_LED_left		PORTC.OUTSET=PIN4_bm
#define OFF_RED_LED_right		PORTD.OUTSET=PIN4_bm

#define BUFFER_SIZE			Flash_Page_Size + 10

#define BAUD_RATE			115200


//****************************************************************************//
//************************  Type Loader  *************************************//
//****************************************************************************//
#define   WRITE_Pr                     0x02
#define   WRITE_Sn	                   0x03
#define   READ_Sn		               0x04

//#define		RECV_END					0xcc

#define   CRC_Err                          0xAF
#define   MEM_Err                          0xFF
#define   Last_Pack                        0x01
#define   Next_Pack                        0x00


#define  STAY_IN_LOADER						0xaa
#define  CRASH_WRITE						0xcc
#define  GO_FROM_LOADER						0xbb
#define  SUCC_WRITE							0xdd
//****************************************************************************//
//******************************* For System Board ***************************//
//****************************************************************************//
#define Boot_Reset_Address				BOOT_SECTION_START
#define Flash_Page_Size					APP_SECTION_PAGE_SIZE
#define Sn_Device_Address				Boot_Reset_Address - Flash_Page_Size

#define EEPR_PAGE_ADDR					1


