#ifndef _PIVOT_PFE_H
#define _PIVOT_PFE_H

#include <stdint.h>

void pivot_pfe_task(void);

void pivot_pfe_init(void);

void pivot_pfe_request(int req, char *retframe);

void pivot_pfe_activation(int state, char *retframe);

void pivot_pfe_set(int mode, char *retframe);

void get_config_data(uint16_t motor_time, uint16_t pump_time, char *retframe);

#endif // _PIVOT_PFE_H