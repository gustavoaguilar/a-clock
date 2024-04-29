#ifndef APP_INTERFACE_H
#define APP_INTERFACE_H
#include "pico/stdlib.h"

struct AppInterface_t{
    void (*create)(void);
    void (*destroy)(void);
    void (*update)(void);

    bool (*captured_input)(void);
};

/*
#include "../inc/app_interface.h"

void app_NAME_setup();
void app_NAME_create();
void app_NAME_update();
void app_NAME_destroy();
bool app_NAME_captured_input();

extern struct AppInterface_t app_NAME_interface;
*/

#endif