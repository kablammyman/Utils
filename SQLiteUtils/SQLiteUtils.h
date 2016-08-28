#ifndef SQLiteUtils_H
#define SQLiteUtils_H

#include <string>
#include <vector>
#include "SQLite3/src/sqlite3.h"

class SQLiteUtils
{
	int returnCode;
	sqlite3 *db;
	std::string lastError;
	std::string DBName;
	std::string curTableName;
	static int callback(void *data, int argc, char **argv, char **azColName);
	static std::string returnData;
	static bool gettingData;
public:
	SQLiteUtils(std::string name);
	SQLiteUtils();
	~SQLiteUtils();
	bool openSQLiteDB(std::string name, std::string &output);
	void closeSQLiteDB(std::string &output);
	std::string getLastError();
	bool executeSQL(std::string command, std::string &output);

	bool createTable(std::string name, std::string query);
	std::vector<std::string> displayTable();
	
	std::vector<std::string> viewData() ;
	std::vector<std::string> query(char* query);
	std::vector<std::vector<std::string>> queryV2(char* query);
	std::vector<std::string> getAllTables();
	bool insertData(std::string query); 
	void setTableName(std::string name);
};
#endif //SQLiteUtils_H