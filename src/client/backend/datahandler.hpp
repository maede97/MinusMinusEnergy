#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP
#include <tuple>
/* PRE:  Data on energy consumption sent by sensor
 * POST: Data stored internally
 */

template<typename... T>
std::tuple<T...> readData(std::string filename, unsigned int line){
	
}

// POST: Data signed using public key
bool signData();

// POST: Data stored in SQLite DB for eternity
bool storeData();

// POST: Data sent to token handler smart contract
bool sendData();

#endif /* DATAHANDLER_HPP */
