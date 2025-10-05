#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <LiquidCrystal_I2C.h>
#include "BMS.h"

class DisplayManager {
public:
    DisplayManager() : lcd(0x27, 16, 2) {}
    void init();
    void showStatus(BMS &bms);

private:
    LiquidCrystal_I2C lcd;
    unsigned long previousScroll = 0;
    const long scrollInterval = 300;
    int scrollIndex = 0;
};

#endif
