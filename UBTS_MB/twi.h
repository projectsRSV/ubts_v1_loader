#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <stdbool.h>
#include "structures.h"
#include "initDevice.h"



#define TWI_BAUD(F_CPU, F_TWI)			(F_CPU / (2 * F_TWI)) - 5



void twi_init(TWI_t *twi);


#endif /* TWI_H_ */