#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <vector>
#include <cstring>
#include <sqlite3.h>
/* PRE:  Data on energy consumption sent by sensor
* POST: Data stored internally
*/


inline std::vector<std::string> queryMonth(const std::string& filename, std::pair<unsigned int, unsigned int> month){
	sqlite3 *db;
	sqlite3_stmt *stmt;
	sqlite3_open("../DB_Template.db", &db);
	std::string month_string = std::to_string(month.first) + "-" + std::to_string(month.second);
	std::string month_begin = std::to_string(month.first) + "-" + std::to_string(month.second) + "-01 00:00:00";
	std::vector<std::string> ret;
	month.second++;
	if(month.second == 13){
		month.first++;
		month.second = 1;
	}
	std::string month_end = std::to_string(month.first) + "-" + std::to_string(month.second) + "-01 00:00:00"; + " 00:00:00";
	std::string statstring = "";
	statstring += "select * from sensor_data WHERE time >= '" + month_begin + "' and time < '" + month_end + "'";
	//std::cout << statstring << std::endl;
	sqlite3_prepare_v2(db, statstring.c_str(), -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		//std::cout << (int*)sqlite3_column_text(stmt, 2) << std::endl;
		ret.push_back(std::string((const char*)sqlite3_column_text(stmt, 2)));
	}
	return ret;
}
/*template<typename... T>
std::tuple<T...> readLine(const std::string& arg){
	std::tuple<T...> return_value;
	const unsigned int size = std::tuple_size<std::tuple<T...>>::value;
	auto iter = arg.begin();
	for(int i = 0;i < size;i++){
		auto lastIter = iter;
		while(*iter != ' ' && iter != arg.end()){iter++;}
	}
}

template<typename... T>
std::tuple<T...> readData(const std::string& filename, const unsigned int line){
	std::ifstream in_stream;
	in_stream.open(filename);
	std::string line_n;
	for(int i = 0;i < line;i++){
		std::getline(in_stream, line_n);
	}
	return readLine<T...>(line_n);
}
*/
// POST: Data signed using public key
bool signData();

// POST: Data stored in SQLite DB for eternity
bool storeData();

// POST: Data sent to token handler smart contract
bool sendData();

#endif /* DATAHANDLER_HPP */
