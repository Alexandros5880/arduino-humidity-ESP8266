// WIFI , HTTP
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
// Connect to Wifi function
void connectWIFI();
// Send http request function
void sentRequest(float);
ESP8266WiFiMulti WiFiMulti;
const char* ssid = "*********";
const char* password = "**********";


void setup() {
  Serial.begin(115200);
  // Connect to Wifi
  connectWIFI();
  
}


void loop() {
if ( Serial.available() ) { 
    if ( WiFiMulti.run() == WL_CONNECTED ) {
        // Read Data From Serial
        String data = String(Serial.read());
        Serial.println("percent_of_humidity: " + data + " %");
        // HTTP Request
        sentHTTP(data);
      } else {
        // Connect to Wifi
        Serial.println("WIFI Desconnected!");
        connectWIFI();
      }
    // Settings Delay
    int hour = 3.6e+6;
    long day = hour*24;
    long three_times_a_day = day/3;
    long seven_times_a_day = day/7;
    long towenty_four_time_a_day = day/24;
    delay(towenty_four_time_a_day);
  }
}


// Connect to Wifi
void connectWIFI() {
  WiFiMulti.addAP(ssid, password);
  //WiFi.begin(ssid, password);
  while ( WiFiMulti.run() != WL_CONNECTED ) {
    delay(1000);
    Serial.println("Wifi Connecting...");
  }
}


// Send Request
void sentHTTP(String data) {
  HTTPClient http;                                                                                                                   // SHA-1 Fingerprint                          
  http.begin("https://test-5880-developer-edition.eu31.force.com/sensordata/services/apexrest/Arduino_Humanity__c?humanity=" + data, "CA 6B E2 3B A5 B3 54 2D 5A 61 08 5C A7 65 D4 EA 12 B6 64 54");
  // start connection and send HTTP header
  int httpCode = http.GET();
  // httpCode will be negative on error
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.println("[HTTP] GET... code: %d\n"+  String(httpCode));
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}
