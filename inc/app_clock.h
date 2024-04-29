#ifndef APP_CLOCK_H
#define APP_CLOCK_H

#include "../inc/app_interface.h"

void app_clock_setup();
void app_clock_create();
void app_clock_update();
void app_clock_draw();
void app_clock_destroy();
bool app_clock_captured_input();

extern struct AppInterface_t app_clock_interface;

#endif