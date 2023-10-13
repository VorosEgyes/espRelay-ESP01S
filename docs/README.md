# espRelay - ESP01S relay controller

EspRelay project is a very simple IOT project to control a relay by MQTT messages.

## Functions

- control a relay with MQTT messages
- OTA (On-The-Air) firmware update

## Hardware

Because of the OTA update you need ESP01 with 1MB flash (ESP01**S**)! In this example I used a 5V usb light. This is my way, you can choose yours. If you don't know how does it work, please don't use it.

![ESP01S](https://github.com/VorosEgyes/espRelay/blob/master/docs/esp01.JPG)

![esp02](https://github.com/VorosEgyes/espRelay/blob/master/docs/esp02.jpg)

![esp03](https://github.com/VorosEgyes/espRelay/blob/master/docs/esp03.jpg)

For the first upload you will need an ESP01 programmer, i use this:

![ESP01programmer](https://github.com/VorosEgyes/espRelay/blob/master/docs/ESP01programmer.jpg)

More info: https://www.diyhobi.com/flash-program-esp-01-using-usb-serial-adapter/

## Software

https://github.com/VorosEgyes/espRelay-ESP01S

### Platformio IDE
Edit your platformio.ini file and make your first upload.

### Arduino IDE
Copy and paste the content of the src\main.cpp into a new Arduino IDE sketch and copy the config.h file in your saved ardunio file. In the Library Manager you have to install:

- ArduinoOta        : for OTA firmware update
- ESP8266WiFi       : for Wifi
- DNSServer.h       : for Wifi
- ESP8266WebServer  : for WifiManager
- WiFiManager       : WifiManager
- PubSubClient      : MQTT

### config.h

Please edit your config.h according to your parameters.

## Wifi

After your first upload search for EspRelayAP wifi network, and in the browser choose your LAN.
Further help: https://github.com/tzapu/WiFiManager#how-it-works

## Example

Commands:
- mosquitto_pub -t "espRelay2/cmd" -m "RST"   // Reseting the ESP01
- mosquitto_pub -t "espRelay2/relay" -m "ON"  // Switch Relay ON
- mosquitto_pub -t "espRelay2/relay" -m "OFF" // Switch Relay OFF
