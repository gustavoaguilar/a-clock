#include "dialog.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <app_keys.h>

uint32_t dialog_get_uint32(const char* title, const char* content, const uint32_t initial_value){
    return dialog_get_uint32_range(title, content, initial_value, 0, UINT32_MAX);
}

inline uint32_t dialog_get_uint32_range(const char* title, const char* content,
                                 const uint32_t initial_value,
                                 const uint32_t min, const uint32_t max){
    uint32_t ret = initial_value;
    char line[16] = "hi";
        
    if(ret < min)
        ret = min;
    if(ret > max)
        ret = max;

    lcd_update_line(title, 1);
    lcd_update_line("", 2);
    while(!keys_is_released(KEY_MID)){
        app_keys_update();
    }

    do{
        app_keys_update();
        
        if(ret > min && keys_is_pressed(KEY_LEFT)){
            ret--;
            sleep_ms(100);
        }
        if(ret < max && keys_is_pressed(KEY_RIGHT)){
            ret++;
            sleep_ms(100);
        }
        snprintf(line, 16, "%s: %d", content, ret);
        lcd_update_line(line, 2);
    }while(!keys_is_pressed(KEY_MID));

    return ret;
}