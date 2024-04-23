#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>

#include "../inc/app_clock.h"
#include "../inc/lcd.h"
#include "../inc/ds3231.h"
#include "../inc/keys.h"
#include "../inc/dialog.h"

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
    // Enter configuration mode
    if(keys_is_hold(KEY_MID)){
        ds3231.year = dialog_get_uint32_range("Config Clock", "Year(2000)", ds3231.year, 0, 99);
        ds3231.month = dialog_get_uint32_range("Config Clock", "Month", ds3231.month, 1, 12);
        ds3231.date = dialog_get_uint32_range("Config Clock", "Day", ds3231.date, 1, 31);
        ds3231.day = dialog_get_uint32_range("Config Clock", "WDay(Sun=1)", ds3231.day, 1, 7);

        ds3231.hours = dialog_get_uint32_range("Config Clock", "Hour(24hrs)", ds3231.hours, 0, 23);
        ds3231.minutes = dialog_get_uint32_range("Config Clock", "Minute", ds3231.minutes, 0, 59);
        ds3231.seconds = 0;
        
        lcd_update_line("Config Clock", 1);
        if(!ds3231_set_data(ds3231)){
            lcd_update_line("Success", 2);
        } else{
            lcd_update_line("Failed", 2);
        }
        busy_wait_ms(1000);
        while(!keys_is_released(KEY_MID)){
            app_keys_update();
        }
    }
}

void app_clock_draw(){
    char out[64];
    snprintf(out, 64, "%02d:%02d:%02d %02d/%02d/%02d", ds3231.hours,
            ds3231.minutes, ds3231.seconds, ds3231.date, ds3231.month,
            ds3231.year);
    //puts(out);
    lcd_send_clock(ds3231);
}