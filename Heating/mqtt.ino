xvoid reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(mqttClientId, mqttUser, mqttPass)) {
            Serial.println("connected");
            client.publish(onlineTopic, "hello world");
            client.subscribe(heatingTopic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" trying again in 5 seconds");
            delay(5000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    // No need to check topic as it's subscribed to only one
    if ((char)payload[0] == '1') {
        digitalWrite(heatingPin, HIGH);
        client.publish(heatingConfirmTopic, "1");
    } else {
        digitalWrite(heatingPin, LOW);
        client.publish(heatingConfirmTopic, "0");
    }
}
