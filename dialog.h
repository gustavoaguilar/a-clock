#ifndef DIALOG_H
#define DIALOG_H
#include "pico/stdlib.h"

uint32_t dialog_get_uint32(const char* title, const char* content, const uint32_t initial_value);
uint32_t dialog_get_uint32_range(const char* title, const char* content,
                                 const uint32_t initial_value,
                                 const uint32_t min, const uint32_t max);
void display_dialog(char* title, char* content);

#endif