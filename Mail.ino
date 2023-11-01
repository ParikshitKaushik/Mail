#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

const char* ssid = "Ankush";
const char* pass = "12345678";

const char* IFTTT_WEBHOOK_EVENT = "doorLock";
const char* IFTTT_KEY = "djmTxByajgaXmwohDns7rvPK_Nsk553vDqSaO1saWzK";

const char* server = "maker.ifttt.com";
const int port = 80;

const int buttonPin = 2;  // Define the pin connected to your push button

int lastButtonState = LOW;
int buttonState = LOW;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(buttonPin, INPUT_PULLUP);  // Use the internal pull-up resistor

  connectWiFi();
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    sendEmail("Someone at the door!");
  }

  lastButtonState = buttonState;
}

void connectWiFi() {
  if (WiFi.begin(ssid, pass) == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("WiFi connection failed");
    while (1);
  }
}

void sendEmail(const char* comment) {
  Serial.println("Sending email...");
  String url = "/trigger/" + String(IFTTT_WEBHOOK_EVENT) + "/with/key/" + String(IFTTT_KEY);
  url += "&value1=" + String(comment);

  client.get(url);

  int statusCode = client.responseStatusCode();
  //Serial.print("Status code: ");
  Serial.println(statusCode);

 // if (statusCode == -3) {
    Serial.println("Email sent successfully!");
 //
  delay(1000);  // Debounce the button
}