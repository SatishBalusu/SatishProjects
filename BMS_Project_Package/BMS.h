#ifndef BMS_H
#define BMS_H

#include <Arduino.h>

enum SystemState { WAIT, RUN, FAULTED };

class BMS {
public:
    // BMS Data
    float V_batt = 10.0;
    float T_pack = 25.0;
    float I_pack = 0.0;
    float SOC = 0.0;
    float SOH = 100.0;
    uint16_t cycle_count = 0;

    // State Machine
    SystemState currentState = WAIT;

    // Thresholds
    const float V_max = 50.0;
    const float V_min = 10.0;
    const float T_max = 60.0;
    const float T_min = 0.0;
    const float I_max = 100.0;

    // SOC mapping
    float voltages[5] = {10, 20, 30, 40, 50};
    float socValues[5] = {0, 25, 50, 75, 100};
    const int numPoints = 5;

    // Faults
    bool voltageFault = false;
    bool tempFault = false;
    bool currentFault = false;
    String faultMessage = "";

    // Methods
    float getSOCFromVoltage(float voltage);
    void updateBMS();
};

#endif
