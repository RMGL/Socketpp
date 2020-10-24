#include "Socketpp.h"

using namespace socketpp;

void BaseSocket::close() {
    if (closed) {
        throw SocketException("Socket is already closed");
    }

    closed = true;
    fD = -1;
}

std::size_t RWSocket::read(char* buffer, std::size_t size) {
    readLock.lock();

    std::size_t read = recv(getSocketNumber(), buffer, size, 0);

    if (read == static_cast<std::size_t>(-1)) {
        try{
        close();
        } catch (const SocketException& se){
            std::cerr << se.what() << std::endl;
        }
    }
    
    readLock.unlock();
    return read;
}

std::size_t RWSocket::write(const char* buffer, std::size_t size) {
    writeLock.lock();

    std::size_t written = send(getSocketNumber(), buffer, size, 0);

    if (written == static_cast<std::size_t>(-1)) {
        try{
            close();
        } catch (const SocketException& se){
            std::cerr << se.what() << std::endl;
        }
    }

    writeLock.unlock();
    return written;
}

Socket::Socket(const std::string& host, int port) : RWSocket(::socket(PF_INET, SOCK_STREAM, 0)) {
    struct sockaddr_in hostAddress {};
    std::memset(&hostAddress, 0, sizeof hostAddress);

    hostAddress.sin_family = AF_INET;
    hostAddress.sin_port = htons(port);
    hostAddress.sin_addr.s_addr = inet_addr(host.c_str());

    if (connect(getSocketNumber(), (struct sockaddr*)&hostAddress, sizeof hostAddress) != 0) {
        try{
            close();
        } catch (const SocketException& se){
            std::cerr << se.what() << std::endl;
        }
    }
}

SocketInputStream Socket::getInputStream() {
    return SocketInputStream(this);
}

SocketOutputStream Socket::getOutputStream() {
    return SocketOutputStream(this);
}

ServerSocket::ServerSocket(int port) : BaseSocket(::socket(PF_INET, SOCK_STREAM, 0)) {
    struct sockaddr_in hostAddress {};
    std::memset(&hostAddress, 0, sizeof hostAddress);

    hostAddress.sin_family = AF_INET;
    hostAddress.sin_port = htons(port);
    hostAddress.sin_addr.s_addr = INADDR_ANY;

    if (::bind(getSocketNumber(), (struct sockaddr*)&hostAddress, sizeof hostAddress) != 0) {
        close();
        std::cout << "Error bindind" << std::endl;
        throw SocketException("Error binding");
    }

    if (::listen(getSocketNumber(), 255) != 0) {
        close();
        std::cout << "Error listening" << std::endl;
        throw SocketException("Error listening");
    }
}

Socket ServerSocket::accept() {
    if (!isOpen()) {
        throw SocketException("Socket is not open");
    }

    struct sockaddr clientAddress;
    socklen_t addressSize = sizeof clientAddress;
    int socketNumber = ::accept(getSocketNumber(), (struct sockaddr*)&clientAddress, &addressSize);

    if (socketNumber == -1) {
        throw SocketException("Socket was not able to accept connection");
    }
    
    return Socket(socketNumber);
    
}
