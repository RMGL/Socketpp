#ifndef SOCKET_STREAM_H
#define SOCKET_STREAM_H

#include "Socketpp.h"

namespace socketpp{

    class Socket;

    class SocketStream{
        private:
            bool closed;
        protected:
            void ensureOpen();
            Socket* socket;
        public:

            /**
            *  @brief Base socket stream class, saves logical information about the stream
            *  @param sock pointer to the socket that the stream is binded
            */
            SocketStream(Socket* sock): closed(false), socket(sock){}
            ~SocketStream(){}

            /**
            *  @brief closes the stream
            */
            void close();

            /**
            *  @brief checks if the stream is open
            *  @return true if open, false otherwise
            */
            bool isOpen();
    };

    class SocketInputStream: public SocketStream{
        public:

            /**
            *  @brief Input stream abstraction, TODO thread safe, writes to socket in a blocking way
            *  @param sock pointer to the socket that the stream is binded
            */
            SocketInputStream(Socket* sock): SocketStream(sock){}
            ~SocketInputStream(){}
            
            /**
            *  @brief Gets the number of bytes available to read
            *  @return Number of bytes available to read
            */
            std::size_t	available();

            /**
            *  @brief Reads a single byte to c
            *  @param c Destination char 
            *  @return number of bytes read
            */            
            std::size_t	read(char* c);

            /**
            *  @brief Reads n bytes to buffer
            *  @param buffer Destination char array
            *  @param n number of bytes to read
            *  @return number of bytes read
            */  
            std::size_t	read(char* buffer, std::size_t n);

            /**
            *  @brief Reads n bytes to buffer from offset to offset + n
            *  @param buffer Destination char array
            *  @param size, buffer size
            *  @param offset offset of destination buffer
            *  @param n number of bytes to read
            *  @return number of bytes read 
            */  
            std::size_t	read(char* buffer, std::size_t size, std::size_t offset, std::size_t n);

            /**
            *  @brief Reads n bytes to buffer until delimiter is found or no more bytes are available to read
            *  @param buffer Destination char array
            *  @param n number of bytes to read
            *  @param delimiter stop condition
            *  @return number of bytes read 
            */  
            std::size_t readUntil(char* buffer, std::size_t size, char delimiter);

            
            /**
            *  @brief Reads a string from socket
            *  @return string read
            */            
            std::string readString();

            /**
            *  @brief Reads a char from socket
            *  @return char read 
            */   
            char readChar();
            
            /**
            *  @brief Discars up to n bytes from the socket
            *  @return number of bytes discarded
            */  
            std::size_t discard(long n);

            std::vector<char> operator >> (std::vector<char>& v);
            std::string operator >> (std::string& s);
    };

    class SocketOutputStream: public SocketStream{
        public:
            SocketOutputStream(Socket* sock): SocketStream(sock){}
            ~SocketOutputStream(){}

            std::size_t write(const char* buffer, std::size_t n);

            /**
            * REVIEW
            *  @brief Reads a char from socket
            *  @return char read 
            */  
            std::size_t write(const char* buffer, std::size_t size, std::size_t offset, std::size_t n);
            std::size_t write(char c);

            std::size_t write(const std::vector<char> v);
            std::size_t writeString(std::string s);

            std::size_t operator << (const std::vector<char>& v);
            std::size_t operator << (const std::string& s);
    };

}

#endif