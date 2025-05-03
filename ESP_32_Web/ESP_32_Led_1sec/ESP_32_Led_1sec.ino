const int ledPin = 2;  // GPIO pin for the LED

void setup() {
  pinMode(ledPin, OUTPUT);  // Set the LED pin as an output
}

void loop() {
  digitalWrite(ledPin, HIGH);  // Turn the LED ON
  delay(1000);                  // Wait for 1 second
  digitalWrite(ledPin, LOW);   // Turn the LED OFF
  delay(1000);                  // Wait for 1 second
}