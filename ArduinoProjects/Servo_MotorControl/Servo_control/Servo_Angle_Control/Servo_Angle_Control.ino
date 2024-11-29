#include <Servo.h> // Include the Servo library

Servo myServo; // Create a Servo object

void setup() {
  myServo.attach(9); // Attach the servo to pin 9
  Serial.begin(9600); // Start serial communication
  Serial.println("Servo Motor Control Initialized");
  Serial.println("Enter a position (0 to 180):");
}

void loop() {
  if (Serial.available()) { // Check if data is available from Serial Monitor
    String input = Serial.readStringUntil('\n'); // Read the input as a string
    int angle = input.toInt(); // Convert the input to an integer0

    // Validate the input angle
    if (angle >= 0 && angle <= 180) {
      myServo.write(angle); // Set the servo position
      Serial.print("Servo moved to: ");
      Serial.println(angle);
    } else {
      Serial.println("Invalid input. Please enter a value between 0 and 180.");
    }
  }
}
