#include <crypt/BigInt64.hpp>
#include <hash.hpp>
#include <algorithm>
#include <random>
#include <iostream>
#include <siostream.hpp>
#include <socket_streambuf.hpp>
#include <util.hpp>
#include <udpsocket.hpp>
int main(){
    udpsocket sock(25025);
    socket_ostreambuf ob(&sock, "127.0.0.1", 10000);
    while(true){
        packet pack = sock.receiveP();
        std::cout << pack.content << std::endl;
        std::cout << inet_ntoa(pack.addr.sin_addr) << std::endl;
    }
}
