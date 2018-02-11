
# MQTT-Temperature-and-heating

Code for my thermostat installation.

### I use 2 Arduinos:
#### 1. Internet connected heating control (ESP8266 based) with:
-  Relay *(connected to the central heating)* 
-  433MHz receiver *(used for receiving temperature data)*
#### 2. Thermometer:
- 2 x DHT22 sensors *(one outside and one inside)*
- 433MHz transmiter 

MQTT data is processed in [Home Assistant](https://home-assistant.io/) where a [MQTT climate component](https://home-assistant.io/components/climate.mqtt/) controls the heating.

