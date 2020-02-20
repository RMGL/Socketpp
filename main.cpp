#include "Socketpp.h"

int main(){

    socketpp::Socket s("123", 1234);
    s.getInputStream();

    return 0;
}