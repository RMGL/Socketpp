#include "RWSocket.h"

namespace socketpp {

using namespace socketpp::exceptions;

RWSocket::RWSocket(int fileDescriptor) : BaseSocket(fileDescriptor) {}

RWSocket::~RWSocket() = default;

std::size_t RWSocket::read(char* buffer, const std::size_t size) {
    readLock.lock();
    long int bytesRead = recv(getSocketNumber(), buffer, size, 0);
    readLock.unlock();

    testError(bytesRead);

    return static_cast<std::size_t>(bytesRead);
}

std::size_t RWSocket::write(const char* buffer, const std::size_t size) {
    writeLock.lock();
    long int bytesWritten = send(getSocketNumber(), buffer, size, 0);
    writeLock.unlock();

    testError(bytesWritten);
    return static_cast<std::size_t>(bytesWritten);
}

void RWSocket::testError(const long int bytes) {
    if (bytes <= 0) {
        close();

        if (bytes == 0) {
            // throw Connection was closed by peer Exception
        } else {
            // throw Unknown Exception
        }
    }
}

}  // namespace socketpp
