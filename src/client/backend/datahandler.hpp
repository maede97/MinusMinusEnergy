#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>
#include <sqlite3.h>
/* PRE:  Data on energy consumption sent by sensor
* POST: Data stored internally
*/


inline std::vector<std::string> queryMonth(const std::string& filename, std::pair<unsigned int, unsigned int> month){
	sqlite3 *db;
	sqlite3_stmt *stmt;
	sqlite3_open("../database.db", &db);
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
	std::cout << statstring << std::endl;
	sqlite3_prepare_v2(db, statstring.c_str(), -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		std::cout << (int*)sqlite3_column_text(stmt, 2) << std::endl;
		ret.push_back(std::string((const char*)sqlite3_column_text(stmt, 2)));
	}
	return ret;
}

inline unsigned int sumMonth(const std::string& filename, std::pair<unsigned int, unsigned int> month){
	sqlite3 *db;
	sqlite3_stmt *stmt;
	sqlite3_open(filename.c_str(), &db);
	std::string month_string = std::to_string(month.first) + "-" + std::to_string(month.second);
	std::string month_begin = std::to_string(month.first) + "-" + std::to_string(month.second) + "-01 00:00:00";
	unsigned int ret = 0;
	month.second++;
	if(month.second == 13){
		month.first++;
		month.second = 1;
	}
	std::string month_end = std::to_string(month.first) + "-" + std::to_string(month.second) + "-01 00:00:00"; + " 00:00:00";
	std::string statstring = "";
	statstring += "select SUM(data) from (select * from sensor_data WHERE time >= '" + month_begin + "' and time < '" + month_end + "')";
	
	//std::cout << statstring << std::endl;
	sqlite3_prepare_v2(db, statstring.c_str(), -1, &stmt, NULL);
	sqlite3_step(stmt);
	ret += sqlite3_column_int(stmt, 0);
	return ret;
}
inline std::string tmtostring(std::tm* now){
	unsigned int year = now->tm_year + 1900;
	unsigned int mon = now->tm_mon + 1;
	unsigned int day = now->tm_mday;
	unsigned int hour = now->tm_hour;
	unsigned int minute = now->tm_min;
	unsigned int second = now->tm_sec;
	return std::to_string(year) + "-" + std::to_string(mon) + "-" + std::to_string(day) + " " + std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
}
inline unsigned int sumInterval(const std::string& filename, const std::pair<std::tm*,std::tm*>& time){
	sqlite3 *db;
	sqlite3_stmt *stmt;
	sqlite3_open(filename.c_str(), &db);
	
	
	std::string begin = tmtostring(time.first);
	std::string end = tmtostring(time.second);
	
	unsigned int ret = 0;
	
	std::string statstring = "select SUM(data) from (select * from sensor_data WHERE time >= '" + begin + "' and time < '" + end + "')";
	
	std::cout << statstring << std::endl;
	sqlite3_prepare_v2(db, statstring.c_str(), -1, &stmt, NULL);
	sqlite3_step(stmt);
	ret += sqlite3_column_int(stmt, 0);
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
// POST: Data signed using private key
bool signData();

// POST: Data stored in SQLite DB for eternity
bool storeData();

// POST: Data sent to token handler smart contract
bool sendData();

#endif /* DATAHANDLER_HPP */
