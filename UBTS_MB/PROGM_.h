#ifndef PROGMEM__H_
#define PROGMEM__H_


#include <avr/pgmspace.h>

extern const uint8_t _STATUS[] PROGMEM;
extern const uint8_t _LINE_FEED[] PROGMEM;
extern const uint8_t _OPEN[] PROGMEM;
extern const uint8_t _LISTEN[] PROGMEM;
extern const uint8_t _CLOSE[] PROGMEM;

extern const uint8_t _SEND[] PROGMEM;
extern const uint8_t _RECV[] PROGMEM;
extern const uint8_t _STATUS_LOADER[] PROGMEM;
extern const uint8_t _PAGE[] PROGMEM;


/*
extern const uint8_t _MSS[] PROGMEM;
extern const uint8_t _IR2[] PROGMEM;
extern const uint8_t _IMR2[] PROGMEM;
extern const uint8_t _SOCKET[] PROGMEM;
extern const uint8_t _MAC_DESTIN[] PROGMEM;
extern const uint8_t _IP_SOURCE[] PROGMEM;
extern const uint8_t _IP_SUB[] PROGMEM;
extern const uint8_t _IP_GATE[] PROGMEM;
extern const uint8_t _MAC_ADDR[] PROGMEM;
extern const uint8_t _PHY_STATUS[] PROGMEM;
extern const uint8_t _SOCKET_MR[] PROGMEM;
extern const uint8_t _RX_MEM_SIZE[] PROGMEM;
extern const uint8_t _TX_MEM_SIZE[] PROGMEM;*/
extern const uint8_t _INTERRUPT[] PROGMEM;
extern const uint8_t _IP_DESTIN[] PROGMEM;

#endif /* PROGMEM__H_ */
