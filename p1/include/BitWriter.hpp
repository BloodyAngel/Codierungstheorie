#include <iostream>


template<unsigned int _VALUE_>
struct Log2{
    static const int value = (_VALUE_ ==      1) ? 1  :
                             (_VALUE_ ==      2) ? 2  :
                             (_VALUE_ ==      8) ? 3  :
                             (_VALUE_ ==     16) ? 4  :
                             (_VALUE_ ==     32) ? 5  :
                             (_VALUE_ ==     64) ? 6  :
                             (_VALUE_ ==    128) ? 7  :
                             (_VALUE_ ==    256) ? 8  :
                             (_VALUE_ ==    512) ? 9  :
                             (_VALUE_ ==   1024) ? 10 :
                             (_VALUE_ ==   2048) ? 11 :
                             (_VALUE_ ==   4096) ? 12 :
                             (_VALUE_ ==   8192) ? 13 :
                             (_VALUE_ ==  16384) ? 14 :
                             (_VALUE_ ==  32768) ? 15 :
                             (_VALUE_ ==  65536) ? 16 :
                             (_VALUE_ == 131072) ? 17 :
                             (_VALUE_ == 262144) ? 18 :
                             (_VALUE_ == 524288) ? 19 :
                             (_VALUE_ ==1048576) ? 20 :
                             throw "Vale not supported in Log2 function";
};


template<unsigned int BufferSize_inByte = 1024>
struct BitWriter{
    BitWriter(std::ostream& stream):m_OutputStream(stream){}

    void printToConsole(){
        int counter = 0;
        for (auto byte : this->m_Buffer){
            for (char i = 0; i < 8; ++i){
                if (counter++ == 8){
                    counter = 0;
                    std::cout << std::endl;
                }
                if (byte & (char(1) << i))
                    std::cout << '1';
                else
                    std::cout << '0';
            }
        }
    }

    void flush(){
        // writer number of unused bits
        char count = 0;
        while(this->m_BitPos != 5){
            ++count;
            this->addBit(false);
        }
        for (char i = char(Log2<8>::value) - 1; i >= 0; --i)
            this->addBit( (count & (char(1) << i)) >> i );

        this->_write();
    }
    void addBit(bool bit){
        // clear bit
        this->m_Buffer[this->m_BytePos] &= ~(char(1) << this->m_BitPos);

        // add the new bit
        this->m_Buffer[this->m_BytePos] |= static_cast<unsigned char>(bit) << this->m_BitPos;

        ++this->m_BitPos;
        if (this->m_BitPos == 8){
            this->m_BitPos = 0;
            ++this->m_BytePos;

            if (this->m_BytePos == BufferSize_inByte)
                this->_write();
        }
    }
    template<typename _ContainerType>
    void addBits(const _ContainerType& bitArray){
        // simple solution
        // slower but very easy to implement :D
        for (bool e : bitArray)
            this->addBit(e);
    }

    void addByte(char byte){
        for (char i = 0; i < 8; ++i)
            this->addBit((byte & (char(1) << i)) >> i);
    }
    // container has to be of type "char"
    template<typename _ContainerType>
    void addBytes(const _ContainerType& byteArray){
        for (auto byte : byteArray)
            this->addByte(byte);
    }

    template<typename T>
    void add(const T& data){
        int size = sizeof(T);
        char* d = reinterpret_cast<char*>(&data);
        for (int i = 0; i < size; ++i)
            this->addByte(d[i]);
    }

    unsigned long long getBitCount() const {
        return (unsigned long long)(m_BitPos) + (unsigned long long)(m_BytePos) * (unsigned long long)(8);
    }
    static constexpr unsigned int GetBufferSize() { return BufferSize_inByte; }
private:
    void _write(){
        this->m_OutputStream.write(this->m_Buffer, this->m_BytePos);
        this->m_BitPos = 0;
        this->m_BytePos= 0;
    }

    char m_BitPos = 0;
    unsigned int m_BytePos = 0;

    std::ostream& m_OutputStream;
    char m_Buffer[BufferSize_inByte];
};


