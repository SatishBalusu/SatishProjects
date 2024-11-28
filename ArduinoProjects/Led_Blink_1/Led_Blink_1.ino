// Define the LED pin
const int ledPin = 10;

void setup() {
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Turn the LED on
  digitalWrite(ledPin, HIGH);
  delay(5000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(ledPin, LOW);
  delay(5000); // Wait for 1 second
}
