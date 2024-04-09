#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>

#include "app_clock.h"
#include "lcd.h"
#include "ds3231.h"

struct DS3231_Data ds3231;

char week_day[8][4] = {{'W', 'T', 'F', '\0'},/*Should never happen*/ \
                      {'S', 'u', 'n', '\0'}, \
                      {'M', 'o', 'n', '\0'}, \
                      {'T', 'u', 'e', '\0'}, \
                      {'W', 'e', 'd', '\0'}, \
                      {'T', 'h', 'u', '\0'}, \
                      {'F', 'r', 'i', '\0'}, \
                      {'S', 'a', 't', '\0'}};

void lcd_send_clock(struct DS3231_Data clock){
    char out[33];
    char test1[LCD_CHARS + 1] = "";
    char test2[LCD_CHARS + 1] = "";
 
    snprintf(test1, LCD_CHARS + 1, "    %02d:%02d:%02d", clock.hours, clock.minutes, clock.seconds);
    snprintf(test2, LCD_CHARS + 1, "%03s     %02d/%02d/%02d", week_day[clock.day], clock.date, clock.month, clock.year);

    lcd_update_line(test1, 1);
    lcd_update_line(test2, 2);
}


void app_clock_update(){
    if(ds3231_get_data(&ds3231)){
        puts("Failed to read from sensor");
    }
}

void app_clock_draw(){
    char out[64];
    snprintf(out, 64, "%02d:%02d:%02d %02d/%02d/%02d", ds3231.hours,
            ds3231.minutes, ds3231.seconds, ds3231.date, ds3231.month,
            ds3231.year);
    puts(out);
    lcd_send_clock(ds3231);
}