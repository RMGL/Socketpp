#ifndef SOCKETPP_H
#define SOCKETPP_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "SocketStream.h"

namespace socketpp{

    class BaseSocket{
        private:
            int fD;
            bool closed;
        protected:

            /**
            *  @brief Base socket class, saves logical information about sockets
            *  @param fileDescriptor socket file descriptor
            */
            BaseSocket(int fileDescriptor): fD(fileDescriptor){}
            ~BaseSocket(){}
        public:

            /**
            *  @brief getter for socket number
            *  @return socket number
            */
            int getSocketNumber(){ return fD; }

            /**
            *  @brief closes the socket
            */
            void close();

            /**
            *  @brief checks if the socket is open
            *  @return true if open, false otherwise
            */
            bool isOpen(){ return closed; }
    };

    class RWSocket: public BaseSocket{
        protected:
            /**
            *  @brief I/o socket class, offers a basic read and write operation for the socket
            *  @param fileDescriptor socket file descriptor
            */
            RWSocket(int fileDescriptor): BaseSocket(fileDescriptor){}
            ~RWSocket(){}
        public:

            /**
            *  @brief Reads n bytes to buffer
            *  @param buffer destination to hold the data read
            *  @param n number of bytes to read
            *  @return number of bytes read
            */
            std::size_t read(char* buffer, std::size_t n);

            /**
            *  @brief Writes n bytes from buffer
            *  @param buffer source of data to write
            *  @param n number of bytes to write
            *  @return number of bytes wrote
            */
            std::size_t write(const char* buffer, std::size_t n);
    };

    class SocketInputStream;
    class SocketOutputStream;

    class Socket: public RWSocket{
        public:

            /**
            *  @brief Client socket abstraction, opens a TCP connection with the host
            *  @param host string containing host address
            *  @param port host port
            */
            Socket(const std::string& host, int port);
            Socket(int fileDescriptor): RWSocket(fileDescriptor){}
            ~Socket(){}
            
            /**
            *  @brief Opens an input stream for the socket
            *  @return InputStream for the socket
            */
            SocketInputStream getInputStream();

            /**
            *  @brief Opens an output stream for the socket
            *  @return OutputStream for the socket
            */
            SocketOutputStream getOutputStream();
    };

    class ServerSocket: public BaseSocket{
        public:
            /**
            *  @brief Server socket abstraction, opens a TCP connection
            *  @param port port where the socket will wait for connections
            */
            ServerSocket(int port);

            /**
            *  @brief Accepts incoming client connections
            *  @return Socket that holds a connection to a single client
            */
            Socket accept();
    };
}

#endif