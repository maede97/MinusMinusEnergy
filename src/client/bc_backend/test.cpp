#include <crypt/BigInt64.hpp>
#include <hash.hpp>
#include <algorithm>
#include <random>
#include <iostream>
#include <siostream.hpp>
#include <util.hpp>
int main(){
	circular_buffer a(1024);
	for(int i = 0;i < 200;i++){
		a.push_back('(');
		a.push_back('O');
		a.push_back('w');
		a.push_back('O');
		a.push_back(')');
	}
	std::for_each(a.begin(), a.end(), [](const char& c){std::cout << c;});
	std::cout << "\n" << (a.begin() - a.end()) << "\n";
	std::cout << a.full() << std::endl;
}
