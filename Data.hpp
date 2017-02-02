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



#ifndef Data_hpp
#define Data_hpp

#include <iostream>
#include <vector>


namespace DispatchSocket {
    
    class Data {
        
    public:
        
        Data();
        Data(uint8_t* buffer,const unsigned long& len);
        Data(const Data& rhs);
        Data& operator = (const Data& rhs);
        ~Data();
        
        void writeInt(const int& v);
        void writeUnsignedlong(const unsigned long& v);
        
        int32_t readInt();
        int64_t readUnsignedlong();
        
        void appendBytes(uint8_t* buffer,const unsigned long& len);
        void appendData(const Data& data);
        Data subData(const unsigned long& location,const unsigned long& len);
        void empty();
        
        std::vector<uint8_t> bytes () const;
        unsigned long length () const;
        
    private:
        
        std::vector<uint8_t> _bytes;
        unsigned long _length;
        unsigned long _readPos;
        int32_t read();
        
    };
}

#endif /* Data_hpp */
