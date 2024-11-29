#include <Servo.h> // Include the Servo library

Servo myServo; // Create a Servo object

void setup() {
  myServo.attach(9); // Attach the servo to pin 9
  Serial.begin(9600); // Start serial communication for debugging
  Serial.println("Servo Motor Control Initialized");
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 1) { // Move from 0° to 180°
    myServo.write(pos); // Set the servo position
    delay(15); // Wait for the servo to reach the position
  }

  delay(1000); // Pause at 180°

  for (int pos = 180; pos >= 0; pos -= 1) { // Move from 180° to 0°
    myServo.write(pos); // Set the servo position
    delay(15); // Wait for the servo to reach the position
  }

  delay(1000); // Pause at 0°
}

