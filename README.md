## Socketpp

Socketpp is a lightweight library, designed for simplicity in using TCP sockets in C++, it was designed keeping in mind users that are not experienced with sockets or C++.

It offers two simple abstractions, the client and server socket wrapper, and a input/output socket stream.

## Usage

Simple client and client example.

### Client

``` c++
#include "Socketpp.h"

socketpp::Socket s("127.0.0.1", 1234); // host address and port

socketpp::SocketInputStream is = s.getInputStream(); // create io streams
socketpp::SocketOutputStream os = s.getOutputStream();
is.writeString("Hello");

std::string reply = os.readString();
//"world"

```

### Server

```c++
#include "Socketpp.h"

socketpp::ServerSocket ss(1234); // port

socketpp::Socket s = ss.accept(); // accept new connection

socketpp::SocketInputStream is = s.getInputStream(); // create io streams
socketpp::SocketOutputStream os = s.getOutputStream();

std::string msg = os.readString();
//"hello"

is.writeString("world");
```

## TODO list

- Handle exceptions accordingly.
- Thread safety.
- Compatibility with other OS, other than Linux

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
