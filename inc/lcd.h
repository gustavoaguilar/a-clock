#ifndef LCD_H
#define LCD_H

#include "pico/stdlib.h"

#define LCD_CHARS 16
#define LCD_LINES 2

#define LCD_E 17
#define LCD_RS 16
#define LCD_D4 18
#define LCD_D5 19
#define LCD_D6 20
#define LCD_D7 21

void lcd_send_string(char* str, uint32_t len);
void lcd_update_line(const char* str, const int line);

void lcd_init();
void lcd_clear();

#endif