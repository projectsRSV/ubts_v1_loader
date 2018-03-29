
#ifndef INITDEVICE_H_
#define INITDEVICE_H_

#include <avr/io.h>
#include "defines.h"
#include "usart.h"
#include <util/delay.h>
#include "w5200.h"
#include "twi.h"

void init_all(void);
void init_clock(void);
void init_port(void);
void reset_w5200(void);

#endif /* INITDEVICE_H_ */