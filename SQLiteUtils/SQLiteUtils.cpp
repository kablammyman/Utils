
#include "SQLiteUtils.h"
#include <ios>
#include <iostream>
#include <string>

//need this for a sleep call
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

 
using namespace std;
string SQLiteUtils::returnData;
bool SQLiteUtils::gettingData;
/*
static void testFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	// check that we have four arguments (lat1, lon1, lat2, lon2)
	//if (argc != 4)
	//	return;
	// check that all four arguments are non-null
	if (sqlite3_value_type(argv[0]) == SQLITE_NULL || sqlite3_value_type(argv[1]) == SQLITE_NULL || sqlite3_value_type(argv[2]) == SQLITE_NULL || sqlite3_value_type(argv[3]) == SQLITE_NULL) {
		sqlite3_result_null(context);
		return;
	}
	// get the four argument values
	double lat1 = sqlite3_value_double(argv[0]);
	double lon1 = sqlite3_value_double(argv[1]);
	double lat2 = sqlite3_value_double(argv[2]);
	double lon2 = sqlite3_value_double(argv[3]);
	char *string1 = (char *)sqlite3_value_text(argv[0]);
	char *string2 = (char *)sqlite3_value_text(argv[1]);
	string ret = string1;
	ret += "~~~";
	ret += string2;
	//cout <<"mine: " << ret << endl;
	char *buffer = new char[ret.size()];
	strcpy(buffer, ret.c_str());
	sqlite3_result_text(context, buffer, (int)strlen(buffer),0);
}

static void testFunc2(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	char *string1 = (char *)sqlite3_value_text(argv[0]);
	char *string2 = (char *)sqlite3_value_text(argv[1]);
	string ret = string1;
	ret += "~~~";
	ret += string2;

	char *buffer = new char[ret.size()];
	strcpy(buffer, ret.c_str());
	//trying to see what i can do to limit what i return, so i picked to only return "small strings"
	if(ret.size() < 9)
		sqlite3_result_text(context, buffer, (int)strlen(buffer), 0);

	//it doesnt matter if this is here or not for this method
	//else
	//	sqlite3_result_null(context);

	//success!!
	//dbCtrlr.ExecuteSQL("SELECT testFunc(id,name) FROM BodyPart where id = 1;", output);
	//success!! it will only return the data that has less than 9 chars...looks like: testFunc2(id,name) | 1~~~Head
	//dbCtrlr.ExecuteSQL("SELECT testFunc2(id,name) FROM BodyPart where testFunc2(id,name) NOT 0;", output);
	//can i only reutnr special cols now? YES
	//dbCtrlr.ExecuteSQL("SELECT name FROM Gallery where testFunc2(id,name) NOT 0;", output);
	//what happens with a large table? seems super fast!
	//dbCtrlr.ExecuteSQL("SELECT galleryName FROM Gallery where testFunc2(id,galleryName) NOT 0;", output);

	//lets add in our own data instead of using query data for params
	//dbCtrlr.ExecuteSQL("SELECT galleryName FROM Gallery where testFunc2('yup',id) NOT 0;", output);
}
*/
static void hammingDistance(sqlite3_context *context,sqlite3_value **argv)
{
	char *string1 = (char *)sqlite3_value_text(argv[0]);
	char *string2 = (char *)sqlite3_value_text(argv[1]);

	int difference = 0;
	for (int i = 0; i<64; i++)
	{
		if (string1[i] != string2[i])
			difference++;
	}
	
	sqlite3_result_int(context, difference);
}

SQLiteUtils::SQLiteUtils()
{
	returnCode = 0;
	db = 0;
	innerLastError ="";
	DBName="";
	curTableName="";
	returnData ="";
	gettingData = false;
}

