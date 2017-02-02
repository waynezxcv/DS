/*
 Copyright (c) 2016 waynezxcv <liuweiself@126.com>
 
 https://github.com/waynezxcv/DispatchSocket
 
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */




#include "Data.hpp"

using namespace DispatchSocket;

Data::Data() : _bytes(std::vector<uint8_t>()) , _length (0) ,_readPos(0) {
    
}

Data::Data(uint8_t* buffer,const unsigned long& len) : _bytes(std::vector<uint8_t>()) , _length (0) {
    for (uint8_t* i = buffer; i < buffer + len; i ++) {
        _bytes.push_back(*i);
        _length ++;
    }
}

Data::~Data(){
    _bytes.clear();
};

Data::Data(const Data& rhs) : _bytes(rhs._bytes), _length(rhs._length) {
    
}

Data& Data::operator = (const Data& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _bytes = rhs._bytes;
    _length = rhs._length;
    return *this;
};

void Data::appendBytes(uint8_t* buffer,const unsigned long& len) {
    for (uint8_t* i = buffer; i < buffer + len; i ++) {
        _bytes.push_back(*i);
        _length ++;
    }
}

void Data::appendData(const Data& data) {
    for (auto c : data._bytes) {
        _bytes.push_back(c);
        _length ++;
    }
}


Data Data::subData(const unsigned long& location,const unsigned long& len) {
    Data sub;
    for (unsigned long i = location; i < location + len ; i ++ ) {
        uint8_t c = _bytes[i];
        sub.appendBytes(&c, 1);
    }
    return sub;
}

std::vector<uint8_t> Data::bytes () const {
    return _bytes;
}

unsigned long Data::length () const {
    return _length;
}

void Data::writeInt(const int& v) {
    size_t len = sizeof(int);
    uint8_t ch[len];
    for (int32_t i = 0; i < len; i ++) {
        ch[i] = ((v >> ((3 - i) * 8)) & 0x0ff);
    }
    this->appendBytes(ch, len);
}

void Data::writeUnsignedlong(const unsigned long& v) {
    size_t len = sizeof(unsigned long);
    uint8_t ch[len];
    for(int32_t i = 0; i < len; i++){
        ch[i] = ((v >> ((7 - i) * 8)) & 0x0ff);
    }
    this->appendBytes(ch, len);
}


int32_t Data::readInt() {
    int32_t ch1 = read();
    int32_t ch2 = read();
    int32_t ch3 = read();
    int32_t ch4 = read();
    return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
}


int64_t Data::readUnsignedlong() {
    int8_t ch[8];
    memcpy(ch, &bytes()[0], 8);
    _readPos = _readPos + 8;
    return (((int64_t)ch[0] << 56) +
            ((int64_t)(ch[1] & 255) << 48) +
            ((int64_t)(ch[2] & 255) << 40) +
            ((int64_t)(ch[3] & 255) << 32) +
            ((int64_t)(ch[4] & 255) << 24) +
            ((ch[5] & 255) << 16) +
            ((ch[6] & 255) <<  8) +
            ((ch[7] & 255) <<  0));
}

int32_t Data::read() {
    int8_t v = bytes()[_readPos];
    _readPos ++;
    return ((int32_t)v & 0x0ff);
}

void Data::empty() {
    _bytes.clear();
    _length = 0;
}



