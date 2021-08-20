#include <Arduino.h>
#include <AMOOK/Receiver.h>

Receiver receiver(2, 2000);

void setup() {

    Serial.begin(9600);
    receiver.begin();
}

// Added comments test

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
        while (receiver.frame.CanReadByte()) {
            uint8_t byte;
            receiver.frame.ReadByte(byte); // Should be 129 - can check to verify
            if(!byte && byte != 129) Serial.write(byte);

        }
        // Reset state of receiver - will be moved to function
        receiver.received = false;
        receiver.bits_read = 0;
        sei(); // Enable interrupts again
    }


}