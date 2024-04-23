#ifndef KEYS_H
#define KEYS_H
#include "pico/stdlib.h"

enum Keys{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_MID,
};

#define KEY_LEFT_PIN 6
#define KEY_RIGHT_PIN 7
#define KEY_MID_PIN 8

void app_keys_init();
void app_keys_update();

bool keys_is_pressed(enum Keys k);
bool keys_is_released(enum Keys k);
bool keys_is_hold(enum Keys k);

#endif