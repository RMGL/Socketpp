#pragma once

#include "BaseSocket.h"

#include <mutex>

namespace socketpp {

class RWSocket : public BaseSocket {
   private:
    std::mutex readLock;
    std::mutex writeLock;

   protected:
    /**
     *  @brief I/o socket class, offers a basic read and write operation for the
     * socket
     *  @param fileDescriptor socket file descriptor
     */
    RWSocket(int fileDescriptor) : BaseSocket(fileDescriptor) {}
    ~RWSocket() = default;

   public:
    /**
     *  @brief Reads n bytes to buffer
     *  @param buffer destination to hold the data read
     *  @param n number of bytes to read
     *  @return number of bytes read
     */
    std::size_t read(char* buffer, const std::size_t n);

    /**
     *  @brief Writes n bytes from buffer
     *  @param buffer source of data to write
     *  @param n number of bytes to write
     *  @return number of bytes wrote
     */
    std::size_t write(const char* buffer, const std::size_t n);
};

}  // namespace socketpp