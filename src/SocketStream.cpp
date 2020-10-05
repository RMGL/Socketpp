#include "SocketStream.h"

using namespace socketpp;

void SocketStream::ensureOpen() {
    if (closed) {
        std::string error_message = "Socket is not open";
        std::cerr << error_message << std::endl;
        throw SocketException(error_message);
    }
}

bool SocketStream::isOpen() {
    return !closed;
}

std::size_t SocketInputStream::available() {
    ensureOpen();

    int bytesAvailable;
    #ifdef _WIN32

    #endif
    // NOTE Linux only
    ioctl(socket->getSocketNumber(), FIONREAD, &bytesAvailable);

    return bytesAvailable;
}

std::size_t SocketInputStream::read(char* c) {
    char toReturn[1];
    std::size_t r = read(&toReturn[0], 1);
    memset(c, toReturn[0], 1);
    return r;
}

std::size_t SocketInputStream::read(char* buffer, std::size_t size) {
    return read(buffer, size, 0, size);
}

std::size_t SocketInputStream::read(char* buffer, std::size_t size, std::size_t offset, std::size_t length) {
    ensureOpen();
    std::memset(buffer, 0, size);

    if ((length | offset | size) < 0 || size > length - offset) {
        //TODO error
    }

    size_t read = 0;
    while (read < size) {
        size_t readThisRound = socket->read((buffer + read + offset), length - read);

        read += readThisRound;
    }

    return read;
}

std::size_t SocketInputStream::readUntil(char* buffer, std::size_t size, char delimiter) {
    std::memset(buffer, 0, size);
    size_t readFromSocket = 0;
    std::vector<char> b;

    while (readFromSocket < size) {
        char c;
        readFromSocket += read(&c);

        b.push_back(c);

        if (c == delimiter) {
            break;
        }
    }

    memcpy(buffer, b.data(), readFromSocket);

    return readFromSocket;
}

std::string SocketInputStream::readString() {
    char stringBytes[65535];
    size_t t = readUntil(stringBytes, 65535, '\0');
    char trimmed[t - 1];

    std::memcpy(trimmed, stringBytes, t - 1);

    return std::string(trimmed, t - 1);
}

std::size_t SocketInputStream::discard(long n) {
    char toDiscard[n];
    return read(&toDiscard[0], n, 0, n);
}

std::vector<char> SocketInputStream::operator>>(std::vector<char>& v) {
    char stringBytes[65535];
    size_t t = read(stringBytes, 65535);
    char trimmed[t];

    std::memcpy(trimmed, stringBytes, t);
    v.clear();
    std::copy(v.begin(), v.end(), trimmed);
    return v;
}

std::string SocketInputStream::operator>>(std::string& s) {
    s = this->readString();
    return s;
}

std::size_t SocketOutputStream::write(const char* buffer, std::size_t size, std::size_t offset, std::size_t length) {
    ensureOpen();

    if ((length | offset | size) < 0 || size > length - offset) {
        //TODO error
    }

    size_t written = 0;
    while (written < size) {
        size_t writtenThisRound = socket->write(buffer + written + offset, length - written);

        written += writtenThisRound;
    }

    return written;
}

std::size_t SocketOutputStream::write(char c) {
    return write(&c, 1);
}

std::size_t SocketOutputStream::write(const char* buffer, std::size_t size) {
    return write(buffer, size, 0, size);
}

std::size_t SocketOutputStream::write(const std::vector<char> v) {
    return write(v.data(), v.size());
}

std::size_t SocketOutputStream::writeString(std::string s) {
    size_t bytesSize = s.length() + 1;
    char bytes[bytesSize];
    std::memcpy(bytes, s.c_str(), bytesSize - 1);

    bytes[bytesSize - 1] = '\0';

    return write(bytes, bytesSize, 0, bytesSize);
}

std::size_t SocketOutputStream::operator<<(const std::vector<char>& v) {
    return this->write(v);
}

std::size_t SocketOutputStream::operator<<(const std::string& s) {
    return this->writeString(s);
}
