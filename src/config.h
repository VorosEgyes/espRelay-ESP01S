#define INVERSED 1                          // if soldered for other pin
#define RELAY 0                             // relay connected to  GPIO0
#define HOSTNAME "espRelay5"                // network hostname
#define RELAYTOPIC "espRelay5/relay"        // topic of relay message can be: "ON" or "OFF"
#define CMDTOPIC "espRelay5/cmd"            // command topic: message can be "RST" = reset the ESP
#define WILLTOPIC "espRelay5/availability"  // MQTT LWT topic: "online" or "offline"
#define STATUSTOPIC "espRelay5/status"      // debug topic

#define MQTTPORT 1883                       // MQTT port
#define MQTTSERVER "192.168.1.57"           // MQTT server IP
