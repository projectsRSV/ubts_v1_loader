#include <avr/io.h>
#include <avr/interrupt.h>
#include "w5200.h"
//#include "PROGM_.h"


void ISR_init(void);
extern uint8_t commandDecoder(uint8_t length, uint8_t* buff);

typedef void (*fpStatusLed)(void);
fpStatusLed blinkFuncPtr;
void utils_fastBlink();
void utils_middleBlink();
void utils_slowBlink();
//extern const fpBlinkLed blinkPtrTable[] PROGMEM;
extern fpStatusLed blinkPtrTable[];

extern uint8_t buffer[];
extern uint8_t loaderAction;
//extern uint8_t endVar;
//extern uint16_t buffLength;
