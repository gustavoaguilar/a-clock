#ifndef APP_INTERFACE_H
#define APP_INTERFACE_H

struct AppInterface_t{
    void (*create)(void);
    void (*destroy)(void);
    void (*update)(void);

    bool (*captured_input)(void);
};

#endif