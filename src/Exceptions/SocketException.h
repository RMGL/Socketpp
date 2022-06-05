#pragma once

#include <iostream>
#include <string>

namespace socketpp {
namespace exceptions {
namespace socketExceptions {
    
// TODO specify exceptions
class SocketException : public std::exception {
   protected:
    std::string message;

   public:
    /** Cosntructor (C++ STL string)
     * @param _message The error message
     */
    explicit SocketException(std::string _message);

    virtual const char* what() const throw();
};
}  // namespace socketExceptions
}  // namespace exceptions
}  // namespace socketpp
