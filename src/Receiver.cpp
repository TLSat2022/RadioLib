//
// Created by szymon on 20/08/2021.
//

#include <../Receiver.h>

Receiver* Receiver::instance = nullptr;

void external_isr(){
    auto instance = Receiver::GetInstance();
    if(instance) instance->isr();
}

void Receiver::begin() const {
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), external_isr, CHANGE);
}

int Receiver::listen_val() const {
    return digitalRead(pin);
}

Receiver::Receiver(int pin, int baud_rate){
    if(!instance){
        this->pin = pin;
        this->baud_rate = baud_rate;
        instance = this;
    }
}

Receiver* Receiver::GetInstance() {
    return instance;
}

void Receiver::isr() {
    if(bits_read > 800)
        received = true;
    if(received) return;

    int y = listen_val();
    int val = !y;

    milli_t dt = micros() - last_data_read;
    if((1 * dt_e) / 2 < dt && dt < (3 * dt_e) / 2) {
        // Ignore - just preapred change of value
    }
    else if((3 * dt_e) / 2 < dt && dt < (5 * dt_e) / 2) {
        // Count as correct bit
//        Serial.println(val);
        last_8_bits <<= 1;
        last_8_bits |= val;
//        Serial.print(last_8_bits); Serial.print(" "); Serial.println(contiguous_zeros_count);
        if(contiguous_zeros_count == 5 && val == 1)
            contiguous_zeros_count = 0;
        else {
            frame << val;
            bits_read++;


            if (last_8_bits == starting_sequence) {
//                Serial.println("So i am here right?");
                if (beginning) {
                    beginning = false;
                } else {
                    received = true;
                    beginning = true;
                }
            }

            if (!val) contiguous_zeros_count++;
            else contiguous_zeros_count = 0;
        }

        last_data_read = micros();

    }
    else if(dt > 5 * (dt_e / 2)){
        // Too long
        if(val) {
            frame.clear();
            last_data_read = micros();
            prep = true;
            beginning = true;
            frame << 1;
            last_8_bits = 0x01;
        }
        else error = true;
    }
}
