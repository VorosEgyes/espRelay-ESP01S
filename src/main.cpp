#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <WebSerial.h>
//#include <ArduinoJson.h>

#include <PubSubClient.h>


//!!!!
const char* topic1 = "espRelay1/relay";
const char* cmdtopic = "espRelay1/cmd";
WiFiClient espRelay;
PubSubClient client(espRelay);
const char* willTopic = "espRelay1/availability";
const String clientID = "espRelay1";

const int mqttPort = 1883;
const char* mqtt_server = "192.168.1.57";



//AsyncWebServer server(80);

#ifndef STASSID 
#define STASSID "T-Home_5DBA64"
#define STAPSK  "mnkx4gem"

#endif

const char* ssid = STASSID;
const char* password = STAPSK;

#define RELAY 0 // relay connected to  GPIO0


char uzenet[50];
char celtopic_ch[50];
void uzenj(String celtopic_str, String uzenet_str) {
  uzenet_str.toCharArray(uzenet, uzenet_str.length() + 1);
  celtopic_str.toCharArray(celtopic_ch, celtopic_str.length() + 1);
  client.publish(celtopic_ch, uzenet);
}




void reconnect() {
    while (!client.connected()) {
      
      if (client.connect(clientID.c_str(), NULL, NULL, willTopic, 0, false, "offline", true)) {
        Serial.println("MQTT connected");
        client.publish(willTopic, "online");
        client.subscribe(topic1);
        client.subscribe(cmdtopic);
      } else {
        Serial.println("MQTT NOT connected: ");
        Serial.print(client.state());
      }
    }
}
void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  String message;
  Serial.print("Topic: ");
  Serial.println(topic);
  for (int i = 0; i < length; i ++)
  {
    message += (char)payload[i];
  }
  Serial.print("Message: ");
  Serial.println(message);
  if (strcmp(topic,topic1)==0){
    if (message == "ON")  {
      digitalWrite(RELAY,HIGH);
    } 
    if (message == "OFF") { 
      digitalWrite(RELAY,LOW);
    }
  }
  if (strcmp(topic,cmdtopic)==0){
    if (message == "RST")  {
      ESP.restart();
    } 
  }


}


/*
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}*/

void setup() {
  

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("espRelay1");

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
/*  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin(); // WebSerial is accessible at "<IP Address>/webserial" in browser*/

  client.setServer(mqtt_server, mqttPort);
  client.setCallback(subscribeReceive);
  reconnect();
  
  

  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY, LOW);
//  WebSerial.println("Setup Ready");
  uint32_t chipID = ESP.getChipId(); 
  uzenj("espRelay1/status", "Setup finished" + String(chipID));
}

void loop() {
  ArduinoOTA.handle();
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) { 
        reconnect();
    } else {
      client.loop();
    }
  } else {
    ESP.restart();
  }
//
}