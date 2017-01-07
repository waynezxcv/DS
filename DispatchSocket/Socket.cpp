/*
 Copyright (c) 2016 waynezxcv <liuweiself@126.com>

 https://github.com/waynezxcv/ELoop

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


#include "Socket.hpp"



using namespace DispatchSocket;

void Socket::sockGetSockName(const int& fd, std::string &ip,uint16_t &port) const{
    int err;
    int success;
    char str[128];
    struct sockaddr sockaddr;
    AddressHelper::getSockaddrStruct(ip, port, &sockaddr);

    if (AddressHelper::isIPv4Addr(&sockaddr)) {
        struct sockaddr_in addr_in;
        socklen_t addrLen = sizeof(addr_in);
        err = getsockname(fd,  (struct sockaddr *) &addr_in, &addrLen);
        success = (err == 0);
        if (success) {
            if (addrLen == sizeof(addr_in)) {
                port = ntohs(addr_in.sin_port);
                inet_ntop(AF_INET, &addr_in.sin_addr, str, sizeof(str));
                ip = std::string(str);
            }
        }

    } else if (AddressHelper::isIPv6Addr(&sockaddr)) {
        struct sockaddr_in6 addr_in6;
        socklen_t addrLen = sizeof(addr_in6);
        err = getsockname(fd,  (struct sockaddr *) &addr_in6, &addrLen);
        success = (err == 0);
        if (success) {
            if (addrLen == sizeof(addr_in6)) {
                port = ntohs(addr_in6.sin6_port);
                inet_ntop(AF_INET6, &addr_in6.sin6_addr, str,  sizeof(str));
                ip = std::string(str);
            }
        }
    }
}


void Socket::sockGetPeerName(const int& fd, std::string &ip,uint16_t &port) const {
    int err;
    int success;
    char str[128];
    struct sockaddr sockaddr;
    AddressHelper::getSockaddrStruct(ip, port, &sockaddr);

    if (AddressHelper::isIPv4Addr(&sockaddr)) {
        struct sockaddr_in addr_in;
        socklen_t addrLen = sizeof(addr_in);
        err = getpeername(fd,  (struct sockaddr *) &addr_in, &addrLen);
        success = (err == 0);
        if (success) {
            if (addrLen == sizeof(addr_in)) {
                port = ntohs(addr_in.sin_port);
                inet_ntop(AF_INET, &addr_in.sin_addr, str, sizeof(str));
                ip = std::string(str);
            }
        }
    } else if (AddressHelper::isIPv6Addr(&sockaddr)) {
        struct sockaddr_in6 addr_in6;
        socklen_t addrLen = sizeof(addr_in6);
        err = getpeername(fd,  (struct sockaddr *) &addr_in6, &addrLen);
        success = (err == 0);
        if (success) {
            if (addrLen == sizeof(addr_in6)) {
                port = ntohs(addr_in6.sin6_port);
                inet_ntop(AF_INET6, &addr_in6.sin6_addr, str, sizeof(str));
                ip = std::string(str);
            }
        }
    }
}

std::string Socket::sockGetIfaddrs() const {
    std::string address = "error";
    struct ifaddrs* interfaces = NULL;
    struct ifaddrs* temlw_addr = NULL;
    int success = 0;
    success = getifaddrs(&interfaces);
    if (success == 0) {
        temlw_addr = interfaces;
        while (temlw_addr != NULL) {
            if(temlw_addr->ifa_addr->sa_family == AF_INET) {
                char* ifa = temlw_addr->ifa_name;
                if (!strcmp(ifa, "en0")) {
                    char str[128];
                    sockaddr_in* addr_in;
                    addr_in = (sockaddr_in *)temlw_addr->ifa_addr;
                    inet_ntop(AF_INET,&addr_in->sin_addr, str, sizeof(str));
                    address = std::string(str);

                }
            } else if (temlw_addr->ifa_addr->sa_family == AF_INET6) {
                char* ifa = temlw_addr->ifa_name;
                if (!strcmp(ifa, "en0")) {
                    char str[128];
                    sockaddr_in6* addr_in6;
                    addr_in6 = (sockaddr_in6*)temlw_addr->ifa_addr;
                    inet_ntop(AF_INET6,&addr_in6->sin6_addr, str, sizeof(str));
                    address = std::string(str);
                }
            }
            temlw_addr = temlw_addr->ifa_next;
        }
    }
    freeifaddrs(interfaces);
    return address;
}


int Socket::setNonBlock(const int& fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return flags;
    }
    int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return ret;
}


