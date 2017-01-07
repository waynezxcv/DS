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


#include "AddressHelper.hpp"


using namespace DispatchSocket;

//输入ip地址和端口号获取通用型地址结构
void AddressHelper::getSockaddrStruct(const std::string& interface,const u_int16_t& port,struct sockaddr* outputAddr) {
    std::string _bufferIp = interface;
    if (_bufferIp == "") {
        _bufferIp = "0.0.0.0";
    }
    char* __ip = (char *)_bufferIp.c_str();
    uint16_t __port = port;
    in_addr  addr4;
    in6_addr addr6;
    //IPv4
    if (inet_pton(AF_INET, __ip, &addr4)) {
        struct sockaddr_in addr_in;
        memset(&addr_in, 0, sizeof(addr_in));
        addr_in.sin_len = sizeof(addr_in);
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(__port);
        if (interface == "") {
            addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
        } else if (interface == "localhost") {
            addr_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        } else {
            addr_in.sin_addr = addr4;
        }
        struct sockaddr* sockaddr = (struct sockaddr *) &addr_in;
        *outputAddr = *sockaddr;
        //IPv6
    } else if (inet_pton(AF_INET6, __ip, &addr6)) {
        struct sockaddr_in6 addr_in6;
        memset(&addr_in6, 0, sizeof(addr_in6));
        addr_in6.sin6_len = sizeof(addr_in6);
        addr_in6.sin6_family = AF_INET6;
        addr_in6.sin6_port = htons(__port);
        if (interface == "") {
            addr_in6.sin6_addr = in6addr_any;
        } else if (interface == "localhost") {
            addr_in6.sin6_addr = in6addr_loopback;
        } else {
            addr_in6.sin6_addr = addr6;
        }
        struct sockaddr* sockaddr = (struct sockaddr *) &addr_in6;
        *outputAddr = *sockaddr;
    }
}

//输入通用型地址结构，获取URL字符串（ip:端口号）
std::string AddressHelper::getUrl(const struct sockaddr* addr) {
    char portStr[8];
    static char str[128];
    if (isIPv4Addr(addr)) {
        struct sockaddr_in* sin = (struct sockaddr_in *)addr;
        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL) {
            return NULL;
        }
        if (ntohs(sin->sin_port) != 0) {
            snprintf(portStr, sizeof(portStr), ":%d",ntohs(sin->sin_port));
            strcat(str, portStr);
        }
        return std::string(str);
    } else if (isIPv6Addr(addr)) {
        struct sockaddr_in6* sin6 = (struct sockaddr_in6 *)addr;
        if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL) {
            return NULL;
        }
        if (ntohs(sin6->sin6_port) != 0) {
            snprintf(portStr, sizeof(portStr), ":%d",ntohs(sin6->sin6_port));
            strcat(str, portStr);
        }
        return std::string(str);
    } else {
        return "0.0.0.0:0";
    }
}

//输入ip地址和端口号获取IPv4型地址结构
void AddressHelper::getSockAddrInStruct(const std::string& interface,const u_int16_t& port,struct sockaddr_in* outputAddr_in) {
    struct sockaddr addr;
    getSockaddrStruct(interface, port, &addr);

    if (isIPv4Addr(&addr)) {
        struct sockaddr_in* sockaddr = (struct sockaddr_in*) &addr;
        *outputAddr_in = *sockaddr;
    }
}

//输入IPv4型地址结构，获取URL字符串（ip:端口号）
std::string AddressHelper::getUrl(const struct sockaddr_in* addr_in) {
    char portStr[8];
    static char str[128];

    if (inet_ntop(AF_INET, &addr_in->sin_addr, str, sizeof(str)) == NULL) {
        return NULL;
    }
    if (ntohs(addr_in->sin_port) != 0) {
        snprintf(portStr, sizeof(portStr), ":%d",ntohs(addr_in->sin_port));
        strcat(str, portStr);
    }
    return std::string(str);
}


//输入ip地址和端口号获取IPv6型地址结构
void AddressHelper::getSockAddrIn6Struct(const std::string& interface,const u_int16_t& port,struct sockaddr_in6* outputAddr_in6) {
    struct sockaddr addr;
    getSockaddrStruct(interface, port, &addr);

    if (isIPv6Addr(&addr)) {
        struct sockaddr_in6* sockaddr = (struct sockaddr_in6*) &addr;
        *outputAddr_in6 = *sockaddr;
    }
}


//输入IPv6型地址结构，获取URL字符串（ip:端口号）
std::string AddressHelper::getUrl(const struct sockaddr_in6* addr_in6) {
    char portStr[8];
    static char str[128];

    if (inet_ntop(AF_INET6, &addr_in6->sin6_addr, str, sizeof(str)) == NULL) {
        return NULL;
    }
    if (ntohs(addr_in6->sin6_port) != 0) {
        snprintf(portStr, sizeof(portStr), ":%d",ntohs(addr_in6->sin6_port));
        strcat(str, portStr);
    }
    return std::string(str);
}


//判断一个地址是否是IPv4型地址
bool AddressHelper::isIPv4Addr(const struct sockaddr* addr) {
    const struct sockaddr _addr = *addr;
    if (_addr.sa_family == AF_INET) {
        return true;
    }
    return false;
}

//判断一个地址是否是IPv6型地址
bool AddressHelper::isIPv6Addr(const struct sockaddr* addr) {
    const struct sockaddr _addr = *addr;
    if (_addr.sa_family == AF_INET6) {
        return true;
    }
    return false;
}


