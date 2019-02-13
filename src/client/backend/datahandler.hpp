#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP
#include <iostream>
#include <tuple>
/* PRE:  Data on energy consumption sent by sensor
 * POST: Data stored internally
 */

template<typename... T>
std::tuple<T...> readData(const std::string& filename, const unsigned int line){
	std::ifstream in_stream;
	in_stream.open(filename);
	std::string line_n;
	for(int i = 0;i < line;i++){
		std::getline(in_stream, line_n);
	}
	return readLine(line_n);
}
template<typename... T>
std::tuple<T...> readLine(const std::string& arg){
	std::tuple<T...> return_value;
	const unsigned int size = std::tuple_size<std::tuple<T...>>::value;
}

// POST: Data signed using public key
bool signData();

// POST: Data stored in SQLite DB for eternity
bool storeData();

// POST: Data sent to token handler smart contract
bool sendData();

#endif /* DATAHANDLER_HPP */
