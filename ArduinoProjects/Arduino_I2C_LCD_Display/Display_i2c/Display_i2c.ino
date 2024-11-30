#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16x2 LCD (common address; adjust if different)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize the LCD
  lcd.begin();
  
  // Turn on the backlight
  lcd.backlight();

  // Print the welcome message on the LCD
  lcd.setCursor(0, 0); // Set cursor to column 0, row 0
  lcd.print("Welcome to");
  lcd.setCursor(0, 1); // Set cursor to column 0, row 1
  lcd.print("Arduino Projects");
}

void loop() {
  // Nothing to do here; the message remains displayed
}

