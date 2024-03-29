#include <Arduino.h>
#include <ArduinoOTA.h>


#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <PubSubClient.h>
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);

void send_message(String topic_str, String message_str) {
  char message[50];
  char topic_ch[50];
  message_str.toCharArray(message, message_str.length() + 1);
  topic_str.toCharArray(topic_ch, topic_str.length() + 1);
  client.publish(topic_ch, message);
}

void reconnect() {
    while (!client.connected()) {
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);  
      if (client.connect(clientId.c_str(), NULL, NULL, WILLTOPIC, 0, false, "offline", true)) {
        Serial.println("MQTT connected");
        client.publish(WILLTOPIC, "online");
        client.subscribe(RELAYTOPIC);
        client.subscribe(CMDTOPIC);
      } else {
        Serial.println("MQTT NOT connected: ");
        Serial.print(client.state());
      }
    }
}
void subscribeReceive(char* topic, byte* payload, unsigned int length) {
  String message;
  
  // Print the received topic and message to the Serial monitor
  Serial.print("Topic: ");
  Serial.println(topic);
  for (int i = 0; i < length; i ++) {
    message += (char)payload[i];
  }
  Serial.print("Message: ");
  Serial.println(message);
  
  // Check if the received topic is the relay topic
  if (strcmp(topic,RELAYTOPIC)==0) {
    // If the message is "ON", switch the relay ON and publish "Relay switched ON" message
    if (message == "ON")  {
      digitalWrite(RELAY, INVERSED == 0 ? LOW : HIGH);
      send_message(WILLTOPIC, "online");
      send_message(STATUSTOPIC, "Relay switched ON");
    } 
    // If the message is "OFF", switch the relay OFF and publish "Relay switched OFF" message
    if (message == "OFF") { 
      digitalWrite(RELAY, INVERSED == 0 ? HIGH : LOW);
      send_message(WILLTOPIC, "online");
      send_message(STATUSTOPIC, "Relay switched OFF");
    }
  }
  
  // Check if the received topic is the command topic
  if (strcmp(topic,CMDTOPIC)==0) {
    // If the message is "RST", restart the device and publish "Reseting..." message
    if (message == "RST")  {
        send_message(STATUSTOPIC, "Reseting...");
      ESP.restart();
    } 
  }
}


void setup() {
  Serial.println("Booting");
  WiFiManager wifiManager;
  WiFi.hostname(HOSTNAME);
  wifiManager.autoConnect(HOSTNAME);

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(HOSTNAME);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(MQTTSERVER, MQTTPORT);
  client.setCallback(subscribeReceive);
  reconnect();

  pinMode(RELAY,OUTPUT);
  if (INVERSED == 0) digitalWrite(RELAY, HIGH); //default off
  if (INVERSED == 1) digitalWrite(RELAY, LOW); //default off

  
  String ipaddress = WiFi.localIP().toString();
  send_message(STATUSTOPIC, "{\"ip\":"+ ipaddress+"}");
  send_message(STATUSTOPIC, "Setup finished");
}

void loop() {
  ArduinoOTA.handle(); // Handle Over The Air updates
  client.loop(); // Handle MQTT client events

  static unsigned long lastTimepoint = 0;
  unsigned long currentTime = millis();

  // Reconnect to MQTT broker if necessary
  if (WiFi.status() == WL_CONNECTED && !client.connected()) { 
    delay(5000);
    ESP.restart();
  }

  // Publish status message every 5 minutes
  if (currentTime - lastTimepoint > 300000U) {
    lastTimepoint = currentTime;
    client.publish(WILLTOPIC, "online");
    send_message(STATUSTOPIC, "{\"ip\":"+ WiFi.localIP().toString() +"}");
  }
}


