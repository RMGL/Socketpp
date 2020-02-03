#ifndef SOCKETPP_H
#define SOCKETPP_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "SocketStream.hpp"

namespace Socketpp{

    class BaseSocket{
        private:
            int socketNumber;
            bool closed;
        protected:
            BaseSocket(int sNumber): socketNumber(sNumber){}
            ~BaseSocket(){}
        public:
            int getSocketNumber(){ return socketNumber;}
            void close();
            bool isClosed(){ return closed;}
    };

    class RWSocket: public BaseSocket{
        protected:
            RWSocket(int socketNumber): BaseSocket(socketNumber){}
            ~RWSocket(){}
        public:
            std::size_t read(char* buffer, std::size_t size);
            std::size_t write(const char* buffer, std::size_t size);
    };

    class SocketInputStream;
    class SocketOutputStream;

    class Socket: public RWSocket{
        public:
            Socket(const std::string& host, int port);
            Socket(int socketNumber): RWSocket(socketNumber){}
            ~Socket(){}
            
            SocketInputStream getInputStream();
            SocketOutputStream getOutputStream();
    };

    class ServerSocket: public RWSocket{
        public:
            ServerSocket(int port);
            Socket accept();
    };
}

#endif
