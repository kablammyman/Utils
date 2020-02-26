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
	std::string DataGrabber(std::string &word, std::size_t &startPos);
	
public:
	typedef std::pair <std::string, std::string> dbDataPair;
	
	SQLiteUtils(std::string name);
	SQLiteUtils();
	~SQLiteUtils();
	bool openSQLiteDB(std::string name, std::string &output);
	void closeSQLiteDB(std::string &output);
	bool IsOpen();
	std::string getLastError();
	bool ExecuteSQL(std::string command, std::string &output);

	bool createTable(std::string name, std::string query);
	//std::vector<std::string> displayTable();
	
	std::vector<std::string> viewData() ;
	std::vector<std::string> query(char* query);
	std::vector<std::vector<std::string>> queryV2(char* query);
	//std::vector<std::string> getAllTables();
	bool insertData(std::string query); 
	void setTableName(std::string name);
	int GetLatestRowID();
	std::string GetDataFromID(int id, std::string table = "");
	std::string GetDataFromSingleLineOutput(std::string colName);

	bool InsertNewDataEntry(dbDataPair data, std::string &output);
	bool InsertNewDataEntry( std::vector<dbDataPair> data, std::string &output);
	bool DoDBQuerey(std::string data, std::string &output);
	bool DoDBQuerey(std::string data, dbDataPair fromWhere, std::string &output);
	bool DoDBQuerey(std::vector<std::string> data, std::string &output);
	bool DoDBQuerey(std::vector<dbDataPair> data, std::string &output);
	bool DoDBQuerey(std::string selectData, std::vector<dbDataPair> whereData, std::string &output);
	bool DoDBQuerey(std::vector<std::string> selectData, std::vector<dbDataPair> whereData, std::string &output);
	bool UpdateStringEntry(std::vector<dbDataPair> data, dbDataPair WhereClause, std::string &output);
	bool UpdateIntEntry(std::vector<dbDataPair> data, dbDataPair WhereClause, std::string &output);
	void GetDataPairFromOutput(std::string &inputData, std::string colName1, std::string colName2, std::vector<dbDataPair> &returnData);
	void GetAllValuesFromCol(std::string &inputData, std::string colName, std::vector<std::string> &returnData);
	void GetAllColsInTable(std::string tableName, std::vector<std::string> &retData);
	int GetLatestID();
	void GetAllTablesInDB(std::vector<std::string> &retData);
	void SplitDataIntoResults(std::vector<std::string> &returnData, std::string allData,std::string firstField, bool removeNewline = true);
private:
	std::string GetUpdateQuereyString(std::vector<dbDataPair> data, dbDataPair WhereClause);
};
#endif //SQLiteUtils_H