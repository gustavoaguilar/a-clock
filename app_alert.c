#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>

#include "app_alert.h"
#include "lcd.h"

#define LED_PIN 13

static bool led_status = false;
static uint32_t blink_speed = 0;
static struct repeating_timer timer;

void app_alert_init(){
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);
}

bool alert_update(struct repeating_timer *t){
    if(blink_speed != 0){
        led_status = !led_status;
        // if(led_status == false){
        //     led_status = true;
        // } else {
        //     led_status = false;
        // }
        gpio_put(LED_PIN, led_status);
    }
    return true;
}

void alert_blink(){
    cancel_repeating_timer(&timer);
    blink_speed = 500;
    add_repeating_timer_ms(blink_speed, alert_update, NULL, &timer);
}

void alert_blink_hz(uint8_t freq){
    cancel_repeating_timer(&timer);
    blink_speed = 1000 / (freq*2);
    add_repeating_timer_ms(blink_speed, alert_update, NULL, &timer);
}

void alert_set(bool status){
    cancel_repeating_timer(&timer);
    led_status = status;
    blink_speed = 0;
    gpio_put(LED_PIN, led_status);
}