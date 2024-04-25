#ifndef DS3231_H
#define DS3231_H

#define DS3231_SDA 4
#define DS3231_SCL 5

enum DS3231_DAY_t{
    SUNDAY = 1,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

struct DS3231_Data{
    int seconds;
    int minutes;
    int hours;

    enum DS3231_DAY_t day;
    int date;
    int month;
    int year;
};

int ds3231_set_data(struct DS3231_Data data);
int ds3231_get_data(struct DS3231_Data* data);

struct DS3231_Data ds3231_generate_data(int hours, int minutes, int seconds,
                                        int day, int date, int month, int year);

#endif