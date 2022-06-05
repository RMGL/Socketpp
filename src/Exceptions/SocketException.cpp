#include "SocketException.h"

namespace socketpp {
namespace exceptions {
namespace socketExceptions {

SocketException::SocketException(std::string _message) : message(_message) {}

const char* SocketException::what() const throw() { return message.c_str(); }

}  // namespace socketExceptions
}  // namespace exceptions
}  // namespace socketpp