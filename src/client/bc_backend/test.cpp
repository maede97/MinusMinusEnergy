#include <crypt/BigInt64.hpp>
#include <hash.hpp>
#include <algorithm>
#include <random>
#include <iostream>
#include <siostream.hpp>
#include <socket_streambuf.hpp>
#include <util.hpp>
#include <udpsocket.hpp>
unsigned short swapBytes(unsigned short k){
    return (k << 8) | (k >> 8);
}
int main(){
    udpsocket sock(25025);
    //socket_ostreambuf ob(&sock, "127.0.0.1", 10000);
    socket_istreambuf ib(&sock);
    std::cout << sizeof(std::istream) << std::endl;
    std::istream is(&ib);
    char a;
    while(true){
        is.read(&a, 1);
        std::cout << ": "<< a << std::endl;
    }
}
