#include <Arduino.h>
#include <AMOOK/Transmitter.h>

Transmitter transmitter(2, 2000);

void setup() {
    Serial.begin(9600);
    transmitter.begin();
}

void loop() {
    if(transmitter.CanSend()) {
        transmitter << "Hello, World!";
        transmitter.send_message();
        delay(1000);
    }
}

