#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdio.h>

#include "../inc/lcd.h"

#define LCD_DATA_MASK ((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) |\
 (1 << LCD_D7))

void lcd_send_command(uint8_t cmd){
    gpio_put(LCD_RS, false);
    gpio_put_masked(LCD_DATA_MASK, ((cmd & 0x0F) << LCD_D4));
    gpio_put(LCD_E, true);
    busy_wait_ms(1);
    gpio_put(LCD_E, false);
    busy_wait_ms(1);
}

void lcd_send_command_4bit(uint8_t cmd){
    gpio_put(LCD_RS, false);
    gpio_put_masked(LCD_DATA_MASK, (((cmd >> 4) & 0x0F) << LCD_D4));
    gpio_put(LCD_E, true);
    busy_wait_ms(1);
    gpio_put(LCD_E, false);
    busy_wait_ms(1);
    gpio_put_masked(LCD_DATA_MASK, ((cmd & 0x0F) << LCD_D4));
    gpio_put(LCD_E, true);
    busy_wait_ms(1);
    gpio_put(LCD_E, false);
    busy_wait_ms(1);
}

void lcd_send_data(uint8_t data){
    gpio_put(LCD_RS, true);
    gpio_put_masked(LCD_DATA_MASK, ((data & 0x0F) << LCD_D4));
    gpio_put(LCD_E, true);
    busy_wait_ms(1);
    gpio_put(LCD_E, false);
    busy_wait_ms(1);
}

void lcd_send_data_4bit(uint8_t data){
    gpio_put(LCD_RS, true);
    
    gpio_put_masked(LCD_DATA_MASK, (((data >> 4) & 0x0F) << LCD_D4));
    gpio_put(LCD_E, true);
    busy_wait_us(500);
    gpio_put(LCD_E, false);
    busy_wait_us(500);
    
    gpio_put_masked(LCD_DATA_MASK, ((data & 0x0F) << LCD_D4));
    gpio_put(LCD_E, true);
    busy_wait_us(500);
    gpio_put(LCD_E, false);
    busy_wait_us(500);
}

void lcd_send_string(char* str, uint32_t len){
    for(uint32_t i = 0; i < len; ++i){
        if(str[i] == '\n'){
            for(uint8_t j = i; j<40; ++j){
                lcd_send_data_4bit(' ');
            }
            continue;
        }
        if(str[i] == '\0') break;

        lcd_send_data_4bit(str[i]);
    }
}

void lcd_update_line(const char* str, const int line){
    bool end = false;

    switch(line){
    default:
    case 1:
        lcd_send_command_4bit(0x80);
        break;
    case 2:
        lcd_send_command_4bit(0xA8);
        break;
    }
    
    for (uint32_t i = 0; i < LCD_CHARS; ++i){
        // Detect the end of the string
        if(str[i] == '\n' || str[i] == '\0'){
            end = true;
        }
        
        // Type char or end the line with empty spaces
        if (end){ 
            lcd_send_data_4bit(' ');
        } else {
            lcd_send_data_4bit(str[i]);
        }
    }
    
}

void lcd_init(){
    gpio_init(LCD_E);
    gpio_init(LCD_RS);
    gpio_init(LCD_D4);
    gpio_init(LCD_D5);
    gpio_init(LCD_D6);
    gpio_init(LCD_D7);

    gpio_set_dir(LCD_E, GPIO_OUT);
    gpio_set_dir(LCD_RS, GPIO_OUT);
    gpio_set_dir(LCD_D4, GPIO_OUT);
    gpio_set_dir(LCD_D5, GPIO_OUT);
    gpio_set_dir(LCD_D6, GPIO_OUT);
    gpio_set_dir(LCD_D7, GPIO_OUT);

    gpio_put(LCD_E, false);
    gpio_put(LCD_RS, false);
    gpio_put(LCD_D4, false);
    gpio_put(LCD_D5, false);
    gpio_put(LCD_D6, false);
    gpio_put(LCD_D7, false);
    
    sleep_ms(15);

    // Config 4bit mode
    lcd_send_command(0x02);
    
    // 2 lines 5x8 char
    lcd_send_command_4bit(0x28);

    //enable display
    lcd_send_command_4bit(0x0F);
    
    // entry mode set (increment, no shift)
    lcd_send_command_4bit(0x06);

    //clear display
    lcd_send_command_4bit(0x01);

    // return to home
    lcd_send_command_4bit(0x02);
}


void lcd_clear(){
    lcd_update_line("", 1);
    lcd_update_line("", 2);
}