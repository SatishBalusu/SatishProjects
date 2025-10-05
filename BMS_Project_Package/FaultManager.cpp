#include "FaultManager.h"

void FaultManager::storeData(BMS &bms) {
    static float lastSOC = -1;
    static float lastSOH = -1;

    if(abs(bms.SOC - lastSOC) >= 0.1) {
        EEPROM.put(SOC_ADDR, bms.SOC);
        lastSOC = bms.SOC;
    }
    if(abs(bms.SOH - lastSOH) >= 0.1) {
        EEPROM.put(SOH_ADDR, bms.SOH);
        lastSOH = bms.SOH;
    }

    EEPROM.put(CYCLE_ADDR, bms.cycle_count);
}
