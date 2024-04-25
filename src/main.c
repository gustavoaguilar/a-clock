#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdio.h>

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

struct System{
    enum Screens_enum status;
}system;

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
    app_alert_init();
    app_keys_init();
    alert_blink_hz(1);

    while (true){
        app_keys_update();

        switch (system.status){
        case SCREEN_CLOCK:
            app_clock_update();
            app_clock_draw();
            break;
        case SCREEN_TEST:
            if(dialog_get_uint32("Type a number!", "Number", 0) >= 15){
                system.status = SCREEN_CLOCK;
                puts("switing to clock");
            }
            break;
        default:
            system.status = SCREEN_CLOCK;
            break;
        }
    }

    return 0;
}
