#pragma once

#include <string>
#include <utility>      // std::pair
#include <vector>
#include "SQLiteUtils.h"
#include "DateTime.h"
#include "StringUtils.h"

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
//-------------------------------------------------------------
		void GetAllKeys(vector<string> &retVec)
		{
			for (size_t i = 0; i < data.size(); i++)
			{
				retVec.push_back(data[i].first);
			}
		}
//-------------------------------------------------------------
		void GetAllValues(vector<string> &retVec)
		{
			for (size_t i = 0; i < data.size(); i++)
			{
				retVec.push_back(data[i].second);
			}
		}
//-------------------------------------------------------------
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
//-------------------------------------------------------------
		int GetIntValueFromKey(string key)
		{
			for (size_t i = 0; i < data.size(); i++)
			{
				if(data[i].first == key)
				{
					try
					{
						return StringUtils::GetIntFromString(data[i].second);
					}
					catch (...)
					{
						return 0;
					}
				}
			}
			return 0;
		}
//-------------------------------------------------------------
		float GetFloatValueFromKey(string key)
		{
			for (size_t i = 0; i < data.size(); i++)
			{
				if (data[i].first == key)
				{
					try
					{
						return StringUtils::GetFloatFromString(data[i].second);
					}
					catch (...)
					{
						return 0.0f;
					}
				}
			}
			return 0.0f;
		}
//-------------------------------------------------------------
		bool IsEmpty()
		{
			if (data.size() == 0)
				return true;
			return false;
		}
	};
	DatabaseController();
	~DatabaseController();
	struct Error
	{
		int errorCode;
		std::string errorMessage;
	};

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
	bool DoDBQuerey(string table, string selectData, vector<dbDataPair> whereData, string &output, bool useAnd = true );
	
	//select x,y,z from table where dbpair.first = dbpair.second
	bool DoDBQuerey(string table, vector<string> selectData, dbDataPair whereData, string &output);
	
	// SELECT x,y,x FROM Table WHERE dbpair[0].first = dbpair[0].second  AND dbpair[1].first = dbpair[1].second ;
	bool DoDBQuerey(string table, vector<string> selectData, vector<dbDataPair> whereData, string &output, bool useAnd = true);
	//this is good whe we have a multi select, but need to use LIKE instead of =. provide the whole like string, "data like LIKE %someVale%"
	bool DoDBQuerey(string table, vector<string> selectData, string whereString, string &output);
	bool UpdateEntry(string table, dbDataPair data, dbDataPair WhereClause, string &output);
	bool UpdateEntry(string table, vector<dbDataPair> data, dbDataPair WhereClause, string &output);
	bool UpdateEntry(string table, vector<dbDataPair> data, vector<dbDataPair> whereData, string &output, bool useAnd = true);

	bool UpdateNumberEntry(string table, dbDataPair data, dbDataPair WhereClause, string& output);
	bool UpdateNumberEntry(string table, vector<dbDataPair> data, dbDataPair WhereClause, string& output);
	bool UpdateNumberEntry(string table, vector<dbDataPair> data, vector<dbDataPair> whereData, string& output, bool useAnd = true);

	static void ParseDBOutput(string &inputData, int numFields, vector <vector<string>> &returnData);
	static void ParseDBOutput(string &inputData, int numFields, vector<DBResult> &returnData);
	static void ParseDBOutputOLD(string &inputData, vector<string>fields, vector<DBResult> &returnData);
	static void ParseDBOutput(string &inputData, vector<string>fields, vector<DBResult> &returnData);
	static void ParseDBOutput(string &inputData, string col, vector<DBResult> &returnData);

	//num cols is hopw many cols were truthned from the querey, and the colToUse is the data we want to put in the vector
	static void RemoveTableNameFromOutput(string &inputData, int numCols, int colToUse, vector<string> &returnData);
	static void RemoveTableNameFromOutput(string &inputData, int numCols, int colToUse, vector<int> &returnData);
	static void RemoveTableNameFromOutput(string &inputData, int numCols, int colToUse, vector<float> &returnData);
	static void RemoveTableNameFromOutput(string &inputData);
	static int GetIdFromSingleQueryOutput(std::string quereyResult);

	//same as before, but now we can couple data together
	static void GetDataPairFromOutput(string &inputData, string colName1, string colName2, vector<dbDataPair> &returnData);
	static void GetAllValuesFromCol(string &inputData, string colName, vector<string> &returnData);
	//specify your fields with a vector, and turn it into a csv, good for easily making long SELECT quereies
	
	static void AddStringDataToQuerey(vector<dbDataPair> & data, string colName, string valValue);
	static void AddFloatDataToQuerey(vector<dbDataPair> & data, string colName, float valValue);
	static void AddIntDataToQuerey(vector<dbDataPair> & data, string colName, int valValue);
	static void AddBoolDataToQuerey(vector<dbDataPair> & data, string colName, bool valValue);
	static void AddDateDataToQuerey(vector<dbDataPair> & data, string colName, DateTime colValue);

	static string GetCSVFromVector(vector<string> &fields);
	void GetAllTablesInDB(std::vector<std::string> &retData);
	void GetAllColsInTable(string tableName, std::vector<std::string> &retData);
	int DuplicateEntry(string table, int id);
	int GetLatestRowID()
	{
		return db->GetLatestRowID();
	}
	static string GetIsNullOREmptyString(string fieldName);
	//when i need to get an id froma  quere, i do this all the time, so i made this method
	int GetIdFromQuereyResult(std::string input);
	void CreateTable(string tableName, string fields);
	bool CreateTable(string tableName, vector<dbDataPair> fields, string& output);
	//test methods
	void testGetTable();
	void testDBEntry();
	void testDBQuerey();
};