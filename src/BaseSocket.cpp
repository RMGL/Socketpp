#include "BaseSocket.h"

namespace socketpp {

using namespace socketpp::exceptions;

void BaseSocket::close() {
    // if (closed) {
    //     throw socketExceptions::SocketException("Socket is already closed");
    // }
    ::close(fD);

    closed = true;
    fD = -1;
}

}  // namespace socketpp
