#include "Socket.h"

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <cstring>
#include <memory>

#include "Exceptions/SocketException.h"
#include "SocketStream.h"

using namespace socketpp;

Socket::Socket(const std::string& host, int port)
    : RWSocket(::socket(PF_INET, SOCK_STREAM, 0)) {
    struct sockaddr_in hostAddress {};
    std::memset(&hostAddress, 0, sizeof hostAddress);

    hostAddress.sin_family = AF_INET;
    hostAddress.sin_port = htons(port);
    hostAddress.sin_addr.s_addr = inet_addr(host.c_str());

    if (connect(getSocketNumber(), (struct sockaddr*)&hostAddress,
                sizeof hostAddress) != 0) {
        try {
            close();
        } catch (const exceptions::socketExceptions::SocketException& se) {
            std::cerr << se.what() << std::endl;
        }
    } else {
        inputStream =
            std::make_shared<SocketInputStream>(SocketInputStream(*this));
        outputStream =
            std::make_shared<SocketOutputStream>(SocketOutputStream(*this));
    }
}

Socket::Socket(int fileDescriptor) : RWSocket(fileDescriptor) {
    inputStream = std::make_shared<SocketInputStream>(SocketInputStream(*this));
    outputStream = std::make_shared<SocketOutputStream>(SocketOutputStream(*this));
}

Socket::Socket(const Socket& obj) : RWSocket(obj.getSocketNumber()) {}

Socket::~Socket() = default;

std::shared_ptr<SocketInputStream> Socket::getInputStream() {
    return inputStream;
}

std::shared_ptr<SocketOutputStream> Socket::getOutputStream() {
    return outputStream;
}

ServerSocket::ServerSocket(int port)
    : BaseSocket(::socket(PF_INET, SOCK_STREAM, 0)) {
    struct sockaddr_in hostAddress {};
    std::memset(&hostAddress, 0, sizeof hostAddress);

    hostAddress.sin_family = AF_INET;
    hostAddress.sin_port = htons(port);
    hostAddress.sin_addr.s_addr = INADDR_ANY;

    if (::bind(getSocketNumber(), (struct sockaddr*)&hostAddress,
               sizeof hostAddress) != 0) {
        close();
        throw exceptions::socketExceptions::SocketException("Error binding");
    }

    if (::listen(getSocketNumber(), 255) != 0) {
        close();
        throw exceptions::socketExceptions::SocketException("Error listening");
    }
}

Socket ServerSocket::accept() {
    if (!isOpen()) {
        throw exceptions::socketExceptions::SocketException(
            "Socket is not open");
    }

    struct sockaddr clientAddress;
    socklen_t addressSize = sizeof clientAddress;
    int socketNumber = ::accept(getSocketNumber(),
                                (struct sockaddr*)&clientAddress, &addressSize);

    if (socketNumber == -1) {
        throw exceptions::socketExceptions::SocketException(
            "Socket was not able to accept connection");
    }

    return Socket(socketNumber);
}