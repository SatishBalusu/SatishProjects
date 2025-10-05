#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// ==================== LCD SETUP ====================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==================== BMS VARIABLES ====================
float V_batt = 10.0;
float T_pack = 25.0;
float I_pack = 0.0;
float SOC = 0.0;
float SOH = 100.0;
uint16_t cycle_count = 0;

// ==================== THRESHOLDS ====================
const float V_max = 50.0;
const float V_min = 10.0;
const float T_max = 60.0;
const float T_min = 0.0;
const float I_max = 100.0;

// ==================== SOC MAP ====================
float voltages[] = {10, 20, 30, 40, 50};
float socValues[] = {0, 25, 50, 75, 100};
const int numPoints = 5;

// ==================== EEPROM ADDRESSES ====================
const int SOC_ADDR = 0;
const int SOH_ADDR = 4;
const int CYCLE_ADDR = 8;
const int INIT_FLAG_ADDR = 20;
const int FAULT_LOG_ADDR = 100; // Starting EEPROM address for fault log
const byte INIT_FLAG = 0xA5;

// ==================== TIMING ====================
unsigned long previousLCDUpdate = 0;
const long lcdInterval = 1000;
unsigned long previousScroll = 0;
const long scrollInterval = 300;
int scrollIndex = 0;

// ==================== FAULT FLAGS ====================
bool voltageFault = false;
bool tempFault = false;
bool currentFault = false;
String faultMessage = "";

// ==================== STATE MACHINE ====================
enum SystemState {WAIT, RUN, FAULTED};
SystemState currentState = WAIT;

// ==================== PINS ====================
const int switchPin = 2;   // Reset button
const int redLED   = 8;    // FAULTED
const int blueLED  = 9;    // WAIT
const int greenLED = 10;   // RUN

