#ifndef SQLiteUtils_H
#define SQLiteUtils_H

#include <string>
#include <vector>
#include <utility>      // std::pair
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
	std::string dataGrabber(std::string &word, std::size_t &startPos);
	
public:
	typedef std::pair <std::string, std::string> dbDataPair;
	
	SQLiteUtils(std::string name);
	SQLiteUtils();
	~SQLiteUtils();
	bool openSQLiteDB(std::string name, std::string &output);
	void closeSQLiteDB(std::string &output);
	bool IsOpen();
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
	int GetLatestRowID();
	std::string GetDataFromID(int id, std::string table = "");
	std::string GetDataFromSingleLineOutput(std::string colName);

	bool insertNewDataEntry(dbDataPair data, std::string &output);
	bool insertNewDataEntry( std::vector<dbDataPair> data, std::string &output);
	bool doDBQuerey(std::string data, std::string &output);
	bool doDBQuerey(std::string data, dbDataPair fromWhere, std::string &output);
	bool doDBQuerey(std::vector<std::string> data, std::string &output);
	bool doDBQuerey(std::vector<dbDataPair> data, std::string &output);
	bool doDBQuerey(std::string selectData, std::vector<dbDataPair> whereData, std::string &output);
	bool doDBQuerey(std::vector<std::string> selectData, std::vector<dbDataPair> whereData, std::string &output);
	bool UpdateStringEntry(std::vector<dbDataPair> data, dbDataPair WhereClause, std::string &output);
	bool UpdateIntEntry(std::vector<dbDataPair> data, dbDataPair WhereClause, std::string &output);
	void getDataPairFromOutput(std::string &inputData, std::string colName1, std::string colName2, std::vector<dbDataPair> &returnData);
	void getAllValuesFromCol(std::string &inputData, std::string colName, std::vector<std::string> &returnData);
	int GetLatestID();
	void SplitDataIntoResults(std::vector<std::string> &returnData, std::string allData,std::string firstField, bool removeNewline = true);
private:
	std::string GetUpdateQuereyString(std::vector<dbDataPair> data, dbDataPair WhereClause);
};
#endif //SQLiteUtils_H