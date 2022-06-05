#pragma once

#include <unistd.h>

#include "Exceptions/SocketException.h"

namespace socketpp {

class BaseSocket {
   private:
    int fD;
    bool closed;

   protected:
    /**
     *  @brief Base socket class, saves logical information about sockets
     *  @param fileDescriptor socket file descriptor
     */
    BaseSocket(int fileDescriptor);
    ~BaseSocket();

   public:
    /**
     *  @brief getter for socket number
     *  @return socket number
     */
    int getSocketNumber() const;

    /**
     *  @brief closes the socket
     */
    void close();

    /**
     *  @brief checks if the socket is open
     *  @return true if open, false otherwise
     */
    bool isOpen() const;
};
}  // namespace socketpp