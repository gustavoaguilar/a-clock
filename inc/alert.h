#ifndef ALERT_H
#define ALERT_H
#include<unistd.h>

void alert_init();

void alert_blink();
void alert_blink_hz(uint8_t freq);
void alert_set(bool status);

#endif