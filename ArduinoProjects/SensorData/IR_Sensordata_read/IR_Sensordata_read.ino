#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int sensorPin = A1;        // OUT pin of the sensor connected to A1
const int ledPin = 13;           // Built-in LED on Arduino Uno
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD address (0x27) and size (16x2)

void setup() {
  pinMode(sensorPin, INPUT_PULLUP); // Use internal pull-up resistor for A1
  pinMode(ledPin, OUTPUT);          // LED as output

  lcd.begin();                      // Initialize the LCD
  lcd.backlight();                  // Turn on the LCD backlight

  Serial.begin(9600);               // Start serial communication
  lcd.setCursor(0, 0);
  lcd.print("IR Sensor Test");      // Display startup message
}

void loop() {
  int sensorValue = digitalRead(sensorPin); // Read sensor output

  // Display sensor value on Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // Display sensor status on LCD
  lcd.clear(); // Clear the LCD for new data
  if (sensorValue == LOW) { // Object detected
    lcd.setCursor(0, 0);    // Set cursor to the first line
    lcd.print("Object Detected");
    digitalWrite(ledPin, HIGH); // Turn on the LED
  } else { // No object detected
    lcd.setCursor(0, 0);
    lcd.print("No Object");
    digitalWrite(ledPin, LOW); // Turn off the LED
  }

  delay(500); // Wait 500ms for stability
}
