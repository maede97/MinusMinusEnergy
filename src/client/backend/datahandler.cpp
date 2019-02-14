#include "datahandler.hpp"
#include <cstdio>
#include <string>
#include <cstdint>
#include <cassert>
#include <tuple>
#include <fstream>
#include <ctime>
#include <set>
#include "pstream.h"
template<typename T>
T relu(const T& arg){
	if(arg > 0)return arg;
	return 0;
}
template<typename... Ts>
using table = std::set<std::tuple<Ts...>>;
double timediff(std::pair<std::tm*, std::tm*> times){
	return std::difftime(std::mktime(times.first), std::mktime(times.second)) - 3600;
}
std::string paddedInt(unsigned int a, unsigned int length = 10){
	std::string k = std::to_string(a);
	while(k.size() < length){
		k = "0" + k;
	}
	return k;
}
using std::printf;
int main(int argc, char** args){
	std::uint64_t nonce = 0;
	std::string database_path;
	if(argc == 1){
		database_path = std::string("../database.db");
	}
	if(argc >= 2){
		database_path = std::string(args[1]);
	}
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
	std::tm last;
	std::ifstream ifs("lastdate.dat");
	if(ifs.good()){
		ifs >> last.tm_year;
		ifs >> last.tm_mon;
		ifs >> last.tm_mday;
		ifs >> last.tm_hour;
		ifs >> last.tm_min;
		ifs >> last.tm_sec;
		ifs >> nonce;
	}
	ifs.close();
	//std::cout << timediff({now, &last}) << std::endl;
	unsigned int year = now->tm_year + 1900;
	unsigned int mon = now->tm_mon + 1;
	unsigned int day = now->tm_mday;
	unsigned int hour = now->tm_hour;
	unsigned int minute=now->tm_min ;
	unsigned int second=now->tm_sec ;
	std::ofstream ofs("lastdate");
	ofs << now->tm_year<<"\n";
	ofs << now->tm_mon<<"\n";
	ofs << now->tm_mday<<"\n";
	ofs << now->tm_hour<<"\n";
	ofs << now->tm_min <<"\n";
	ofs << now->tm_sec <<"\n";
	ofs << (nonce + 1) << std::endl;
	ofs.close();
	mon--;
	if(mon < 1){
		year--;
		mon = 12;
	}
	unsigned int energyConsumed = sumInterval(database_path, {now,now});
	
	double avg_consumption = (double)energyConsumed / timediff({now, &last});
	
	std::uint64_t earned = relu(10.0 - avg_consumption) * 1e12;
	//std::cout << "Consumed: " << energyConsumed << std::endl;
	
	std::string commandarg = std::to_string(earned);
	
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
	//std::cout << str << std::endl;
	std::vector<std::string> jargs;
	jargs.push_back(str);
	jargs.push_back(commandarg);
	jargs.push_back(timestamp);
	redi::ipstream JS("node " + str + " " + commandarg + " " + timestamp);
	//std::cout << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday << std::endl;
	std::string errmsg;
	while (std::getline(JS.err(), errmsg)) {
		std::cerr << errmsg << std::endl;
	}
	return 0;
}
