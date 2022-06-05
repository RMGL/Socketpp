#include "BaseSocket.h"

namespace socketpp {

BaseSocket::BaseSocket(int fileDescriptor)
    : fD(fileDescriptor), closed(false) {}

BaseSocket::~BaseSocket() { close(); }

void BaseSocket::close() {
    if (closed) {
        return;
    }

    ::close(fD);

    closed = true;
    fD = -1;
}

int BaseSocket::getSocketNumber() const { return fD; }

bool BaseSocket::isOpen() const { return !closed; }

}  // namespace socketpp
