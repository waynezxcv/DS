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

Data::Data() : _bytes(std::vector<uint8_t>()) , _length (0) {

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

void Data::empty() {
    _bytes.clear();
    _length = 0;
}



