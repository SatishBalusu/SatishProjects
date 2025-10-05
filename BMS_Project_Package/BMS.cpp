#include "BMS.h"
#include <EEPROM.h>

float BMS::getSOCFromVoltage(float voltage) {
    if(voltage <= voltages[0]) return socValues[0];
    if(voltage >= voltages[numPoints-1]) return socValues[numPoints-1];
    for(int i=0; i<numPoints-1; i++) {
        if(voltage >= voltages[i] && voltage <= voltages[i+1]) {
            return socValues[i] + (voltage - voltages[i]) * 
                   (socValues[i+1] - socValues[i]) / 
                   (voltages[i+1] - voltages[i]);
        }
    }
    return 0;
}

void BMS::updateBMS() {
    SOC = getSOCFromVoltage(V_batt);
    SOH = 100.0 - ((float)cycle_count / 2000.0 * 100.0);
    if(SOH < 0) SOH = 0;

    voltageFault = tempFault = currentFault = false;
    faultMessage = "";

    if(V_batt > V_max) { voltageFault = true; faultMessage += "OVER VOLTAGE! "; }
    else if(V_batt < V_min) { voltageFault = true; faultMessage += "UNDER VOLTAGE! "; }

    if(T_pack > T_max) { tempFault = true; faultMessage += "OVER TEMP! "; }
    else if(T_pack < T_min) { tempFault = true; faultMessage += "UNDER TEMP! "; }

    if(I_pack > I_max) { currentFault = true; faultMessage += "OVER CURRENT! "; }

    if(voltageFault || tempFault || currentFault) currentState = FAULTED;
}
