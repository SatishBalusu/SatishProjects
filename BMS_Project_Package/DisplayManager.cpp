#include "DisplayManager.h"

void DisplayManager::init() {
    lcd.begin();       // Fixed: no arguments required
    lcd.backlight();
}

void DisplayManager::showStatus(BMS &bms) {
    lcd.setCursor(0, 0);
    lcd.print("SOC:");
    lcd.print(bms.SOC, 1);
    lcd.print("% ");

    String stateName = (bms.currentState==WAIT)?"WAIT":(bms.currentState==RUN)?"RUN":"FAULT";
    lcd.setCursor(9, 0);
    lcd.print(stateName);

    if(bms.faultMessage.length() > 0) {
        int displayWidth = 16;
        unsigned long currentMillis = millis();
        if(currentMillis - previousScroll >= scrollInterval) {
            previousScroll = currentMillis;
            scrollIndex++;
            if(scrollIndex > bms.faultMessage.length()) scrollIndex = 0;
        }
        String toDisplay = bms.faultMessage.substring(scrollIndex);
        if(toDisplay.length() < displayWidth) {
            toDisplay += bms.faultMessage.substring(0, displayWidth - toDisplay.length());
        }
        lcd.setCursor(0,1);
        lcd.print(toDisplay.substring(0, displayWidth));
    } else {
        lcd.setCursor(0,1);
        lcd.print("I:");
        lcd.print(bms.I_pack, 1);
        lcd.print("A SOH:");
        lcd.print(bms.SOH,0);
        lcd.print("% ");
    }
}
