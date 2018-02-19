#include "Decoder.h"



uint8_t Decoder_Byte(uint8_t* byte,uint8_t type,uint8_t* polynom ){
	uint8_t returnData;
	uint8_t temp;
	uint32_t tempPoly;

	if (type){
		returnData=*byte ^ *polynom;
	}
	else{
		tempPoly=((uint32_t)(*polynom)<<24);
		tempPoly |=((uint32_t)(*(++polynom))<<16);
		tempPoly |=((uint32_t)(*(++polynom))<<8);
		tempPoly |=*(++polynom);

		returnData=*byte ^ *(polynom-2);
		temp=(*(polynom-1) & 0x08);
		temp ^=(*polynom & 0x02)<<2;
		temp ^=(*polynom & 0x10)>>1;
		tempPoly >>=1;
		if (temp){
			tempPoly +=0x80000000;
		}
		*polynom --=tempPoly;
		*polynom --=tempPoly>>8;
		*polynom --=tempPoly>>16;
		*polynom=tempPoly>>24;
	}
	return returnData;
}
