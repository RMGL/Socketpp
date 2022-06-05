#pragma once

#include <memory>

#include "RWSocket.h"

namespace socketpp {

class SocketInputStream;
class SocketOutputStream;

class Socket : public RWSocket {
   public:
    /**
     *  @brief Client socket abstraction, opens a TCP connection with the host
     *  @param host string containing host address
     *  @param port host port
     */
    Socket(const std::string& host, int port);

    explicit Socket(int fileDescriptor);

    Socket(const Socket& obj);
    ~Socket();

    /**
     *  @brief Opens an input stream for the socket
     *  @return InputStream for the socket
     */
    std::shared_ptr<SocketInputStream> getInputStream();

    /**
     *  @brief Opens an output stream for the socket
     *  @return OutputStream for the socket
     */
    std::shared_ptr<SocketOutputStream> getOutputStream();

   private:
    std::shared_ptr<SocketInputStream> inputStream;
    std::shared_ptr<SocketOutputStream> outputStream;
};

class ServerSocket : public BaseSocket {
   public:
    /**
     *  @brief Server socket abstraction, opens a TCP connection. May throw an
     * exception on binding or listening error
     *  @param port port where the socket will wait for connections
     */
    explicit ServerSocket(int port);
    ~ServerSocket() = default;

    /**
     *  @brief Accepts incoming client connections
     *  @return Socket that holds a connection to a single client or an
     * exception if the socket was not able to accept the connection or is not
     * open
     */
    Socket accept();
};
}  // namespace socketpp
