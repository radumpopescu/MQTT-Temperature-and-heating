#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <RCSwitch.h>

const int serialSpeed     = 115200;
const int RCPin           = 12;
const int heatingPin      = 13;

const char* ssid          = "[redacted]";
const char* password      = "[redacted]";
const char* mqttServer    = "[redacted]";
const char* mqttPort      = "[redacted]";
const char* mqttUser      = "[redacted]";
const char* mqttPass      = "[redacted]";
const char* mqttClientId  = "[redacted]";

// MQTT Topics:
const char* onlineTopic               = "[redacted]";
const char* pingTopic                 = "[redacted]";
const char* heatingConfirmTopic       = "[redacted]";
const char* heatingTopic              = "[redacted]";
const char* codeErrorTopic            = "[redacted]";
const char* hashErrorTopic            = "[redacted]";
const char* insideHumidityTopic       = "[redacted]";
const char* insideTemperatureTopic    = "[redacted]";
const char* outsideHumidityTopic      = "[redacted]";
const char* outsideTemperatureTopic   = "[redacted]";

WiFiClient espClient;
RCSwitch mySwitch = RCSwitch();
PubSubClient client(espClient);
long lastMsg = 0;

void setup() {
    Serial.begin(serialSpeed);
    mySwitch.enableReceive(RCPin);
    pinMode(heatingPin, OUTPUT);
    
    setup_wifi();
    
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 60 * 1000) {
        lastMsg = now;
        
        client.publish(pingTopic, "ping");
    }

    if (mySwitch.available()) {
        long received = mySwitch.getReceivedValue();
        if (received != 0) {
            sendTermo(received);
        }
        mySwitch.resetAvailable();
    }
}
