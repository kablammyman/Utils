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
	static string DataGrabber(string &word, size_t &startPos);

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

	bool OpenDB(string path);
	bool IsDBOpen();
	bool CreateNewDB(string newDBName, string createCommand="");
	bool ExecuteSQL(string command, string &output);
	string GetDBName();
	string GetTable(string tableName);
	string GetLastError();

	bool InsertNewDataEntry(string table, dbDataPair data, string &output);
	bool InsertNewDataEntry(string table, vector<dbDataPair> data, string &output);
	bool DoDBQuerey(string querey, string &output);
	bool DoDBQuerey(string table, string data, string &output);
	bool DoDBQuerey(string table, string data, dbDataPair fromWhere, string &output);
	bool DoDBQuerey(string table, vector<string> data, string &output);
	bool DoDBQuerey(string table, vector<dbDataPair> data, string &output);
	bool DoDBQuerey(string table, string selectData, vector<dbDataPair> whereData, string &output);
	
	//select x,y,z from table where dbpair.first = dbpair.second
	bool DoDBQuerey(string table, vector<string> selectData, dbDataPair whereData, string &output);
	
	// SELECT x,y,x FROM Table WHERE dbpair[0].first = dbpair[0].second  AND dbpair[1].first = dbpair[1].second ;
	bool DoDBQuerey(string table, vector<string> selectData, vector<dbDataPair> whereData, string &output);
	bool UpdateEntry(string table, dbDataPair data, dbDataPair WhereClause, string &output);
	bool UpdateEntry(string table, vector<dbDataPair> data, dbDataPair WhereClause, string &output);
	
	static void ParseDBOutput(string &inputData, int numFields, vector <vector<string>> &returnData);
	static void ParseDBOutput(string &inputData, int numFields, vector<DBResult> &returnData);
	static void ParseDBOutput(string &inputData, vector<string>fields, vector<DBResult> &returnData);

	//num cols is hopw many cols were truthned from the querey, and the colToUse is the data we want to put in the vector
	static void RemoveTableNameFromOutput(string &inputData, int numCols, int colToUse, vector<string> &returnData);
	static void RemoveTableNameFromOutput(string &inputData);
	//same as before, but now we can couple data together
	static void GetDataPairFromOutput(string &inputData, string colName1, string colName2, vector<dbDataPair> &returnData);
	static void GetAllValuesFromCol(string &inputData, string colName, vector<string> &returnData);
	//specify your fields with a vector, and turn it into a csv, good for easily making long SELECT quereies
	
	static string GetCSVFromVector(vector<string> &fields);

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