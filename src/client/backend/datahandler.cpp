#include "datahandler.hpp"
#include <cstdio>
#include <string>
#include <cassert>
#include <ctime>
#include "pstream.h"
std::string paddedInt(unsigned int a, unsigned int length = 10){
	std::string k = std::to_string(a);
	while(k.size() < length){
		k = "0" + k;
	}
	return k;
}
using std::printf;
int main(int argc, char** args){
	std::string database_path;
	if(argc == 1){
		database_path = std::string("../database.db");
	}
	if(argc >= 2){
		database_path = std::string(args[1]);
	}
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
	unsigned int year = now->tm_year + 1900;
	unsigned int mon = now->tm_mon + 1;
	unsigned int day = now->tm_mday;
	unsigned int hour = now->tm_hour;
	unsigned int minute = now->tm_min;
	unsigned int second = now->tm_sec;
	mon--;
	if(mon < 1){
		year--;
		mon = 12;
	}
	unsigned int energyConsumed = sumMonth(database_path, {year, mon});
	std::string commandarg = std::to_string(energyConsumed);
	std::string timestamp = "";
	timestamp += std::to_string(year);
	timestamp += ":";
	timestamp += std::to_string(mon);
	timestamp += ":";
	timestamp += std::to_string(day);
	timestamp += ":";
	timestamp += std::to_string(hour);
	timestamp += ":";
	timestamp += std::to_string(minute);
	timestamp += ":";
	timestamp += std::to_string(second);
	redi::ipstream in("ls *.js");
	std::string str;
	in >> str;
	redi::ipstream JS(std::string("node ") + str + commandarg + timestamp);
	//std::cout << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday << std::endl;
	
	return 0;
}