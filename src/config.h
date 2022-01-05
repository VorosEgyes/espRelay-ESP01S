#define HOSTNAME "espRelay2"                //network hostname
#define RELAYTOPIC "espRelay2/relay"        //topic of relay message can be: "ON" or "OFF"
#define CMDTOPIC "espRelay2/cmd"            //command topic: message can be "RST" = reset the ESP
#define WILLTOPIC "espRelay2/availability"  //MQTT LWT topic: "online" or "offline"
#define STATUSTOPIC "espRelay2/status"      //debug topic

#define MQTTPORT 1883                       //MQTT port
#define MQTTSERVER "192.168.1.57"           //MQTT server IP
