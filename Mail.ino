#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

const char* ssid = "Ankush"; // WiFi network name
const char* pass = "12345678"; // WiFi password

const char* IFTTT_WEBHOOK_EVENT = "doorLock"; // IFTTT event name
const char* IFTTT_KEY = "djmTxByajgaXmwohDns7rvPK_Nsk553vDqSaO1saWzK"; // IFTTT Maker Webhooks key

const char* server = "maker.ifttt.com"; // IFTTT server address
const int port = 80; // HTTP port

const int buttonPin = 2;  // Define the pin connected to your push button

int lastButtonState = LOW;
int buttonState = LOW;

WiFiClient wifi; // Create a WiFiClient instance
HttpClient client = HttpClient(wifi, server, port); // Create an HttpClient instance for IFTTT

void setup() {
  Serial.begin(9600); // Initialize serial communication
  delay(10);

  pinMode(buttonPin, INPUT_PULLUP);  // Configure the pin for the push button and use the internal pull-up resistor

  connectWiFi(); // Connect to WiFi
}

void loop() {
  buttonState = digitalRead(buttonPin); // Read the state of the push button

  if (buttonState == LOW && lastButtonState == HIGH) {
    sendEmail("Someone at the door!"); // Send an email when the button is pressed
  }

  lastButtonState = buttonState; // Store the current button state for the next iteration
}

void connectWiFi() {
  if (WiFi.begin(ssid, pass) == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("WiFi connection failed");
    while (1); // End the program if WiFi connection fails
  }
}

void sendEmail(const char* comment) {
  Serial.println("Sending email...");
  String url = "/trigger/" + String(IFTTT_WEBHOOK_EVENT) + "/with/key/" + String(IFTTT_KEY);
  url += "&value1=" + String(comment);

  client.get(url); // Send an HTTP GET request to IFTTT

  int statusCode = client.responseStatusCode(); // Get the HTTP response status code
  Serial.println(statusCode);

  delay(1000);  // Debounce the button to prevent multiple presses
}
