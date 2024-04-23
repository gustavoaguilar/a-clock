#include <unistd.h>
#include "../inc/keys.h"


struct Button_t{
    bool status;
    bool is_pressed;
    bool is_hold;

    absolute_time_t last_change;
    uint8_t pin;
    bool active_status;
};

static struct Button_t button_left;
static struct Button_t button_right;
static struct Button_t button_mid;

void app_keys_init(){
    gpio_set_dir(KEY_LEFT_PIN, false);
    gpio_set_dir(KEY_RIGHT_PIN, false);
    gpio_set_dir(KEY_MID_PIN, false);

    gpio_pull_up(KEY_LEFT_PIN);
    gpio_pull_up(KEY_RIGHT_PIN);
    gpio_pull_up(KEY_MID_PIN);

    button_left.active_status = false;
    button_right.active_status = false;
    button_mid.active_status = false;

    button_left.pin = KEY_LEFT_PIN;
    button_right.pin = KEY_RIGHT_PIN;
    button_mid.pin = KEY_MID_PIN;
}


void update_button(struct Button_t *b){
    absolute_time_t now = get_absolute_time();
    bool new_status = gpio_get(b->pin);
    
    if(new_status != b->status){
        b->status = new_status;
        b->last_change = now;
        b->is_hold = false;
        b->is_pressed = false;
    }else{
        if(b->status == !b->active_status){
            b->is_hold = false;
            b->is_pressed = false;
            return;
        }

        int64_t status_steady_time = absolute_time_diff_us(b->last_change, now);

        if(status_steady_time > 500000){ // 500ms debounce
            b->is_hold = true;
            b->is_pressed = true;
        }else if(status_steady_time > 10000){ // 10ms debounce
            b->is_pressed = true;
        }
    }
}

void app_keys_update(){
    update_button(&button_left);
    update_button(&button_right);
    update_button(&button_mid);    
}
bool keys_is_pressed(enum Keys k){
    switch (k){
    case KEY_LEFT:
        return button_left.is_pressed;
        break;
    case KEY_RIGHT:
        return button_right.is_pressed;
        break;
    case KEY_MID:
        return button_mid.is_pressed;
        break;
    default:
        break;
    }
}

bool keys_is_released(enum Keys k){
    return !keys_is_pressed(k);
}

bool keys_is_hold(enum Keys k){
    switch (k){
    case KEY_LEFT:
        return button_left.is_hold;
        break;
    case KEY_RIGHT:
        return button_right.is_hold;
        break;
    case KEY_MID:
        return button_mid.is_hold;
        break;
    default:
        break;
    }
}