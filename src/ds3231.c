#include "../inc/ds3231.h"
#include "hardware/i2c.h"

#define DS3231_ADDR 0x68
#define DS3231_REG_SECONDS 0x00
#define DS3231_REG_MINUTES 0x01
#define DS3231_REG_HOURS 0x02
#define DS3231_REG_DAY 0x03
#define DS3231_REG_DATE 0x04
#define DS3231_REG_MONTH 0x05
#define DS3231_REG_YEAR 0x06

int ds3231_get_reg_value(const uint8_t reg, uint8_t* value){
    int ret = i2c_write_blocking(i2c_default, DS3231_ADDR, &reg, 1, true);
    if(ret != 1 || ret == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }

    uint8_t last_value = *value;
    ret = i2c_read_blocking(i2c_default, DS3231_ADDR, value, 1, false);
    if(ret != 1 || ret == PICO_ERROR_GENERIC){
        *value = last_value;
        return PICO_ERROR_GENERIC;
    }
}

int ds3231_set_reg_value(const uint8_t reg, uint8_t value){
    uint8_t buf[2] = {reg, value};
    int ret = i2c_write_blocking(i2c_default, DS3231_ADDR, buf, 2, true);
    if(ret != 2 || ret == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    return 0;
}

int ds3231_get_data(struct DS3231_Data* data){
    uint8_t reg = 0x00;
    uint8_t value;
    
    if (ds3231_get_reg_value(DS3231_REG_SECONDS, &value) == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    data->seconds = (((value & 0x70) >> 4)*10) + (value & 0x0F);

    if (ds3231_get_reg_value(DS3231_REG_MINUTES, &value) == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    data->minutes = (((value & 0x70) >> 4)*10) + (value & 0x0F);

    if (ds3231_get_reg_value(DS3231_REG_HOURS, &value) == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    data->hours = (((value & 0x30) >> 4) * 10) + (value & 0x0F);

    if (ds3231_get_reg_value(DS3231_REG_DAY, &value) == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    data->day = (enum DS3231_DAY_t) (value & 0x07);

    if (ds3231_get_reg_value(DS3231_REG_DATE, &value) == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    data->date = (((value & 0x30) >> 4) * 10) + (value & 0x0F);

    if (ds3231_get_reg_value(DS3231_REG_MONTH, &value) == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    data->month = (((value & 0x10) >> 4) * 10) + (value & 0x0F);

    if (ds3231_get_reg_value(DS3231_REG_YEAR, &value) == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    data->year = (((value & 0xF0) >> 4) * 10) + (value & 0x0F);

    return 0;
}

/*
 * Used to configure the ds3231 clock. The user should populate an DS3231_Data
 * struct and provide it to the function
 */
int ds3231_set_data(struct DS3231_Data data){
    int ret = 0;

    uint8_t val = ((data.year/10) << 4) | data.year%10;
    ret = ds3231_set_reg_value(DS3231_REG_YEAR, val);
    if(ret != 0) return PICO_ERROR_GENERIC;
    
    val = ((data.month/10) << 4) | data.month%10;
    ret = ds3231_set_reg_value(DS3231_REG_MONTH, val);
    if(ret != 0) return PICO_ERROR_GENERIC;

    val = ((data.date/10) << 4) | data.date%10;
    ret = ds3231_set_reg_value(DS3231_REG_DATE, val);
    if(ret != 0) return PICO_ERROR_GENERIC;

    val = data.day;
    ret = ds3231_set_reg_value(DS3231_REG_DAY, val);
    if(ret != 0) return PICO_ERROR_GENERIC;

    val = ((data.hours/10) << 4) | data.hours%10;
    ret = ds3231_set_reg_value(DS3231_REG_HOURS, val);
    if(ret != 0) return PICO_ERROR_GENERIC;

    val = ((data.minutes/10) << 4) | data.minutes%10;
    ret = ds3231_set_reg_value(DS3231_REG_MINUTES, val);
    if(ret != 0) return PICO_ERROR_GENERIC;

    val = ((data.seconds/10) << 4) | data.seconds%10;
    ret = ds3231_set_reg_value(DS3231_REG_SECONDS, val);
    if(ret != 0) return PICO_ERROR_GENERIC;

    return ret;
}

struct DS3231_Data ds3231_generate_data(int hours, int minutes, int seconds,
                                        int day, int date, int month, int year){
    struct DS3231_Data data;
    data.hours = hours;
    data.minutes = minutes;
    data.seconds = seconds;
    data.day = day;
    data.date = date;
    data.month = month;
    data.year = year;
    
    return data;
}

// void ds3231_force_value() {
//     struct DS3231_Data ds3231 = ds3231_generate_data(22,48,0,4,31,1,24);
//     if(ds3231_set_data(ds3231)){
//         puts("Failed to set time");
//     }
// }