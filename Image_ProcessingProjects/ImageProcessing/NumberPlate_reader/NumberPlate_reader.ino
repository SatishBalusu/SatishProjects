#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Use your LCD's I2C address

String plate = "";

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Plate Number:");
}

void loop() {
  if (Serial.available()) {
    plate = Serial.readStringUntil('\n');
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Plate Number:");
    lcd.setCursor(0, 1);
    lcd.print(plate);
  }
}