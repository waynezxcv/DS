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



#include "ConnectedClient.hpp"


using namespace DispatchSocket;

void ConnectedClient::setReadSource(const dispatch_source_t& source) {
    if (source) {
        dispatch_retain(source);
        _readSource = source;
    }
}

ConnectedClient::~ConnectedClient() {
    if (_writeSource) {
        dispatch_release(_writeSource);
    }
    if (_readSource) {
        dispatch_release(_readSource);
    }
}

void ConnectedClient::setWriteSource(const dispatch_source_t& source) {
    if (source) {
        dispatch_retain(source);
        _writeSource = source;
    }
}

dispatch_source_t ConnectedClient::getReadSource() const {
    return _readSource;
}

dispatch_source_t ConnectedClient::getWriteSource() const {
    return _writeSource;
}

int ConnectedClient::getFd() const {
    return _connectFd;
}

std::string ConnectedClient::getURL() const {
    return _url;
}
