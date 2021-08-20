#include <Arduino.h>
#include <AMOOK/Receiver.h>

Receiver receiver(2, 2000); // Pin 2 is recommended

void setup() {

    Serial.begin(9600);
    receiver.begin();
}

struct Example {
    int e1;
    char e2;
    float e3;
};

int x = 0;

void loop() {
    // Error checker
    if(receiver.error){
        cli(); // Disable interrupts to not receive messages during error writing
        Serial.println("Synchronization problem detected!");
        // Reset receiver state - will be moved to function
        receiver.error = false;
        sei(); // Enable interrupts again
    }

    if(receiver.received){
        cli(); // Disable interrupts to not receive messages during reading
        if (receiver.frame.CanReadByte()) {
            uint8_t byte;
            Example example = {};
            receiver.frame.ReadByte(byte); // Should be 129 - can check to verify
            receiver.frame >> example;
            receiver.frame.ReadByte(byte); // Should be 129 - can check to verify
            Serial.println(example.e3);

        }
        // Reset state of receiver - will be moved to function
        receiver.received = false;
        receiver.bits_read = 0;
        sei(); // Enable interrupts again
    }

}