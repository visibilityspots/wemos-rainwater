#include <PubSubClient.h>
#include <WiFiManager.h>

#define mqtt_sensor NAME
#define volume_topic NAME + String("/volume")

char message_buff[100];

long lastMsg = 0;
long lastRecv = 0;
bool debug = true;

float volume;
float oldVolume;

void initializeMQTT(){
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  if ( debug ) {
    Serial.println("Message received =>  topic: " + String(topic));
    Serial.print(" | length: " + String(length,DEC));
  }
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';

  String msgString = String(message_buff);
  if ( debug ) {
    Serial.println("Payload: " + msgString);
  }

  if ( msgString == "ON" ) {
    digitalWrite(D2,HIGH);
  } else {
    digitalWrite(D2,LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT server...");
    if (client.connect(mqtt_sensor, mqtt_user, mqtt_password)) {
      Serial.println("OK");
    } else {
      Serial.print("ERROR:: ");
      Serial.print(client.state());
      Serial.println("Waiting 5 seconds..");
      delay(5000);
    }
  }
}

void readVolume() {
  JSNSR04Tstart();
  volume = calculateVolume();
  Serial.print("Volume = "); Serial.print(String(volume).c_str());Serial.println(" l");

  if ( isnan(volume) ) {
    Serial.println("Failed to calculate volume based on sensor output!");
    return;
  }

  if (volume != oldVolume )
  {
    sendVolume(volume);
    oldVolume = volume;
  } else {
    return;
  }

}

void sendVolume(float volume) {
  client.publish(String(volume_topic).c_str(), String(volume).c_str(), true);
}
