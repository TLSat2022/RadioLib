//
// Created by szymon on 11/08/2021.
//

#ifndef RECEIEVER_LIB_RECEIVER_H
#define RECEIEVER_LIB_RECEIVER_H

#include <Arduino.h>
#include "BitQueue.h"

typedef unsigned long milli_t;

// It is recommended to use pin 2!
// Check pins which can handle interrupts on your arduino

class Receiver {
public:
    explicit Receiver(int pin = 11, int baud_rate = 2000);

    static Receiver* GetInstance();

    void begin() const;
    int listen_val() const;

    void isr();

    StdBitQueue frame;
    bool beginning = true;
    bool error = false;
    volatile bool received = false;
    uint8_t last_8_bits = 0;

    int bits_read;
private:
    static Receiver* instance;
    int pin;
    int baud_rate;
    bool rejected = false;
    bool prep = true;
    milli_t last_data_read = 0;
    const milli_t dt_e = 1000000 / 2000;

    const uint8_t starting_sequence = 0b10000001;
    int contiguous_zeros_count = 0;
};

#endif //RECEIEVER_LIB_RECEIVER_H
