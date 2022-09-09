#include <ESP8266WiFi.h>  
#include <Servo.h> 
#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         "brisa-2420254"    
#define WIFI_PASS         "bxsrsxhh"
#define APP_KEY           "06eb76e1-d305-4d22-965d-631e0ccb1b55" 
#define APP_SECRET        "945765ff-6919-480f-bf5b-6972ffcd85aa-4f2bd35b-6831-4e2b-9b0d-ab82c702be0b"

#define Lampada_ID       "6319e82d36b44d06d4b6350b"    
#define Lampada_Pin 4    // O pino fisico onde está ligado
#define Porta_ID         "631a408236b44d06d4b693f1"
#define Porta_Pin 5      // O pino fisico onde esta ligado o portao
#define Luzes_ID         "631a630736b44d06d4b6ab75"
#define Luzes_Pin 0      // O pino fisico onde esta ligado as luzes de fora

#define BAUD_RATE         115200               
bool myPowerState = false;

void setupWiFi();
void setupSinricPro();
bool LampadaState(const String &deviceId, bool &state);
bool LuzesState(const String &deviceId, bool &state);
bool PortaState(const String &deviceId, bool &state);
Servo myservo;
// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro(); 
  myservo.attach(2);
 
  pinMode(Lampada_Pin, OUTPUT);
  pinMode(Porta_Pin, OUTPUT);
  pinMode(Luzes_Pin, OUTPUT);
 
}

void loop() {
  SinricPro.handle();
}

bool LampadaState(const String &deviceId, bool &state) {
  Serial.printf("A lampada foi %s\r\n", state?"ligada":"desligada");
  digitalWrite(Lampada_Pin, state);
  return true; 
}
bool LuzesState(const String &deviceId, bool &state) {
  Serial.printf("As luzes foram %s\r\n", state?"ligadas":"desligadas");
  digitalWrite(Luzes_Pin, state);
  return true; 
}
bool  PortaState(const String &deviceId, bool &state) {
  Serial.printf("A porta foi %s\r\n", state?"Aberta":"Fechada");
  digitalWrite(Porta_Pin, state);
  return true; 
}

// setup das conexões Wifi
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Conectando...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("Conectado!\r\n[WiFi]: Endereço de IP e %s\r\n", WiFi.localIP().toString().c_str());
}
// setup das funções para o SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro

  SinricProSwitch &mySwitch2 = SinricPro[Lampada_ID];
  mySwitch2.onPowerState(LampadaState);

  SinricProSwitch &mySwitch1 = SinricPro[Porta_ID];
  mySwitch1.onPowerState(PortaState);

  SinricProSwitch &mySwitch3 = SinricPro[Luzes_ID];
  mySwitch3.onPowerState(LuzesState);

  
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Conectado a nuvem SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Desconectado a nuvem SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}