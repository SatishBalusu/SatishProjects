#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
String input = "";

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Finger Count:");
}

void loop() {
  if (Serial.available()) {
    input = Serial.readStringUntil('\n');
    lcd.setCursor(0, 1);
    lcd.print("                ");  // Clear line
    lcd.setCursor(0, 1);
    lcd.print("Fingers: " + input);
  }
}
