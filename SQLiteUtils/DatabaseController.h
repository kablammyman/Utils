#pragma once

#include <string>
#include <utility>      // std::pair
#include <vector>
#include "SQLiteUtils.h"
using namespace std;

class DatabaseController
{
	SQLiteUtils *db;
	string dbName;
	string dataGrabber(string &word, size_t &startPos);

public:
		
	
	
	typedef pair <string, string> dbDataPair;
	
	//each querey will have an array of these
	struct DBResult
	{
		//this is for each col and value we ask for
		//select name,addy,phone from person; -> name:vic|addy:123 fake|phone:1234567890
		vector<dbDataPair> data;
		
		void insert(string fieldName,string value)
		{
			data.push_back(make_pair(fieldName, value));
		}

		void clear()
		{
			data.clear();
		}
		void GetAllKeys(vector<string> &retVec)
		{
			for (size_t i = 0; i < data.size(); i++)
			{
				retVec.push_back(data[i].first);
			}
		}

		void GetAllValues(vector<string> &retVec)
		{
			for (size_t i = 0; i < data.size(); i++)
			{
				retVec.push_back(data[i].second);
			}
		}

		string GetValueFromKey(string key)
		{
			for (size_t i = 0; i < data.size(); i++)
			{
				if(data[i].first == key)
				{
					return data[i].second;
				}
			}
			return "";
		}
	};
	DatabaseController();
	~DatabaseController();

	bool openDB(string path);
	bool isDBOpen();
	bool createNewDB(string newDBName, string createCommand="");
	bool executeSQL(string command, string &output);
	string getDBName();
	string getTable(string tableName);
	string getLastError();

	bool insertNewDataEntry(string table, dbDataPair data, string &output);
	bool insertNewDataEntry(string table, vector<dbDataPair> data, string &output);
	bool doDBQuerey(string querey, string &output);
	bool doDBQuerey(string table, string data, string &output);
	bool doDBQuerey(string table, string data, dbDataPair fromWhere, string &output);
	bool doDBQuerey(string table, vector<string> data, string &output);
	bool doDBQuerey(string table, vector<dbDataPair> data, string &output);
	bool doDBQuerey(string table, string selectData, vector<dbDataPair> whereData, string &output);
	
	//select x,y,z from table where dbpair.first = dbpair.second
	bool doDBQuerey(string table, vector<string> selectData, dbDataPair whereData, string &output);
	
	// SELECT x,y,x FROM Table WHERE dbpair[0].first = dbpair[0].second  AND dbpair[1].first = dbpair[1].second ;
	bool doDBQuerey(string table, vector<string> selectData, vector<dbDataPair> whereData, string &output);
	bool UpdateEntry(string table, vector<dbDataPair> data, dbDataPair WhereClause, string &output);
	
	void parseDBOutput(string &inputData, int numFields, vector <vector<string>> &returnData);
	void parseDBOutput(string &inputData, int numFields, vector<DBResult> &returnData);
	
	//num cols is hopw many cols were truthned from the querey, and the colToUse is the data we want to put in the vector
	void removeTableNameFromOutput(string &inputData, int numCols, int colToUse, vector<string> &returnData);
	void removeTableNameFromOutput(string &inputData);
	//same as before, but now we can couple data together
	void getDataPairFromOutput(string &inputData, string colName1, string colName2, vector<dbDataPair> &returnData);
	void getAllValuesFromCol(string &inputData, string colName, vector<string> &returnData);
	
	int GetLatestRowID()
	{
		return db->GetLatestRowID();
	}
	//test methods
	void createTable(string tableName, string fields);
	void testGetTable();
	void testDBEntry();
	void testDBQuerey();
};