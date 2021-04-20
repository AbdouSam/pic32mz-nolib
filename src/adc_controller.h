#ifndef _ADCCONTROLLER_H
#define _ADCCONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

int adc_controller_readadc(int ch, uint16_t *val);


#endif /* _ADCCONTROLLER_H */
