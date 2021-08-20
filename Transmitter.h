//
// Created by szymon on 11/08/2021.
//

#ifndef TRANSMITTER_LIB_TRANSMITTER_H
#define TRANSMITTER_LIB_TRANSMITTER_H

#include <Arduino.h>
#include "FIFO.h"
#include "BitQueue.h"


class Transmitter {
public:
    explicit Transmitter(int pin = 2, int baud_rate = 2000);

    static Transmitter* GetInstance();
    // Unable to do copy construction
    void operator = (const Transmitter&) = delete;
    Transmitter(const Transmitter&) = delete;

    void begin() const;
    void send_message();

    bool send_bit(int x);
    void isr();

    template<typename T>
    Transmitter& operator << (const T& ref){
        frame << ref;
        return *this;
    }

    template<typename T>
    Transmitter& operator >> (T& ref){
        frame >> ref;
        return *this;
    }

    bool CanSend();

private:
    StdBitQueue frame;
    int pin{};
    int baud_rate{};
    uint8_t current_byte{};

    const uint8_t start_sequence = 0b10000001;
    static Transmitter* instance;
    volatile bool bit_changed{};
    volatile int bit_index{};
    int contiguous_zeros_count = 0;
    int clock = 0;
    volatile bool sending = false;
    int current_bit{};

};


#endif //TRANSMITTER_LIB_TRANSMITTER_H
