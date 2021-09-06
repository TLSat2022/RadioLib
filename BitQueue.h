//
// Created by szymon on 18/08/2021.
//

#ifndef RECEIEVER_LIB_BITQUEUE_H
#define RECEIEVER_LIB_BITQUEUE_H

template<size_t max_size>
class BitQueue {
public:
    size_t writing_byte{}, writing_bit{}, reading_byte{}, reading_bit{}, fifo_size{};
    uint8_t buffer[max_size]{};

    template<typename T>
    BitQueue& operator >> (T& ref){
        auto* pointer = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&ref));
        for (size_t i = 0; i < sizeof(T); ++i)
            ReadByte(pointer[i]);
        return *this;
    }

    BitQueue& operator << (const int& ref){
        bitWrite(buffer[writing_byte], writing_bit, ref);
        writing_bit = (writing_bit + 1) % 8;
        if(!writing_bit)
            writing_byte = (writing_byte + 1) % max_size;

        fifo_size++;
        return *this;
    }


    BitQueue& operator >> (int& ref){
        ReadBit(ref);
        return *this;
    }

    bool ReadBit(int& ref){
        if(!size())
            return false;

        ref = buffer[reading_byte] & (1 << reading_bit);
        reading_bit = (reading_bit + 1) % 8;
        if(!reading_bit)
            reading_byte = (reading_byte + 1) % max_size;

        fifo_size--;
        return true;
    }

    void ReadByte(uint8_t& ref){
        if(reading_byte == writing_byte) return;
        ref = buffer[reading_byte];
        reading_byte = (reading_byte + 1) % max_size;
        if(fifo_size < 8){
            fifo_size = 0;
            reading_bit = writing_bit;
            reading_byte = writing_byte;
        }
        else
            fifo_size -= 8;
    }

    // This is some kind of command

    size_t size(){
        return fifo_size;
    }

    bool CanReadByte(){
        return fifo_size >= 8;
    }

    void clear(){
        reading_byte = 0;
        reading_bit = 0;
        writing_byte = 0;
        writing_bit = 0;
        fifo_size = 0;
    }

};

typedef BitQueue<150> StdBitQueue;


#endif //RECEIEVER_LIB_BITQUEUE_H
