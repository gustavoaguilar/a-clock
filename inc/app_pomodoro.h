#ifndef APP_POMODORO_H
#define APP_POMODORO_H

#include "../inc/app_interface.h"

void app_pomodoro_setup();
void app_pomodoro_create();
void app_pomodoro_update();
void app_pomodoro_destroy();
bool app_pomodoro_captured_input();

extern struct AppInterface_t app_pomodoro_interface;

#endif