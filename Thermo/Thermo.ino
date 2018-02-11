#include "DHT.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

#define TX_PIN 4
#define IN_SENSOR 3
#define OUT_SENSOR 2
#define DELAY 60
#define SERIAL_SPEED 115200
#define DHTTYPE DHT22

DHT dhtIn(IN_SENSOR, DHTTYPE);
DHT dhtOut(OUT_SENSOR, DHTTYPE);

void setup() {
    Serial.begin(SERIAL_SPEED);
    
    dhtIn.begin();
    dhtOut.begin();

    mySwitch.enableTransmit(TX_PIN);
}

void printValue(String text, float value) {
    Serial.print(text + " is ");
    Serial.print(value);
}

void loop() {
    delay(DELAY * 1000);
 
    float hIn = dhtIn.readHumidity();
    float hOut = dhtOut.readHumidity();

    float tIn = dhtIn.readTemperature();
    float tOut = dhtOut.readTemperature();

    if (!isnan(hIn)) {
        printValue("Inside humidity", hIn);
        
        sendStuff(1, hIn);
    }
    if (!isnan(tIn)) {
        printValue("Inside temperature", tIn);
        
        sendStuff(2, tIn);
    }
    
    if (!isnan(hOut)) {
        printValue("Outside humidity", hOut);

        sendStuff(3, hOut);
    }
    if (!isnan(tOut)) {
        printValue("Outside temperature", tOut);;
        
        sendStuff(4, tOut);
    }
}

int hashIt(float toHash) {
    int theInt = toHash * 100;
    int hash = 0;
    while (theInt > 0) {
        hash += theInt % 10;
        theInt /= 10;
    }
    
    return hash % 10;
}
    
void sendStuff(int code, float value) {
    int hsh = hashIt(value + code);
    
    if (value <= 0) {
        value = -value;
        value += 100;
    }
    long toSend = value * 10000 + code * 10 + hsh;
    
    mySwitch.send(toSend, 24);
    Serial.print(" ... ");
    Serial.print(toSend);
    Serial.print(" sent");
    delay(1000);
}

