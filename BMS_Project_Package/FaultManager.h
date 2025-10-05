#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

#include "BMS.h"
#include <EEPROM.h>

class FaultManager {
public:
    void storeData(BMS &bms);

private:
    const int SOC_ADDR = 0;
    const int SOH_ADDR = 4;
    const int CYCLE_ADDR = 8;
    const int FAULT_LOG_ADDR = 100;
};

#endif
