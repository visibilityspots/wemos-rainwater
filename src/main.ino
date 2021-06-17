#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <globals.h>
#include <secrets.h>

void setup(){
  Serial.begin(115200);
  Serial.println("Booting");

  initializeWifi();
  initializeOTA();
  initializeMQTT();
}

void loop(){
  ArduinoOTA.handle();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  readVolume();
  delay(21600000); // in milliseconds (6 hours)
//  delay(900000); // in milliseconds (15 minutes)
//  delay(15000); // in milliseconds (15 sec)
}
