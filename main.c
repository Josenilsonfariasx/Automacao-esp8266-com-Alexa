#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#define RELAY_PIN_1 12
#define RELAY_PIN_2 14
#else
#include <ESP8266WiFi.h>
#define RELAY_PIN_1 D1
#define RELAY_PIN_2 D2
#endif
#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200

#define WIFI_SSID "LG House"
#define WIFI_PASS "LG19931995"

#define LAMP_1 "LED 1"
#define LAMP_2 "LED 2"

fauxmoESP fauxmo;


// Wi-Fi Conexão
void wifiSetup() {

  // Define o como STA
  WiFi.mode(WIFI_STA);

  // Conecta
  Serial.printf("[WIFI] Conectado ao %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Espera
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Conectado
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  // Inicia a Serial
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  // Conexão Wi-Fi
  wifiSetup();

  // LED
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);

  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_2, LOW);

  
  fauxmo.createServer(true); 
  fauxmo.setPort(80); 

  
  fauxmo.enable(true);


  fauxmo.addDevice(LAMP_1);
  fauxmo.addDevice(LAMP_2);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {

    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, LAMP_1) == 0) ) {
      Serial.println("RELAY 1 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_1, HIGH);
      } else {
        digitalWrite(RELAY_PIN_1, LOW);
      }
    }
    if ( (strcmp(device_name, LAMP_2) == 0) ) {
      Serial.println("RELAY 2 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_2, HIGH);
      } else {
        digitalWrite(RELAY_PIN_2, LOW);
      }
    }
  });

}

void loop() {

  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

}