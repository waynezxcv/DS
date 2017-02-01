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


DispatchSocket::WritePacket::WritePacket(const Data& data,const int& type) : type(type) , length(sizeof(unsigned long) + sizeof(int) + data.length()) {
    Data tmp = Data();
    tmp.writeUnsignedlong(length);
    tmp.writeInt(type);
    tmp.appendData(data);
}

DispatchSocket::WritePacket::~WritePacket() {
    
};

DispatchSocket::PacketHeader::PacketHeader(const Data& data) {
    
    unsigned long* expLen = (unsigned long *)malloc(sizeof(unsigned long));
    int* expType = (int *)malloc(sizeof(int));
    
    for (int i = 0; i < sizeof(unsigned long); i ++) {
        printf("%c\n",data.bytes()[i]);
        
        
        memcpy(expLen + i, &data.bytes()[i], sizeof(uint8_t));
    }
    
    for (int i = sizeof(unsigned long); i < sizeof(unsigned long) + sizeof(int); i ++) {
        memcpy(expType + i, &data.bytes()[i], sizeof(uint8_t));
    }
    
    length = *expLen;
    type = *expType;
    
    printf("lenth : %lu\n",length);
    printf("type :%d\n",type);
    
    free(expLen);
    free(expType);
}



DispatchSocket::PacketHeader::~PacketHeader() {
}



