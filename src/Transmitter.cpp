//
// Created by szymon on 11/08/2021.
//

#include <../Transmitter.h>
#include <Arduino.h>


Transmitter* Transmitter::instance;

Transmitter* Transmitter::GetInstance(){
    return instance;
}

void setup_timer(){
    cli();

    TCCR2A = 0;// set entire TCCR2A register to 0
    TCCR2B = 0;// same for TCCR2B
    TCNT2  = 0;//initialize counter value to 0
    // set compare match register for 8khz increments
    OCR2A = 124;// = (16*10^6) / (8000*8) - 1 (must be <256)
    // turn on CTC mode
    TCCR2A |= (1 << WGM21);
    // Set CS21 bit for 8 prescaler
    TCCR2B |= (1 << CS22);
    // enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2A);

    sei();

}

ISR(TIMER2_COMPA_vect){
    Transmitter* instance = Transmitter::GetInstance();
    if(instance) instance->isr();
}

Transmitter::Transmitter(int pin, int baud_rate){
    if(!Transmitter::instance){
        Transmitter::instance = this;
        this->pin = pin;
        this->baud_rate = baud_rate;
    }

}

void Transmitter::begin() const {
    if(Transmitter::instance == this){
        pinMode(pin, OUTPUT);
        setup_timer();
    }
}

void Transmitter::send_message() {
    if(!CanSend()) return;
    cli();
    clock = 0;
    frame << start_sequence;
    sending = true;
    sei();
}

bool Transmitter::send_bit(int x) {
    x = !!x;
    digitalWrite(pin, x ^ clock);
    clock = !clock;
    return !clock;
}

void Transmitter::isr() {
    if(sending) {
        if (!clock) {
            // Stuff bit
            if(contiguous_zeros_count >= 5 && frame.reading_byte != 0 && frame.reading_byte != frame.writing_byte - 1){
                current_bit = 1;
                contiguous_zeros_count = 0;
            }
            else {
                if (!frame.ReadBit(current_bit)) {
                    sending = false;
                    frame.clear();
                    contiguous_zeros_count = 0;
                    frame << start_sequence;
                    digitalWrite(pin, 1);
//                    Serial.println();
                    return;
                }
                if(!current_bit) contiguous_zeros_count++;
                else contiguous_zeros_count = 0;
            }
//            Serial.print(!!current_bit);
//            Serial.print(" ");
        }
        send_bit(current_bit);
    }
}

bool Transmitter::CanSend() {
    return !sending;
}
