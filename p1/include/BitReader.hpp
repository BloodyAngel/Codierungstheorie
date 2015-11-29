#include <fstream>
#include <iostream>
#include <vector>

#include <limits.h>
#include <inttypes.h>

unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}



template<unsigned int BufferSize_inByte = 1024>
struct BitReader{
    BitReader(std::istream& stream): m_File(stream) {
        this->_read();
    }

    bool readBit() {
        if (!this->good())
            throw "File already prosessed";

        bool toReturn = (this->m_Buffer[this->m_BytePos] & (char(1) << this->m_BitPos)) >> this->m_BitPos;
        ++this->m_BitPos;
        if (this->m_BitPos == 8){
            this->m_BitPos = 0;
            ++this->m_BytePos;

            if (this->m_BytePos >  this->m_LastBytePos ||
               (this->m_BytePos == this->m_LastBytePos && this->m_BitPos > this->m_LastBitPos))
            {
                if (this->m_File) // no bytes left to prosess
                    this->_read();
            }
        }
        return toReturn;
    }
    std::vector<bool> readBits(int bitCount) {
        // simple implemenation o(^|^)o
        std::vector<bool> toReturn;
        for (int i = 0; i < bitCount; ++i)
            toReturn.push_back(this->readBit());

        return toReturn;
    }
    char readByte() {
        // simple implementation =)
        char toReturn = 0;
        for (char i = 0; i < 8; ++i)
            toReturn |= static_cast<char>(this->readBit()) << i;

        return toReturn;
    }
    std::vector<char> readBytes(int byteCount) {
        // simple implementation :D
        std::vector<char> toReturn;
        for (int i = 0; i < byteCount; ++i)
            toReturn.push_back(this->readByte());

        return toReturn;
    }

    bool good (){
        if (this->m_File.good())
            return true;
        else if (this->m_BytePos < this->m_LastBytePos)
            return true;
        else if (this->m_BytePos == this->m_LastBytePos && this->m_BitPos <= this->m_LastBitPos)
            return true;
        else
            return false;
    }


    void printToConsole(){
        for (int i = 0; i < this->m_LastBytePos; ++i){
            for (char b = 0; b <= 7; ++b){
                if (this->m_Buffer[i] & (char(1) << b))
                    std::cout << '1';
                else
                    std::cout << '0';
            }
            std::cout << std::endl;
        }

        for (char b = 0; b <= this->m_LastBitPos; ++b){
            if (this->m_Buffer[this->m_LastBytePos] & (char(1) << b))
                std::cout << '1';
            else
                std::cout << '0';
        }
        std::cout << std::endl << std::endl;
    }

    template<typename T>
    T read(){
        T toReturn;
        int size = sizeof(T);
        char* data = reinterpret_cast<char*>(&toReturn);
        for (int i = 0; i < size; ++i)
            data[i] = this->readByte();
    }


private:
    void _read(){
        auto len = this->m_File.readsome(this->m_Buffer, BufferSize_inByte + 1);

        if (len != BufferSize_inByte + 1){
            this->m_LastBytePos = len - 1;
            this->m_LastBitPos  = 7;

            // remove unused bits
            char unusedBits = 0;
            for (char i = 7; i >= 5; --i)
                unusedBits |= this->m_Buffer[len - 1] & (char(1) << i);

            unusedBits = reverse(unusedBits);
            unusedBits += 3; // used 3 bits itself

            if (unusedBits >= 8){
                unusedBits -= 8;
                --this->m_LastBytePos;
            }
            this->m_LastBitPos -= unusedBits;

            // change file state to "eof"
            char tmp;
            this->m_File.read(&tmp, 1);

        }
        else{
            // undo last read
            this->m_File.seekg(this->m_File.tellg() - std::istream::pos_type(1)); // undo read
            this->m_LastBytePos = BufferSize_inByte;
            this->m_LastBitPos = 7;
        }
    }

    std::istream& m_File;

    unsigned int m_LastBytePos = -1;
    char m_LastBitPos = -1;

    char m_BitPos = 0;
    unsigned int m_BytePos = 0;
    char m_Buffer[BufferSize_inByte];
};

