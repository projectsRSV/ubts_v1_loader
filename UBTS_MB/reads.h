#ifndef READS_H_
#define READS_H_


#include "w5200.h"
#include "defines.h"
#include "ISR.h"


extern uint8_t buffer[];
extern uint8_t loaderAction;
extern uint8_t commandDecoder(uint8_t length, uint8_t* buff);

typedef void (*fpStatusLed)(void);
fpStatusLed blinkFuncPtr;
void utils_fastBlink();
void utils_middleBlink();
void utils_slowBlink();
extern fpStatusLed blinkPtrTable[];

void read_sendW5200();



#endif /* READS_H_ */