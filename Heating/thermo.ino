// Basic hash function which sums up the digits
// and returns the last digit of the sum
int hashIt(float toHash) {
    int theInt = toHash * 100;
    int hash = 0;
    while (theInt > 0) {
        hash += theInt % 10;
        theInt /= 10;
    }
    
    return hash % 10;
}

long lastEncoded;

void sendTermo(long encoded) {
    if (lastEncoded == encoded) {
        return;
    }
    lastEncoded = encoded;

    char stringValue[128];
    int hash = encoded % 10;
    int code = (encoded % 100) / 10;
    float value = (float)(encoded - (code * 10) - hash) / 10000.0;
    
    if (value >= 100) {
        value -= 100;
        value = -value;
    }

    if (code < 1 || code > 4) {
        sprintf (stringValue, "%d-%d", encoded, code);
        client.publish(codeErrorTopic,  stringValue);
        return; 
    }
    
    if (hash != hashIt(value + code)) {
        sprintf (stringValue, "%d-%d", encoded, hashIt(value + code));
        client.publish(hashErrorTopic,  stringValue);
        return; 
    }
    
    sprintf (stringValue, "%0.2f", value);
    char topic[128];

    switch (code) {
        case 1: sprintf(topic, "%s", insideHumidityTopic); break;
        case 2: sprintf(topic, "%s", insideTemperatureTopic); break;
        case 3: sprintf(topic, "%s", outsideHumidityTopic); break;
        case 4: sprintf(topic, "%s", outsideTemperatureTopic); break;
    }
    
    client.publish(topic, stringValue);
}
