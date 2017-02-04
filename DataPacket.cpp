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

#include "DataPacket.hpp"


DispatchSocket::WritePacket::WritePacket(const Data& data,const int& type) :
payload(Data()),
type(type),
length(sizeof(unsigned long) + sizeof(int) + data.length()) {
    
    payload.writeUnsignedlong(length);
    payload.writeInt(type);
    payload.appendData(data);
    
}

DispatchSocket::WritePacket::~WritePacket() {
    
}

DispatchSocket::WritePacket::WritePacket(const WritePacket& rhs) :length(rhs.length) ,type(rhs.type) ,payload(Data(rhs.payload)) {
};

DispatchSocket::WritePacket& DispatchSocket::WritePacket::operator = (const WritePacket& rhs) {
    if (this == &rhs) {
        return *this;
    }
    length = rhs.length;
    type = rhs.type;
    payload = Data(rhs.payload);
    return *this;
}


