#include "stdio.h"
#include "../inc/app_pomodoro.h"
#include "../inc/lcd.h"
#include "../inc/alert.h"
#include "../inc/keys.h"

struct AppInterface_t app_pomodoro_interface;

void app_pomodoro_draw();

enum PomodoroStatus_t{
    POMODORO_STATUS_INIT,
    POMODORO_STATUS_START_WORK,
    POMODORO_STATUS_RUN_FINISH_WORK,
    POMODORO_STATUS_START_BREAK,
    POMODORO_STATUS_RUN_FINISH_BREAK,
    POMODORO_STATUS_START_REST,
    POMODORO_STATUS_RUN_FINISH_REST,
    POMODORO_STATUS_CONFIG = 255
};

struct Pomodoro_t{
    enum PomodoroStatus_t status;
    uint32_t work_timer;
    uint32_t break_timer;
    uint32_t rest_timer;
    uint32_t wait_timer;

    uint8_t work_counter;
    char count_down_string[16];

};

struct Pomodoro_t pomodoro;
static struct repeating_timer seconds_timer;

bool pomodoro_tick(struct repeating_timer *t){
    if(pomodoro.wait_timer != 0){
        pomodoro.wait_timer--;
        app_pomodoro_draw();
        return true;
    }else{
        alert_blink_hz(4);
        return false;
    }
}

void app_pomodoro_create(){
    pomodoro.work_timer = 25;
    pomodoro.break_timer = 5;
    pomodoro.rest_timer = 15;
    pomodoro.wait_timer = 0;
    pomodoro.status = POMODORO_STATUS_INIT;
}

void app_pomodoro_destroy(){
    cancel_repeating_timer(&seconds_timer);
}

void app_pomodoro_update(){
    // keys_update();
    
    switch (pomodoro.status){
    case POMODORO_STATUS_INIT:
        pomodoro.status = POMODORO_STATUS_START_WORK;
        lcd_update_line("Work", 1);
        lcd_update_line("Press to start", 2);
        break;
    case POMODORO_STATUS_START_WORK:
        if(keys_is_pressed(KEY_MID)){
            while(keys_is_pressed(KEY_MID)){
                keys_update();
            }
            pomodoro.wait_timer = pomodoro.work_timer;
            pomodoro.status = POMODORO_STATUS_RUN_FINISH_WORK;
            cancel_repeating_timer(&seconds_timer);
            add_repeating_timer_ms(1000, pomodoro_tick, NULL, &seconds_timer);
        }
        app_pomodoro_draw();
        break;
    case POMODORO_STATUS_RUN_FINISH_WORK:
        if(pomodoro.wait_timer == 0 && keys_is_pressed(KEY_MID)){
            while(keys_is_pressed(KEY_MID)){
                keys_update();
            }
            if(pomodoro.work_counter >= 3){
                pomodoro.status = POMODORO_STATUS_START_REST;
                pomodoro.work_counter = 0;
                alert_set(false);
                break;
            }
            alert_set(false);
            pomodoro.work_counter++;
            pomodoro.status = POMODORO_STATUS_START_BREAK;
        }
        break;
    case POMODORO_STATUS_START_BREAK:
        if(keys_is_pressed(KEY_MID)){
            while(keys_is_pressed(KEY_MID)){
                keys_update();
            }
            pomodoro.wait_timer = pomodoro.break_timer;
            pomodoro.status = POMODORO_STATUS_RUN_FINISH_BREAK;
            cancel_repeating_timer(&seconds_timer);
            add_repeating_timer_ms(1000, pomodoro_tick, NULL, &seconds_timer);
        }
        app_pomodoro_draw();
        break;
    case POMODORO_STATUS_RUN_FINISH_BREAK:
        if(pomodoro.wait_timer == 0 && keys_is_pressed(KEY_MID)){
            while(keys_is_pressed(KEY_MID)){
                keys_update();
            }
            pomodoro.status = POMODORO_STATUS_START_WORK;
            alert_set(false);
        }
        break;
    case POMODORO_STATUS_START_REST:
        if(keys_is_pressed(KEY_MID)){
            while(keys_is_pressed(KEY_MID)){
                keys_update();
            }
            pomodoro.wait_timer = pomodoro.rest_timer;
            pomodoro.status = POMODORO_STATUS_RUN_FINISH_REST;
            cancel_repeating_timer(&seconds_timer);
            add_repeating_timer_ms(1000, pomodoro_tick, NULL, &seconds_timer);
        }
        break;
    case POMODORO_STATUS_RUN_FINISH_REST:
        if(pomodoro.wait_timer == 0 && keys_is_pressed(KEY_MID)){
            pomodoro.status = POMODORO_STATUS_START_WORK;
            alert_set(false);
        }
        break;
    default:
        pomodoro.status = POMODORO_STATUS_INIT;
        break;
    }
    app_pomodoro_draw();
}

void app_pomodoro_draw(){
    switch (pomodoro.status){
    case POMODORO_STATUS_INIT:
        break;
    case POMODORO_STATUS_START_WORK:
        lcd_update_line("Work", 1);
        lcd_update_line("Press to start", 2);
        break;
    case POMODORO_STATUS_RUN_FINISH_WORK:
        lcd_update_line("Work", 1);
        snprintf(pomodoro.count_down_string, 16, "%02dm %02ds", ((uint8_t)pomodoro.wait_timer/60), ((uint8_t)pomodoro.wait_timer%60));
        lcd_update_line(pomodoro.count_down_string, 2);
        break;
    case POMODORO_STATUS_START_BREAK:
        lcd_update_line("Break", 1);
        lcd_update_line("Press to start", 2);
        break;
    case POMODORO_STATUS_RUN_FINISH_BREAK:
        lcd_update_line("Break", 1);
        snprintf(pomodoro.count_down_string, 16, "%02dm %02ds", ((uint8_t)pomodoro.wait_timer/60), ((uint8_t)pomodoro.wait_timer%60));
        lcd_update_line(pomodoro.count_down_string, 2);
        break;
    case POMODORO_STATUS_START_REST:
        lcd_update_line("Rest", 1);
        lcd_update_line("Press to Start", 2);
        break;
    case POMODORO_STATUS_RUN_FINISH_REST:
        lcd_update_line("Rest", 1);
        snprintf(pomodoro.count_down_string, 16, "%02dm %02ds", ((uint8_t)pomodoro.wait_timer/60), ((uint8_t)pomodoro.wait_timer%60));
        lcd_update_line(pomodoro.count_down_string, 2);
        break;
    default:
        break;
    }
    // lcd_update_line("Pomodoro Timer", 1);
    // lcd_update_line("", 2);
}

void app_pomodoro_setup(){
    app_pomodoro_interface.create = app_pomodoro_create;
    app_pomodoro_interface.update = app_pomodoro_update;
    app_pomodoro_interface.destroy = app_pomodoro_destroy;
    app_pomodoro_interface.captured_input = app_pomodoro_captured_input;
}

bool app_pomodoro_captured_input(){
    return false; // for now...
}