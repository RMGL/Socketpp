#include "RWSocket.h"

#include <sys/socket.h>

namespace socketpp {

using namespace socketpp::exceptions;

std::size_t RWSocket::read(char* buffer, const std::size_t size) {
    readLock.lock();

    long int read = recv(getSocketNumber(), buffer, size, 0);

    if (read == -1) {
        try {
            close();
        } catch (const socketExceptions::SocketException& se) {
            std::cerr << se.what() << std::endl;
        }
    }

    readLock.unlock();
    return static_cast<std::size_t>(read);
}

std::size_t RWSocket::write(const char* buffer, const std::size_t size) {
    writeLock.lock();
    long int written = send(getSocketNumber(), buffer, size, 0);

    if (written == -1) {
        try {
            close();
        } catch (const socketExceptions::SocketException& se) {
            std::cerr << se.what() << std::endl;
        }
    }

    writeLock.unlock();
    return static_cast<std::size_t>(written);
}
}  // namespace socketpp
