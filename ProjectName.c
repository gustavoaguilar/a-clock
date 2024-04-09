#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdio.h>

#include "lcd.h"
#include "app_clock.h"

#define PIN_RGB_R 13
#define PIN_RGB_G 14
#define PIN_RGB_B 15
#define RGB_MASK ((1 << PIN_RGB_R) | (1 << PIN_RGB_G) | (1 << PIN_RGB_B))

#define DS3231_SDA 4
#define DS3231_SCL 5

struct repeating_timer timer;

enum Screens_enum {
    SCREEN_BLANK = 0,
    SCREEN_CLOCK,
    SCREEN_TEST,
};

struct System{
    enum Screens_enum status;
}system;

// Testing function
bool foo(struct repeating_timer *t){
    static bool status = false;
    static int counter = 0;

    if (status){
        status = false;
    }else{
        status = true;
    }

    gpio_put(PICO_DEFAULT_LED_PIN, status);
    if(counter == 10){
        counter = 0;
        if(system.status == SCREEN_CLOCK)
            system.status = SCREEN_TEST;
        else
            system.status = SCREEN_CLOCK;
    }
    counter++;
    return true;
}

void init_mcu(){
    stdio_init_all();
    uart_init(uart0, 9600);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    uart_puts(uart0, " Hello, UART!\r\n");

    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(DS3231_SDA, GPIO_FUNC_I2C);
    gpio_set_function(DS3231_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DS3231_SDA);
    gpio_pull_up(DS3231_SCL);

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(PIN_RGB_R);
    gpio_set_dir(PIN_RGB_R, GPIO_OUT);
    gpio_put(PIN_RGB_R, false);

    gpio_init(PIN_RGB_G);
    gpio_set_dir(PIN_RGB_G, GPIO_OUT);
    gpio_put(PIN_RGB_G, false);

    gpio_init(PIN_RGB_B);
    gpio_set_dir(PIN_RGB_B, GPIO_OUT);
    gpio_put(PIN_RGB_B, false);

    gpio_put_masked(RGB_MASK, (RGB_MASK & ~(1<<PIN_RGB_B)));
}

#define BME280_ADDR 0x76
#define BME280_REG_TEMP 0xFA

int bme280_get_reg_value(const uint8_t reg, uint8_t* value){
    int ret = i2c_write_blocking(i2c_default, BME280_ADDR, &reg, 1, true);
    if(ret != 1 || ret == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }

    uint8_t last_value = *value;
    ret = i2c_read_blocking(i2c_default, BME280_ADDR, value, 1, false);
    if(ret != 1 || ret == PICO_ERROR_GENERIC){
        *value = last_value;
        return PICO_ERROR_GENERIC;
    }
}

void bme280_update(){
    uint8_t val = 0;
    bme280_get_reg_value(BME280_REG_TEMP, &val);
    char out[64];
    sprintf(out, "%.2d", val);
    lcd_update_line(out, 2);
}

int main(){
    init_mcu();
    lcd_init();

    add_repeating_timer_ms(500, foo, NULL, &timer);
   
    while (true){
        switch (system.status){
        case SCREEN_CLOCK:
            app_clock_update();
            app_clock_draw();
            break;
        case SCREEN_TEST:
            lcd_update_line("Hello", 1);
            bme280_update();
            break;
        default:
            system.status = SCREEN_CLOCK;
            break;
        }
        sleep_ms(1000);
    }

    return 0;
}
