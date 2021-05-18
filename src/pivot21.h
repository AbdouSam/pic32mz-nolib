#ifndef PIVOT21_H
#define PIVOT21_H

void pivot21_init(void);

void pivot21_task(void);

int pivot21_set_parameter(const char *frame);


int pivot21_read_parameter(const char *frame, char *retframe);

int pivot21_set_parameter_default(void);

#endif // PIVOT21_H
