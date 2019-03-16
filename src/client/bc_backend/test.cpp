#include <crypt/BigInt64.hpp>
#include <hash.hpp>
#include <algorithm>
#include <random>
#include <iostream>
#include <siostream.hpp>
#include <socket_streambuf.hpp>
#include <util.hpp>
int main(){
    udpsocket sock(25025);
    socket_ostreambuf ob(&sock, "127.0.0.1", 10000);
    std::ostream out(&ob);
    socket_istreambuf ib(&sock);
    std::istream in(&ib);
    char x;
    in >> x;
    std::cout << x << "\n";
    in >> x;
    std::cout << x << "\n";
    out << "sdssdf";
}
