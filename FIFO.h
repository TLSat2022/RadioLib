//
// Created by szymon on 12/08/2021.
//

#ifndef TRANSMITTER_LIB_FIFO_H
#define TRANSMITTER_LIB_FIFO_H

template<size_t max_size>
class FIFO {
    uint8_t buffer[max_size]{};
public:

    size_t writing{}, reading{}, fifo_size{};
    template<typename T>
    FIFO& operator << (const T& ref){
        auto* pointer = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&ref));
        for (size_t i = 0; i < sizeof(T); ++i) {
            buffer[writing] = *(pointer++);
            writing = (writing + 1) % max_size;
            fifo_size++;
        }
        return *this;
    }

    template<typename T>
    FIFO& operator >> (T& ref){
        auto* pointer = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&ref));
        for (size_t i = 0; i < sizeof(T); ++i) {
            if(writing == reading) return *this;
             *(pointer++) = buffer[reading];
            reading = (reading + 1) % max_size;
            fifo_size--;
        }
        return *this;
    }

    uint8_t read_byte(){
        if(writing == reading) return 0;
        uint8_t val = buffer[reading];
        reading = (reading + 1) % max_size;
        fifo_size--;
        return val;
    }

    bool empty(){
        return writing != reading;
    }

    size_t size(){
        return fifo_size;
    }

};

typedef FIFO<256> Frame;


#endif //TRANSMITTER_LIB_FIFO_H
