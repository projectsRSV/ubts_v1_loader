#include "CRC.h"



//****************************************************************************//
//************************  Calc CRC of Packet    ****************************//
//****************************************************************************//
void calcCRC(uint8_t data, uint8_t *crc_h, uint8_t *crc_l ){
	
	uint8_t bit_result = 0;
	uint16_t crc;

	crc = (*crc_h << 8) | *crc_l;

	for(uint8_t i = 0; i < 8; i++ ){
		bit_result = data ^ crc ^ (crc >> 1);
		//bit_result ^= crc >> 1;
		bit_result &= 0x01;
		data >>= 1;
		
		crc >>= 1;
		crc |= bit_result << 15;
	}
	
	*crc_l = crc;
	*crc_h = crc >> 8;	
}
