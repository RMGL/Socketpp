#include "SocketStream.h"

#include <sys/ioctl.h>

#include <cstring>
#include <vector>

using namespace socketpp;

void SocketStream::ensureOpen() {
    if (closed) {
        std::string error_message = "Socket is not open";
        std::cerr << error_message << std::endl;
        throw SocketStreamException(error_message);
    }
}

bool SocketStream::isOpen() { return !closed; }

std::size_t SocketInputStream::available() {
    ensureOpen();

    int bytesAvailable;
#ifdef _WIN32
    // TODO handle device control in Windows
#endif
    // NOTE Linux only
    ioctl(socket.getSocketNumber(), FIONREAD, &bytesAvailable);

    return bytesAvailable;
}

std::size_t SocketInputStream::read(char* c) { return read(c, 1); }

std::size_t SocketInputStream::read(char* buffer, std::size_t size) {
    return read(buffer, size, 0, size);
}

std::size_t SocketInputStream::read(char* buffer, std::size_t size,
                                    std::size_t offset, std::size_t length) {
    std::size_t read = 0;

    try {
        ensureOpen();
        std::memset(buffer, 0, size);

        if (size > length - offset) {
            throw SocketStreamException(
                "Size must not be greater than (length - offset)");
        } else if ((length | offset | size) < 0) {
            throw SocketStreamException(
                "Size, length and offset must be greater than 0");
        }

        read += socket.read((buffer + offset), length);

    } catch (const SocketStreamException& sse) {
        std::cerr << sse.what() << std::endl;
    }
    return read;
}

std::size_t SocketInputStream::readUntil(char* buffer, std::size_t size,
                                         char delimiter) {
    std::memset(buffer, 0, size);

    size_t readFromSocket = 0;
    std::vector<char> b;

    while (readFromSocket < size) {
        char c;
        size_t r = read(&c);
        readFromSocket += r;

        b.push_back(c);

        if (c == delimiter) {
            break;
        }
    }
    memcpy(buffer, b.data(), readFromSocket);
    return readFromSocket;
}

std::size_t SocketInputStream::readUntilString(char* buffer, std::size_t size,
                                         const std::string& tokens) {
    std::memset(buffer, 0, size);

    size_t readFromSocket = 0;
    std::vector<char> b;

    while (readFromSocket < size) {
        char c;
        size_t r = read(&c);
        readFromSocket += r;

        b.push_back(c);

        if (readFromSocket >= tokens.length()) {  // we can start comparing
            bool conditionMet = true;

            std::vector<char>::iterator it = b.end() - tokens.length();

            for (int i = 0; it != b.end(); it++, i++) {
                conditionMet &= (*it == tokens.c_str()[i]);

                if (!conditionMet) {
                    break;
                }
            }

            if (conditionMet) {
                break;
            }
        }
    }
    memcpy(buffer, b.data(), readFromSocket);
    return readFromSocket;
}

std::string SocketInputStream::readString() {
    std::vector<char> fullBytes;
    char stringBytes[65535];
    size_t read = 0;

    do {
        read = readUntil(stringBytes, 65535, '\0');

        fullBytes.insert(fullBytes.cend(), stringBytes, stringBytes + read);
    } while (read == 65535 && stringBytes[65534] != '\0');

    return std::string(fullBytes.data(), fullBytes.size());
}

std::size_t SocketInputStream::discard(long n) {
    char toDiscard[n];
    return read(&toDiscard[0], n, 0, n);
}

std::vector<char> SocketInputStream::operator>>(std::vector<char>& v) {
    v.clear();

    while (available()) {
        char bytes[65535];
        size_t t = read(bytes, 65535);

        v.insert(v.cend(), bytes, bytes + t);
    }

    return v;
}

std::string SocketInputStream::operator>>(std::string& s) {
    return this->readString();
}

std::size_t SocketOutputStream::write(const char* buffer, std::size_t size,
                                      std::size_t offset, std::size_t length) {
    size_t written = 0;
    try {
        ensureOpen();

        if (size > length - offset) {
            throw SocketStreamException(
                "Size must not be greater than (length - offset)");
        } else if ((length | offset | size) < 0) {
            throw SocketStreamException(
                "Size, length and offset must be greater than 0");
        }

        while (written < size) {
            written +=
                socket.write(buffer + written + offset, length - written);
        }

    } catch (const SocketStreamException& sse) {
        std::cerr << sse.what() << std::endl;
    }
    return written;
}

std::size_t SocketOutputStream::write(char c) { return write(&c, 1); }

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