// ==================== FUNCTIONS ====================
float getSOCFromVoltage(float voltage) {
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

void logFault(String faultCode) {
  // Find first empty location in EEPROM (0xFF)
  for(int i = FAULT_LOG_ADDR; i < FAULT_LOG_ADDR + 50; i++){
    if(EEPROM.read(i) == 0xFF){
      EEPROM.write(i, faultCode[0]);
      EEPROM.write(i+1, faultCode[1]);
      break;
    }
  }
}

void updateBMS() {
  SOC = getSOCFromVoltage(V_batt);

  // Update SOH based on cycles
  SOH = 100.0 - ((float)cycle_count / 2000.0 * 100.0);
  if(SOH < 0) SOH = 0;

  voltageFault = tempFault = currentFault = false;
  faultMessage = "";

  // --- Voltage Faults ---
  if(V_batt > V_max) {
    voltageFault = true;
    faultMessage += "OVER VOLTAGE! ";
    logFault("OV");
  } 
  else if(V_batt < V_min) {
    voltageFault = true;
    faultMessage += "UNDER VOLTAGE! ";
    logFault("UV");
  }

  // --- Temperature Faults ---
  if(T_pack > T_max) {
    tempFault = true;
    faultMessage += "OVER TEMP! ";
    logFault("OT");
  } 
  else if(T_pack < T_min) {
    tempFault = true;
    faultMessage += "UNDER TEMP! ";
    logFault("UT");
  }

  // --- Current Fault ---
  if(I_pack > I_max) {
    currentFault = true;
    faultMessage += "OVER CURRENT! ";
    logFault("OC");
  }

  // --- Transition to FAULTED if any fault ---
  if(voltageFault || tempFault || currentFault) {
    currentState = FAULTED;
  }
}

void displaySOCSOH(String stateName) {
  lcd.setCursor(0, 0);
  lcd.print("SOC:");
  lcd.print(SOC, 1);
  lcd.print("% ");

  lcd.setCursor(9, 0);
  lcd.print(stateName);

  if(faultMessage.length() > 0) {
    int displayWidth = 16;
    unsigned long currentMillis = millis();
    if(currentMillis - previousScroll >= scrollInterval) {
      previousScroll = currentMillis;
      scrollIndex++;
      if(scrollIndex > faultMessage.length()) scrollIndex = 0;
    }
    String toDisplay = faultMessage.substring(scrollIndex);
    if(toDisplay.length() < displayWidth) {
      toDisplay += faultMessage.substring(0, displayWidth - toDisplay.length());
    }
    lcd.setCursor(0,1);
    lcd.print(toDisplay.substring(0, displayWidth));
  }
  else {
    lcd.setCursor(0,1);
    lcd.print("I:");
    lcd.print(I_pack, 1);
    lcd.print("A SOH:");
    lcd.print(SOH, 0);
    lcd.print("% ");
  }
}

void storeBMSData() {
  static float lastSOC = -1;
  static float lastSOH = -1;
  if(abs(SOC - lastSOC) >= 0.1) {
    EEPROM.put(SOC_ADDR, SOC);
    lastSOC = SOC;
  }
  if(abs(SOH - lastSOH) >= 0.1) {
    EEPROM.put(SOH_ADDR, SOH);
    lastSOH = SOH;
  }
  EEPROM.put(CYCLE_ADDR, cycle_count);
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  pinMode(switchPin, INPUT_PULLUP);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  if (EEPROM.read(INIT_FLAG_ADDR) != INIT_FLAG) {
    SOC = 0; SOH = 100; cycle_count = 0;
    EEPROM.put(SOC_ADDR, SOC);
    EEPROM.put(SOH_ADDR, SOH);
    EEPROM.put(CYCLE_ADDR, cycle_count);
    EEPROM.write(INIT_FLAG_ADDR, INIT_FLAG);
  } else {
    EEPROM.get(SOC_ADDR, SOC);
    EEPROM.get(SOH_ADDR, SOH);
    EEPROM.get(CYCLE_ADDR, cycle_count);
  }

  lcd.setCursor(0,0);
  lcd.print("BMS Starting...");
  delay(1000);
  lcd.clear();
  Serial.println("Enter Voltage Temp Current OR 'HV ON'/'HV OFF'/'RESET'/'CYCLES n'");
}

// ==================== LOOP ====================
void loop() {
  // --- Handle Serial Input ---
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if(input.equalsIgnoreCase("HV ON") && currentState == WAIT) {
      currentState = RUN;
    } 
    else if(input.equalsIgnoreCase("HV OFF") && currentState == RUN) {
      currentState = WAIT;
    }
    else if(input.equalsIgnoreCase("RESET")) {
      if(currentState == FAULTED) {
        faultMessage = "";
        voltageFault = tempFault = currentFault = false;
        currentState = WAIT;
        Serial.println("Fault reset");
      }
    }
    else if(input.startsWith("CYCLES")) {
      int spaceIndex = input.indexOf(' ');
      if(spaceIndex > 0) {
        cycle_count = input.substring(spaceIndex+1).toInt();
        Serial.print("Cycle count set to: "); Serial.println(cycle_count);
      }
    }
    else {
      int firstSpace = input.indexOf(' ');
      int secondSpace = input.indexOf(' ', firstSpace + 1);
      if(firstSpace > 0 && secondSpace > firstSpace) {
        V_batt = input.substring(0, firstSpace).toFloat();
        T_pack = input.substring(firstSpace + 1, secondSpace).toFloat();
        I_pack = input.substring(secondSpace + 1).toFloat();
      }
    }
  }

  // --- Handle Reset Switch ---
  if(digitalRead(switchPin) == LOW && currentState == FAULTED) {
    delay(200); // debounce
    faultMessage = "";
    voltageFault = tempFault = currentFault = false;
    currentState = WAIT;
    Serial.println("Fault reset via switch");
  }

  // --- State Machine Outputs ---
  switch(currentState) {
    case WAIT:
      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, HIGH);
      digitalWrite(greenLED, LOW);
      break;

    case RUN:
      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, HIGH);
      updateBMS();
      break;

    case FAULTED:
      digitalWrite(redLED, HIGH);
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, LOW);
      break;
  }

  // --- LCD Update ---
  if (millis() - previousLCDUpdate >= lcdInterval) {
    previousLCDUpdate = millis();
    String stateName = (currentState==WAIT)?"WAIT":(currentState==RUN)?"RUN":"FAULT";
    displaySOCSOH(stateName);
    storeBMSData();
  }
}