SQLiteUtils::SQLiteUtils(string name)
{
	returnCode = 0;
	innerLastError = "";
	DBName = "";
	curTableName = "";
	gettingData = false;
	openSQLiteDB(name, returnData);
}
//--------------------------------------------------------------------------------------------------
SQLiteUtils::~SQLiteUtils()
{ 
	string output;
	closeSQLiteDB(output);
}
//--------------------------------------------------------------------------------------------------
 bool SQLiteUtils::openSQLiteDB(string name, string &output)
 {
	DBName = name;
	
	if(name[name.size() -3] != '.' || name[name.size() -2] != 'd' || name[name.size() -1] != 'b')
		name +=".db";

	returnCode = sqlite3_open(name.c_str(), &db);
	//a custom sqlite method test..needs to go right after i open the db
	//int numParams = 2;//this var is more for remnding me what things are for
	//sqlite3_create_function(db, "testFunc", numParams, SQLITE_UTF8, 0, &testFunc, 0, 0);

//i get linker errors with this now...hmm...
//	sqlite3_create_function(db, "hammingDistance", 2, SQLITE_UTF8, 0, &hammingDistance, 0, 0);

   if (returnCode != SQLITE_OK)
   {
      string  errMsg = sqlite3_errmsg(db);
	  output += ("Error opening SQLite3 SQLiteUtils: " +errMsg + "\n");
	  SetLastError(output);
	  sqlite3_close(db);
	  db = nullptr;
	  return false;
   }

   output += ("Opened " + name + "\n");
   innerLastError.clear();
   return true;
 }
//--------------------------------------------------------------------------------------------------
 void SQLiteUtils::closeSQLiteDB(string &output)
 {
	  // Close SQLiteUtils if its open
	if (db == nullptr)
		 return;

	output = ("Attepmting to Close " + DBName + "...");
	sqlite3_close(db);
	db = nullptr;
	output +=  "Closed";
}
 //--------------------------------------------------------------------------------------------------
 bool SQLiteUtils::IsOpen()
 {
	 if (db == nullptr)
		 return false;
	 return true;
 }
//--------------------------------------------------------------------------------------------------
 string SQLiteUtils::GetLastError()
 {
	 return innerLastError;
 }
 //--------------------------------------------------------------------------------------------------
 void SQLiteUtils::SetLastError(string errMsg)
 {
	 innerLastError = errMsg;
 }
