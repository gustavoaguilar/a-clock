#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdio.h>

#include "../inc/app_interface.h"
#include "../inc/ds3231.h"
#include "../inc/alert.h"
#include "../inc/keys.h"
#include "../inc/lcd.h"
#include "../inc/dialog.h"
#include "../inc/app_clock.h"

enum Screens_enum {
    SCREEN_BLANK = 0,
    SCREEN_CLOCK,
    SCREEN_TEST,
};

enum AppList_enum{
    APP_CLOCK,
    APP_TEST,
    APP_LIST_SIZE,
};

struct AppInterface_t app_list[APP_LIST_SIZE];
enum AppList_enum current_app;
enum AppList_enum last_app;

void create_apps(){
    app_clock_setup();
    app_list[APP_CLOCK] = app_clock_interface;

    current_app = APP_CLOCK;
    app_list[current_app].create();
    last_app = current_app;
}

void init_mcu(){
    stdio_init_all();
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    uart_puts(uart0, " Hello, UART!\r\n");

    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(DS3231_SDA, GPIO_FUNC_I2C);
    gpio_set_function(DS3231_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DS3231_SDA);
    gpio_pull_up(DS3231_SCL);
}

int main(){
    init_mcu();
    lcd_init();
    alert_init();
    keys_init();
    
    alert_blink_hz(1);
    create_apps();

    while (true){
        keys_update();

        if(keys_is_pressed(KEY_LEFT) && !app_list[current_app].captured_input()){
            if(current_app > 0){
                current_app--;
            }
        }

        if(keys_is_pressed(KEY_RIGHT) && !app_list[current_app].captured_input()){
            if(current_app < APP_LIST_SIZE - 1){
                current_app++;
            }
        }
        
        if(current_app != last_app){
            app_list[last_app].destroy();
            app_list[current_app].create();
            last_app = current_app;
        }
        app_list[current_app].update();
    }

    return 0;
}
