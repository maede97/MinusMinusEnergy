#include "datahandler.hpp"
#include <cstdio>
using std::printf;
int main(){
	/*sqlite3 *db;

	sqlite3_stmt *stmt;
	sqlite3_open("../DB_Template.db", &db);
	printf("Performing query...\n");
	
	sqlite3_prepare_v2(db, "select * from sensor_data", -1, &stmt, NULL);
	
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		int num_cols = sqlite3_column_count(stmt);
		for (int i = 0; i < num_cols; i++){
			switch (sqlite3_column_type(stmt, i)){
			case (SQLITE3_TEXT):
				printf("%s, ", sqlite3_column_text(stmt, i));
				break;
			case (SQLITE_INTEGER):
				printf("%d, ", sqlite3_column_int(stmt, i));
				break;
			case (SQLITE_FLOAT):
				printf("%g, ", sqlite3_column_double(stmt, i));
				break;
			default:
				break;
			}
		}
		printf("\n");
	}

	printf("Got results:\n");*/
	auto vec = queryMonth("../DB_Template.db", std::make_pair<unsigned int, unsigned int>(2017, 1));
	std::cout << vec[0];
	return 0;
}
