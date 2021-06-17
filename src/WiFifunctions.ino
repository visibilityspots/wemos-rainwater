#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void initializeWifi(){
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (LittleFS.begin()) {
    Serial.println("mounted file system");
    if (LittleFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = LittleFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        Serial.println("\nparsed from json file:");
        json.printTo(Serial);
        if (json.success()) {

          strcpy(NAME, json["NAME"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  WiFiManager wifiManager;
//  wifiManager.resetSettings();
  wifiManager.setSaveConfigCallback(saveConfigCallback);


  if ( NAME[0] == '\0' ) {
    Serial.print("Name param emtpy set default");
    strcpy(NAME, "SENSOR-NAME");
  }

  WiFiManagerParameter name("name", "SENSOR-NAME",  NAME, 40);
  wifiManager.addParameter(&name);
  Serial.print("Sensor name: ");
  Serial.println(name.getValue());

//  wifiManager.startConfigPortal(NAME);

  if (!wifiManager.autoConnect(NAME, ap_password)) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    //read configuration from FS json
    Serial.println("mounting FS..");
    //clean FS, for testing
    //LittleFS.format();

    if (LittleFS.begin()) {
      Serial.println("mounted file system");
        Serial.println("writing config file");
        File configFile = LittleFS.open("/config.json", "w");
        if (configFile) {
          Serial.println("opened config file");
          Serial.println("saving config");
          DynamicJsonBuffer jsonBuffer;
          JsonObject& json = jsonBuffer.createObject();
          strcpy(NAME, name.getValue());

          json["NAME"] = NAME;

          File configFile = LittleFS.open("/config.json", "w");
          if (!configFile) {
            Serial.println("failed to open config file for writing");
          }

          json.printTo(Serial);
          json.printTo(configFile);
          configFile.close();
          //end save
          LittleFS.end();
        }
    } else {
      Serial.println("failed to mount FS");
    }
  }
  LittleFS.end();
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Sensor name: ");
  Serial.println(name.getValue());

}
