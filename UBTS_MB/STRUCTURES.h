#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include "defines.h"



struct myFIFO{	
	uint8_t head;
	uint8_t tail;
	uint8_t data[BUFFER_SIZE];	
};


#endif /* STRUCTURES_H_ */