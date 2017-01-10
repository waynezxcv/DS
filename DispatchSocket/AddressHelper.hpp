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

#ifndef AddressHelper_hpp
#define AddressHelper_hpp

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <errno.h>
#include <stdlib.h>

namespace DispatchSocket {
    
    class AddressHelper {
    public:
        //输入ip地址和端口号获取通用型地址结构
        static void getSockaddrStruct(const std::string& interface,const u_int16_t& port,struct sockaddr* outputAddr);
        //输入ip地址和端口号获取IPv4型地址结构
        static void getSockAddrInStruct(const std::string& interface,const u_int16_t& port,struct sockaddr_in* outputAddr_in);
        //输入ip地址和端口号获取IPv6型地址结构
        static void getSockAddrIn6Struct(const std::string& interface,const u_int16_t& port,struct sockaddr_in6* outputAddr_in6);
        //输入IPv4型地址结构，获取URL字符串（ip:端口号）
        static std::string getUrl(const struct sockaddr_in* addr_in);
        //输入IPv6型地址结构，获取URL字符串（ip:端口号）
        static std::string getUrl(const struct sockaddr_in6* addr_in6);
        //输入通用型地址结构，获取URL字符串（ip:端口号）
        static std::string getUrl(const struct sockaddr* addr);
        //判断一个地址是否是IPv4型地址
        static bool isIPv4Addr(const struct sockaddr* addr);
        //判断一个地址是否是IPv6型地址
        static bool isIPv6Addr(const struct sockaddr* addr);
    };
}

#endif /* AddressHelper_hpp */
