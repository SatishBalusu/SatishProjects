#include <Arduino.h>
#include "BMS.h"
#include "DisplayManager.h"
#include "FaultManager.h"

// --- BMS Objects ---
BMS bms;
DisplayManager display;
FaultManager faultManager;

// --- Pins ---
const int SWITCH_PIN = 2;
const int RED_LED   = 8;
const int BLUE_LED  = 9;
const int GREEN_LED = 10;

// --- EEPROM INIT ---
const int INIT_FLAG_ADDR = 20;
const byte INIT_FLAG = 0xA5;

unsigned long previousLCDUpdate = 0;
const long lcdInterval = 1000;

void setup() {
    Serial.begin(9600);

    pinMode(SWITCH_PIN, INPUT_PULLUP);
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    display.init();

    // --- EEPROM Initialization ---
    if (EEPROM.read(INIT_FLAG_ADDR) != INIT_FLAG) {
        bms.SOC = 0; bms.SOH = 100; bms.cycle_count = 0;
        EEPROM.put(0, bms.SOC);
        EEPROM.put(4, bms.SOH);
        EEPROM.put(8, bms.cycle_count);
        EEPROM.write(INIT_FLAG_ADDR, INIT_FLAG);
    } else {
        EEPROM.get(0, bms.SOC);
        EEPROM.get(4, bms.SOH);
        EEPROM.get(8, bms.cycle_count);
    }

    display.showStatus(bms); // <-- Display initial state
    Serial.println("BMS Ready. Enter 'HV ON'/'HV OFF'/'RESET'/'CYCLES n' or 'V T I'");
}

void loop() {
    // --- Serial Input ---
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        // --- HV ON / OFF ---
        if(input.equalsIgnoreCase("HV ON") && bms.currentState == WAIT) {
            bms.currentState = RUN;
        } 
        else if(input.equalsIgnoreCase("HV OFF") && bms.currentState == RUN) {
            bms.currentState = WAIT;
        }
        // --- RESET FAULT ---
        else if(input.equalsIgnoreCase("RESET") && bms.currentState == FAULTED) {
            bms.faultMessage = "";
            bms.voltageFault = bms.tempFault = bms.currentFault = false;
            bms.currentState = WAIT;
            Serial.println("Fault reset");
        }
        // --- SET CYCLES ---
        else if(input.startsWith("CYCLES")) {
            int spaceIndex = input.indexOf(' ');
            if(spaceIndex > 0) bms.cycle_count = input.substring(spaceIndex+1).toInt();
        }
        // --- Manual Voltage, Temp, Current Input ---
        else {
            int firstSpace = input.indexOf(' ');
            int secondSpace = input.indexOf(' ', firstSpace + 1);
            if(firstSpace > 0 && secondSpace > firstSpace) {
                bms.V_batt = input.substring(0, firstSpace).toFloat();
                bms.T_pack = input.substring(firstSpace + 1, secondSpace).toFloat();
                bms.I_pack = input.substring(secondSpace + 1).toFloat();
            }
        }
    }

    // --- Handle Switch Reset ---
    if(digitalRead(SWITCH_PIN) == LOW && bms.currentState == FAULTED) {
        delay(200); // debounce
        bms.faultMessage = "";
        bms.voltageFault = bms.tempFault = bms.currentFault = false;
        bms.currentState = WAIT;
        Serial.println("Fault reset via switch");
    }

    // --- State Machine Outputs ---
    switch(bms.currentState) {
        case WAIT:
            digitalWrite(RED_LED, LOW);
            digitalWrite(BLUE_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
            break;

        case RUN:
            digitalWrite(RED_LED, LOW);
            digitalWrite(BLUE_LED, LOW);
            digitalWrite(GREEN_LED, HIGH);
            bms.updateBMS();  // <-- Update BMS values and faults
            break;

        case FAULTED:
            digitalWrite(RED_LED, HIGH);
            digitalWrite(BLUE_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            break;
    }

    // --- Update Display ---
    if (millis() - previousLCDUpdate >= lcdInterval) {
        previousLCDUpdate = millis();
        display.showStatus(bms);
        faultManager.storeData(bms);
    }
}
