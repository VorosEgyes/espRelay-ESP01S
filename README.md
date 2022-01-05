# espRelay

EspRelay project is a very simple project to control a relay by MQTT messages.

## Functions

- control a relay with MQTT messages
- OTA firmware update

## Hardware

Because of the OTA update you need ESP01 with 1MB flash (ESP01*S*)!

![ESP01S](https://github.com/VorosEgyes/espRelay/blob/master/docs/esp01.JPG)

In the example I used a 5V usb light. This is my way, you can choose yours. If you don't know how does it work, please don't use it.

![esp02](https://github.com/VorosEgyes/espRelay/blob/master/docs/esp02.jpg)

![esp03](https://github.com/VorosEgyes/espRelay/blob/master/docs/esp03.jpg)

For the first upload you will need an ESP01 programmer, i use this:
![ESP01programmer](https://github.com/VorosEgyes/espRelay/blob/master/docs/ESP01programmer.jpg)

## Software

I use platformio ide.

### config.h

Please edit your config.h according to your data.

## Example

Commands:
- mosquitto_pub -t "espRelay2/cmd" -m "RST"   // Reseting the ESP01
- mosquitto_pub -t "espRelay2/relay" -m "ON"  // Switch Relay ON
- mosquitto_pub -t "espRelay2/relay" -m "OFF" // Switch Relay OFF
