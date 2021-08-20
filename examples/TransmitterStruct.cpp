#include <Arduino.h>
#include <AMOOK/Transmitter.h>

Transmitter transmitter(2, 2000); // Pin 2 is recommended

struct Example {
    int e1;
    char e2;
    float e3;
};


void setup() {
    Serial.begin(9600);
    transmitter.begin();
}

void loop() {
    if(transmitter.CanSend()) {
        Example example = { 3, 'x', 4.5f };
        transmitter << example;
        transmitter.send_message();
        delay(1000);
    }
}

