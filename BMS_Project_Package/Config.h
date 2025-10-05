#ifndef CONFIG_H
#define CONFIG_H

// --- Pins ---
#define SWITCH_PIN  2
#define RED_LED     8
#define BLUE_LED    9
#define GREEN_LED   10

// --- HV Relay Pin (optional if using relay) ---
#define HV_RELAY_PIN 7

// --- BMS Thresholds ---
#define V_MAX 50.0
#define V_MIN 10.0
#define T_MAX 60.0
#define T_MIN 0.0
#define I_MAX 100.0

// --- EEPROM Addresses ---
#define SOC_ADDR       0
#define SOH_ADDR       4
#define CYCLE_ADDR     8
#define INIT_FLAG_ADDR 20
#define FAULT_LOG_ADDR 100
#define INIT_FLAG 0xA5

#endif
