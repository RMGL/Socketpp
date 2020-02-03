#ifndef SOCKET_STREAM_H
#define SOCKET_STREAM_H

#include "Socketpp.hpp"

namespace Socketpp{

    class Socket;

    class SocketStream{
        private:
            bool closed;
        protected:
            void ensureOpen();
            Socket* socket;
            // Socket* getSocket();
        public:
            SocketStream(Socket* sock): closed(false), socket(sock){}
            ~SocketStream(){}
            void close();
            bool isClosed();
    };

    class SocketInputStream: public SocketStream{
        private:
            bool stopCondition(char delimitor, char c);

        public:
            SocketInputStream(Socket* sock): SocketStream(sock){}
            ~SocketInputStream(){}
            std::size_t	available();
            std::size_t	read(char* c);
            std::size_t	read(char* buffer, std::size_t size);
            std::size_t	read(char* buffer, std::size_t size, std::size_t offset, std::size_t length);
            std::size_t readUntil(char* buffer, std::size_t size, char delimitor);
            std::string readString();
            
            std::size_t discard(long n);

            std::vector<char> operator >> (std::vector<char>& v);
            std::string operator >> (std::string& s);
    };

    class SocketOutputStream: public SocketStream{
        public:
            SocketOutputStream(Socket* sock): SocketStream(sock){}
            ~SocketOutputStream(){}

            std::size_t write(const char* buffer, std::size_t size);
            std::size_t write(const char* buffer, std::size_t size, std::size_t offset, std::size_t length);
            std::size_t write(char c);

            std::size_t write(const std::vector<char> v);
            std::size_t writeString(std::string s);

            std::size_t operator << (const std::vector<char>& v);
            std::size_t operator << (const std::string& s);
    };

}

#endif
