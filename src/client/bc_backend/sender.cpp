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
    udpsocket sock(32000);
    packet pack("Hello World ", "127.0.0.1", 25025);
    for(int i = 0;i < 4;i++){
        std::cout << pack.checksum[i] << ", ";
    }
    sock.write(pack);
}
