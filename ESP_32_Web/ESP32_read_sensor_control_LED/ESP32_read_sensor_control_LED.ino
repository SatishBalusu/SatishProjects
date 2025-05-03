#include <WiFi.h>
#include <ESPAsyncWebServer.h>
//Connect the sensor vcc to 3 v gnd to gnd and out to pin 4, then connect led to pin 2 and gnd and IP address is http://10.0.0.141/
// Wi-Fi credentials
const char* ssid = "1703Callender";        
const char* password = "bradleyuniversity";  

// GPIO pins
const int ledPin = 2;          // LED pin
const int irSensorPin = 4;     // IR sensor pin

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variable to store the LED state and IR sensor control state
String ledState = "OFF";
bool sensorEnabled = false;   // This variable controls whether the IR sensor is enabled or not

void setup() {
  // Initialize serial monitor for debugging
  Serial.begin(115200);

  // Set LED and IR sensor pin as output/input
  pinMode(ledPin, OUTPUT);
  pinMode(irSensorPin, INPUT);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Serve the HTML page to control the LED and sensor functionality
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h1>ESP32 LED Control</h1>";
    html += "<p>Current LED Status: " + ledState + "</p>";
    html += "<button onclick=\"location.href='/on'\">Turn ON LED</button>";
    html += "<button onclick=\"location.href='/off'\">Turn OFF LED</button>";
    
    // Add buttons to enable/disable IR sensor
    if (sensorEnabled) {
      html += "<p>IR Sensor is Enabled</p>";
      html += "<button onclick=\"location.href='/disableSensor'\">Disable Sensor</button>";
    } else {
      html += "<p>IR Sensor is Disabled</p>";
      html += "<button onclick=\"location.href='/enableSensor'\">Enable Sensor</button>";
    }
    
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Route to turn the LED on
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH); // Turn the LED ON
    ledState = "ON";            // Update the LED state
    request->send(200, "text/html", "<html><body><h1>LED is ON</h1><a href='/'>Go back</a></body></html>");
  });

  // Route to turn the LED off
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);  // Turn the LED OFF
    ledState = "OFF";           // Update the LED state
    request->send(200, "text/html", "<html><body><h1>LED is OFF</h1><a href='/'>Go back</a></body></html>");
  });

  // Route to enable IR sensor functionality
  server.on("/enableSensor", HTTP_GET, [](AsyncWebServerRequest *request){
    sensorEnabled = true;       // Enable sensor functionality
    request->send(200, "text/html", "<html><body><h1>IR Sensor is Enabled</h1><a href='/'>Go back</a></body></html>");
  });

  // Route to disable IR sensor functionality
  server.on("/disableSensor", HTTP_GET, [](AsyncWebServerRequest *request){
    sensorEnabled = false;      // Disable sensor functionality
    request->send(200, "text/html", "<html><body><h1>IR Sensor is Disabled</h1><a href='/'>Go back</a></body></html>");
  });

  // Start the server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // If IR sensor functionality is enabled, check sensor input
  if (sensorEnabled) {
    int irValue = digitalRead(irSensorPin); // Read IR sensor input (HIGH or LOW)
    if (irValue == HIGH) {
      // If the sensor detects an object (signal is HIGH), turn on the LED
      digitalWrite(ledPin, HIGH);
      ledState = "ON";
      Serial.println("Object detected: LED is ON");
    } else {
      // If the sensor does not detect an object (signal is LOW), turn off the LED
      digitalWrite(ledPin, LOW);
      ledState = "OFF";
      Serial.println("No object detected: LED is OFF");
    }
  }

  // The web server handles all the work for the LED and sensor state
}
