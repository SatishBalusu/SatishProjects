#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Wi-Fi credentials
const char* ssid = "1703Callender";        // Your Wi-Fi SSID
const char* password = "bradleyuniversity";  // Your Wi-Fi password

// GPIO pin for the LED
const int ledPin = 2;

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variable to store the LED state
String ledState = "OFF";

void setup() {
  // Initialize serial monitor for debugging
  Serial.begin(115200);

  // Set LED pin as output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

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

  // Serve the HTML page to control the LED
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h1>ESP32 LED Control</h1>";
    html += "<p>Current LED Status: " + ledState + "</p>";
    html += "<button onclick=\"location.href='/on'\">Turn ON</button>";
    html += "<button onclick=\"location.href='/off'\">Turn OFF</button>";
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

  // Start the server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Nothing needed in loop as all work is handled by the web server
}
