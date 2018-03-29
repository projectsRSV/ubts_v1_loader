#include "twi.h"


void twi_init(TWI_t *twi){
	twi->CTRL=0;
	twi->MASTER.BAUD = TWI_BAUD(F_CPU, 250000);
	twi->MASTER.CTRLA = 0;
	twi->MASTER.CTRLB = 0;
	//twi->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}