//--------------------------------------------------------------------------------------------------
void SQLiteUtils::setTableName(std::string name)
{
	curTableName = name;
}
//--------------------------------------------------------------------------------------------------
int SQLiteUtils::callback(void *data, int argc, char **argv, char **azColName)
{
   //fprintf(stderr, "%s: ", (const char*)data);
	//cheap hack so we wait until all data has been collected
	gettingData = true;
	for(int i=0; i<argc; i++)
	{
		//printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "0");
		string col = azColName[i];
		string val = "";

		if(argv[i] != 0)
			val = argv[i];
			
		returnData += (col+ "|" + val+"\n");
	}
	gettingData = false;
	return 0;
}
//--------------------------------------------------------------------------------------------------
bool SQLiteUtils::ExecuteSQL(string command, string &output)
{
	if (db == nullptr)
	{
		output = "the database hasnt been opened!";
		SetLastError("the database hasnt been opened!");
		return false;
	}
	char *error;
	returnData = "";
	bool good = false;
	for (size_t i = 0; i < 3; i++)
	{
		returnCode = sqlite3_exec(db, command.c_str(), callback, 0, &error);
		
		if (returnCode == SQLITE_BUSY || returnCode == SQLITE_LOCKED)
		{
#ifdef _WIN32
			Sleep(1000);
#else
			sleep(1000);
#endif
			continue;
		}
		else if (returnCode != SQLITE_OK)
		{
			output = sqlite3_errmsg(db);
			SetLastError(error);
			sqlite3_free(error);
			//return false;
		}
		else if (returnCode == SQLITE_OK)
		{
			good = true;
			break;
		}
	}
	if (good)
	{
		while (gettingData) {}//wait until return data is no longer empty
		output = returnData;
		innerLastError.clear();
		return true;
	}
	return false;

}
//--------------------------------------------------------------------------------------------------
bool SQLiteUtils::createTable(string name, string query)
{
	setTableName(name);
	string output;
	//string sqlCreateTable = "CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);";
	string sqlCreateTable = "CREATE TABLE "+ curTableName + "(" +query+");";
	return ExecuteSQL(sqlCreateTable,output);
}
//--------------------------------------------------------------------------------------------------
bool SQLiteUtils::insertData(string query) 
{
	string sqlInsert = "INSERT INTO " + curTableName +" VALUES(" + query + ");";
	string output;
	//string sqlInsert = "INSERT INTO MyTable VALUES(0, 'A Value');";
	return ExecuteSQL(sqlInsert,output);
}
//--------------------------------------------------------------------------------------------------
vector<string> SQLiteUtils::viewData() 
{
	string getTables = "SELECT value FROM " + curTableName;
	//SELECT name FROM my_db.sqlite_master WHERE type='table';
	//ExecuteSQL(getTables);
	//so, it seems every colom in the tbale is returned as a vector
	//ex if my table has time, date, name, addy -> those 4 will be returned...this is one record
	//then each record is put into another vector
	return query(const_cast<char*>(getTables.c_str()));
}
//--------------------------------------------------------------------------------------------------
vector<string> SQLiteUtils::query(char* query)
{
	sqlite3_stmt *statement;
	vector<string>results;

	if(sqlite3_prepare_v2(db, query, -1, &statement, 0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while(true)
		{
			result = sqlite3_step(statement);
			
			if(result == SQLITE_ROW)
			{
				string values;
				for(int col = 0; col < cols; col++)
				{
					values += ((char*)sqlite3_column_text(statement, col));
				}
				results.push_back(values);
			}
			else
			{
				break;   
			}
		}
	   
		sqlite3_finalize(statement);
	}
	
	string curError = sqlite3_errmsg(db);
	if(curError != "not an error") 
		SetLastError(curError);
	else
		innerLastError.clear();

	return results;  
}
//--------------------------------------------------------------------------------------------------
vector<vector<string>> SQLiteUtils::queryV2(char* query)
{
	sqlite3_stmt *statement;
	vector<vector<string> > results;

	if(sqlite3_prepare_v2(db, query, -1, &statement, 0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while(true)
		{
			result = sqlite3_step(statement);
			
			if(result == SQLITE_ROW)
			{
				vector<string> values;
				for(int col = 0; col < cols; col++)
				{
					values.push_back((char*)sqlite3_column_text(statement, col));
				}
				results.push_back(values);
			}
			else
			{
				break;   
			}
		}
	   
		sqlite3_finalize(statement);
	}
	
	string curError = sqlite3_errmsg(db);
	if(curError != "not an error") 
		SetLastError( curError);
	else
		innerLastError.clear();

	return results;  
}
int SQLiteUtils::GetLatestRowID()
{
	string output;
	//get the id that was just created from the insert
	ExecuteSQL("SELECT last_insert_rowid()", output);
	//othe ouput looks like: last_insert_rowid()|6

	size_t found = output.find_last_of("|");
	string rowID = output.substr(found + 1);
	
	return atoi(rowID.c_str());
}

string SQLiteUtils::GetDataFromID(int id, string table)
{
	string output;
	if(table == "")
		table = curTableName;
	string querey = "SELECT * FROM " + table;
	querey += " WHERE ID = ";
	querey += to_string(id);
	ExecuteSQL(querey, output);
	return output;
}

string SQLiteUtils::GetDataFromSingleLineOutput(string colName)
{
	std::size_t found = colName.find_last_of("|");
	colName = colName.substr(found + 1);
	
	if(colName.empty())
		return "";

	if(colName.back() == '\n')
		colName.pop_back();//remove new line char
	return colName;
}

bool SQLiteUtils::DoDBQuerey(string data, string &output)
{
	string querey = ("SELECT " + data + " FROM " + curTableName + ";");
	return ExecuteSQL(querey, output);
}

bool SQLiteUtils::DoDBQuerey(string data, dbDataPair fromWhere, string &output)
{
	string querey = ("SELECT " + data + " FROM " + curTableName + " WHERE " + fromWhere.first + " = \"" + fromWhere.second + "\";");
	return ExecuteSQL(querey, output);
}

bool SQLiteUtils::DoDBQuerey( vector<string> data, string &output)
{
	string querey = "SELECT ";

	for (size_t i = 0; i < data.size(); i++)
	{
		querey += data[i];
		if (i < data.size() - 1)
			querey += ",";
	}
	querey += (" FROM " + curTableName + ";");
	return ExecuteSQL(querey, output);
}
//used for quereies like: SELECT path,CategoryID,WebsiteID FROM Gallery WHERE CategoryID = "3" AND WebsiteID = "16";
bool SQLiteUtils::DoDBQuerey( vector<dbDataPair> data, string &output)
{
	string querey = "SELECT ";
	string whereString = " WHERE ";
	string fromString = (" FROM " + curTableName);

	//count how many specific items to querey
	//ex select * from table where name ="bla" and age = "bla"; \\2 whereArgs here
	int numWhereArgs = 1;

	for (size_t i = 0; i < data.size(); i++)
	{
		if (i > 0)
			querey += ",";


		querey += data[i].first;

		if (!data[i].second.empty())
		{
			if (numWhereArgs > 1)
				whereString += " AND ";
			whereString += (data[i].first + " = \"" + data[i].second + "\"");
			numWhereArgs++;
		}


	}
	whereString += ";";

	return ExecuteSQL(querey + fromString + whereString, output);
}

//used for quereies like: SELECT path FROM Gallery WHERE CategoryID = "3" AND WebsiteID = "16";
bool SQLiteUtils::DoDBQuerey(string selectData, vector<dbDataPair> whereData, string &output, bool useAnd)
{
	string querey = "SELECT ";
	string whereString = " WHERE ";
	string fromString = (" FROM " + curTableName);


	querey += selectData;

	for (size_t i = 0; i < whereData.size(); i++)
	{
		if (i > 0)
		{
			if(useAnd)
				whereString += " AND ";
			else
				whereString += " OR ";
		}
		whereString += (whereData[i].first + " = \"" + whereData[i].second + "\"");
	}
	whereString += ";";

	return ExecuteSQL(querey + fromString + whereString, output);
}

//used for quereies like: SELECT ID,path FROM Gallery WHERE CategoryID = "3" AND WebsiteID = "16";
bool SQLiteUtils::DoDBQuerey(vector<string> selectData, vector<dbDataPair> whereData, string &output, bool useAnd)
{
	string querey = "SELECT ";
	string whereString = " WHERE ";
	string fromString = (" FROM " + curTableName);

	//count how many specific items to querey
	//ex select * from table where name ="bla" and age = "bla"; \\2 whereArgs here

	for (size_t i = 0; i < selectData.size(); i++)
	{
		if (i > 0)
			querey += ",";


		querey += selectData[i];

	}


	for (size_t i = 0; i < whereData.size(); i++)
	{
		if (i > 0)
		{
			if(useAnd)
				whereString += " AND ";
			else
				whereString += " OR ";
		}
		whereString += (whereData[i].first + " = \"" + whereData[i].second + "\"");
	}
	whereString += ";";

	return ExecuteSQL(querey + fromString + whereString, output);
}

//returns true when successful
bool SQLiteUtils::InsertNewDataEntry(dbDataPair data, string &output)
{
	string querey = ("INSERT INTO " + curTableName + " (" + data.first + ")");
	string values = "VALUES ( \"" + data.second + "\");";

	return ExecuteSQL(querey + values, output);

}

//returns true when successful
bool SQLiteUtils::InsertNewDataEntry(vector<dbDataPair> data, string &output)
{
	string querey = ("INSERT INTO " + curTableName + " (");
	string values = "VALUES ( ";
	for (size_t i = 0; i < data.size(); i++)
	{
		querey += data[i].first;
		values += "\"" + data[i].second + "\"";

		if (i < data.size() - 1)
		{
			querey += ",";
			values += ",";
		}
	}
	querey += ")";
	values += ");";

	return ExecuteSQL(querey + values, output);

}

string SQLiteUtils::GetUpdateQuereyString(vector<dbDataPair> data, dbDataPair WhereClause)

{
	string querey = ("UPDATE " + curTableName + " SET ");
	string whereClause(" WHERE " + WhereClause.first + "= \"" + WhereClause.second + "\"");

	for (size_t i = 0; i < data.size(); i++)
	{
		querey += (data[i].first + "= \"" + data[i].second + "\"");

		if (i < data.size() - 1)
		{
			querey += ",";
		}
	}

	
	return querey + whereClause;

}

bool SQLiteUtils::UpdateStringEntry(vector<dbDataPair> data, dbDataPair WhereClause, string &output)
{
	string querey = GetUpdateQuereyString( data,  WhereClause);
	return ExecuteSQL(querey, output);
}

bool SQLiteUtils::UpdateIntEntry(vector<dbDataPair> data, dbDataPair WhereClause, string &output)
{
	//remove quotes, and I dont like replace method
	string querey = GetUpdateQuereyString(data, WhereClause);

	for (size_t i = 0; i < querey.size(); i++)
		if (querey[i] == '\"')
			querey[i] = ' ';
	
	return ExecuteSQL(querey, output);
}
string SQLiteUtils::DataGrabber(string &word, size_t &curPos)
{
	string curWord;
	curPos = word.find('|', curPos);
	curPos++;//move past the pipe and start to collect next word
	while (word[curPos] != '\n')
		curWord += word[curPos++];

	return curWord;
}

void SQLiteUtils::GetAllValuesFromCol(string &inputData, string colName, vector<string> &retData)
{
	bool done = false;
	string curWord;
	size_t c = 0;
	while (!done)
	{
		c = inputData.find(colName, c);
		if (c != string::npos)
			retData.push_back(DataGrabber(inputData, c));

		else
			done = true;
	}
}

void SQLiteUtils::GetDataPairFromOutput(string &inputData, string colName1, string colName2, vector<dbDataPair> &retData)
{
	bool done = false;
	string firstWord;
	size_t c = 0;
	while (!done)
	{
		c = inputData.find(colName1, c);
		if (c != string::npos)
			firstWord = DataGrabber(inputData, c);

		c = inputData.find(colName2, c);
		if (c != string::npos)
			retData.push_back(make_pair(firstWord, DataGrabber(inputData, c)));
		else
			done = true;

	}
}

int SQLiteUtils::GetLatestID()
{
	string output;
	//get the id that was just created from the insert
	ExecuteSQL("SELECT last_insert_rowid()", output);
	//othe ouput looks like: last_insert_rowid()|6

	size_t found = output.find_last_of("|");
	string rowID = output.substr(found + 1);
	return atoi(rowID.c_str());
}
//when you have a giant blob of data that was retunred, break it up into individual results
/*
ID|1
romName|005
ID|2
romName|10yard
ID|3
romName|3wondersh
->data[0] = ID|1 romName|005
->data[1] = ID|2 romName|10yard
->data[2] = ID|3 romName|3wondersh
*/
void SQLiteUtils::SplitDataIntoResults(std::vector<std::string> &retData, string allData, string firstField,bool removeNewline)
{
	size_t i = 0; 
	firstField+='|';
	while(i < allData.size())
	{
		size_t start = allData.find(firstField, i);
		size_t end = allData.find(firstField, start+1);
		end--;//dont include next data 
		string curData = allData.substr(start,(end-start));
		if (removeNewline)
		{
			//dont like string repalce, so i do this
			for (size_t j = 0; j < curData.size(); j++)
				if (curData[j] == '\n')
					curData[j] = ' ';
		}
		retData.push_back(curData);
		i=end;
	}
}

void SQLiteUtils::GetAllTablesInDB(std::vector<std::string> &retData)
{
	string output;
	//string getTables = "SELECT name FROM sqlite_master WHERE type='table'ORDER BY name;";
	ExecuteSQL("SELECT name FROM sqlite_master WHERE type = \"table\"", output);
	GetAllValuesFromCol(output, "name", retData);
}

void SQLiteUtils::GetAllColsInTable(string tableName,std::vector<std::string> &retData)
{
	string output;
	ExecuteSQL("PRAGMA table_info(\""+tableName+"\");", output);
	//vector<string> cols = { "cid","name","type","notnull","dflt_value","pk" };
	GetAllValuesFromCol(output, "name", retData);

}

