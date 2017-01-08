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



#ifndef SocketDataPacket_hpp
#define SocketDataPacket_hpp

#include <iostream>


constexpr int kBufferSize = 1024 * 32;//32 bytes


namespace DispatchSocket {
    
    struct SocketDataPacket {
        SocketDataPacket(){
            _data = (char *)malloc(kBufferSize);
            _length = 0;
        };
        
        ~SocketDataPacket() {
            free(_data);
        }
        
        void appendBuffer(char* buffer,const ssize_t& size) {
            char* tmp = (char *)malloc(size + _length);
            memcpy(tmp, _data,_length);
            memcpy(tmp + _length ,buffer,size);
            _length += size;
            free(tmp);
        };
        
        char* getData() const {
            return _data;
        }
        
        char* _data;
        int _length;
    };
}

#endif /* SocketDataPacket_hpp */
