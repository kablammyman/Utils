
#include "SQLiteUtils.h"
#include <ios>
#include <iostream>
#include <string>

 
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
	sqlite3_result_text(context, buffer, (int)strlen(buffer),NULL);
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
		sqlite3_result_text(context, buffer, (int)strlen(buffer), NULL);

	//it doesnt matter if this is here or not for this method
	//else
	//	sqlite3_result_null(context);

	//success!!
	//dbCtrlr.executeSQL("SELECT testFunc(id,name) FROM BodyPart where id = 1;", output);
	//success!! it will only return the data that has less than 9 chars...looks like: testFunc2(id,name) | 1~~~Head
	//dbCtrlr.executeSQL("SELECT testFunc2(id,name) FROM BodyPart where testFunc2(id,name) NOT NULL;", output);
	//can i only reutnr special cols now? YES
	//dbCtrlr.executeSQL("SELECT name FROM Gallery where testFunc2(id,name) NOT NULL;", output);
	//what happens with a large table? seems super fast!
	//dbCtrlr.executeSQL("SELECT galleryName FROM Gallery where testFunc2(id,galleryName) NOT NULL;", output);

	//lets add in our own data instead of using query data for params
	//dbCtrlr.executeSQL("SELECT galleryName FROM Gallery where testFunc2('yup',id) NOT NULL;", output);
}
*/
static void hammingDistance(sqlite3_context *context, int argc, sqlite3_value **argv)
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
	db = NULL;
	lastError ="";
	DBName="";
	curTableName="";
	returnData ="";
	gettingData = false;
}

SQLiteUtils::SQLiteUtils(string name)
{
	returnCode = 0;
	lastError = "";
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
	 // Open SQLiteUtils
	DBName = name;
	
	if(name[name.size() -3] != '.' || name[name.size() -2] != 'd' || name[name.size() -1] != 'b')
		name +=".db";

	returnCode = sqlite3_open(name.c_str(), &db);
	//a custom sqlite method test..needs to go right after i open the db
	int numParams = 2;//this var is more for remnding me what things are for
	//sqlite3_create_function(db, "testFunc", numParams, SQLITE_UTF8, NULL, &testFunc, NULL, NULL);
	//sqlite3_create_function(db, "testFunc2", numParams, SQLITE_UTF8, NULL, &testFunc2, NULL, NULL);
	sqlite3_create_function(db, "hammingDistance", 2, SQLITE_UTF8, NULL, &hammingDistance, NULL, NULL);
   //returnCode = sqlite3_open("MyDb.db", &db);
   if (returnCode)
   {
      string  errMsg = sqlite3_errmsg(db);
	  output += ("Error opening SQLite3 SQLiteUtils: " +errMsg + "\n");
      sqlite3_close(db);
	  return false;
   }

   output += ("Opened " + name + "\n");
   lastError.clear();
   return true;
 }
//--------------------------------------------------------------------------------------------------
 void SQLiteUtils::closeSQLiteDB(string &output)
 {
	  // Close SQLiteUtils if its open
	if (db == NULL)
		 return;
   output = ("Attepmting to Close " + DBName + "...");
   sqlite3_close(db);
   output +=  "Closed";
   lastError.clear();
 }
//--------------------------------------------------------------------------------------------------
 string SQLiteUtils::getLastError()
 {
	 return lastError;
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
		//printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		string col = azColName[i];
		string val = "";

		if(argv[i] != NULL)
			val = argv[i];
			
		returnData += (col+ "|" + val+"\n");
	}
	gettingData = false;
	return 0;
}
//--------------------------------------------------------------------------------------------------
bool SQLiteUtils::executeSQL(string command, string &output)
{
	char *error;
	returnData = "";
	returnCode = sqlite3_exec(db, command.c_str(), callback, NULL, &error);
	if (returnCode)
	{
		//output = "Error executing SQLite3 statement: " + sqlite3_errmsg(db);
		output =  sqlite3_errmsg(db);
		lastError = error;
		sqlite3_free(error);
		return false;
	}
	while(gettingData){}//wait until return data is no longer empty
	output = returnData;
	lastError.clear();
	return true;
}
//--------------------------------------------------------------------------------------------------
bool SQLiteUtils::createTable(string name, string query)
{
	setTableName(name);
	string output;
	//string sqlCreateTable = "CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);";
	string sqlCreateTable = "CREATE TABLE "+ curTableName + "(" +query+");";
	return executeSQL(sqlCreateTable,output);
}
//--------------------------------------------------------------------------------------------------
bool SQLiteUtils::insertData(string query) 
{
	string sqlInsert = "INSERT INTO " + curTableName +" VALUES(" + query + ");";
	string output;
	//string sqlInsert = "INSERT INTO MyTable VALUES(NULL, 'A Value');";
	return executeSQL(sqlInsert,output);
}
//--------------------------------------------------------------------------------------------------
vector<string> SQLiteUtils::getAllTables() 
{
	string getTables = "SELECT name FROM sqlite_master WHERE type='table'ORDER BY name;";
	//SELECT name FROM my_db.sqlite_master WHERE type='table';
	//executeSQL(getTables);
	//[x][y] x = tabel name [y] is...??
	
	return query(const_cast<char*>(getTables.c_str()));
}
//--------------------------------------------------------------------------------------------------
vector<string> SQLiteUtils::viewData() 
{
	string getTables = "SELECT value FROM " + curTableName;
	//SELECT name FROM my_db.sqlite_master WHERE type='table';
	//executeSQL(getTables);
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
		lastError = curError;
	else
		lastError.clear();

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
		lastError = curError;
	else
		lastError.clear();

	return results;  
}
int SQLiteUtils::GetLatestRowID()
{
	string output;
	//get the id that was just created from the insert
	executeSQL("SELECT last_insert_rowid()", output);
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
	executeSQL(querey, output);
	return output;
}

string SQLiteUtils::GetDataFromSingleLineOutput(string colName)
{
	std::size_t found = colName.find_last_of("|");
	colName = colName.substr(found + 1);
	if(colName.back() == '\n')
		colName.pop_back();//remove new line char
	return colName;
}

bool SQLiteUtils::doDBQuerey(string data, string &output)
{
	string querey = ("SELECT " + data + " FROM " + curTableName + ";");
	return executeSQL(querey, output);
}

bool SQLiteUtils::doDBQuerey(string data, dbDataPair fromWhere, string &output)
{
	string querey = ("SELECT " + data + " FROM " + curTableName + " WHERE " + fromWhere.first + " = \"" + fromWhere.second + "\";");
	return executeSQL(querey, output);
}

bool SQLiteUtils::doDBQuerey( vector<string> data, string &output)
{
	string querey = "SELECT ";

	for (size_t i = 0; i < data.size(); i++)
	{
		querey += data[i];
		if (i < data.size() - 1)
			querey += ",";
	}
	querey += (" FROM " + curTableName + ";");
	return executeSQL(querey, output);
}
//used for quereies like: SELECT path,CategoryID,WebsiteID FROM Gallery WHERE CategoryID = "3" AND WebsiteID = "16";
bool SQLiteUtils::doDBQuerey( vector<dbDataPair> data, string &output)
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

	return executeSQL(querey + fromString + whereString, output);
}

//used for quereies like: SELECT path FROM Gallery WHERE CategoryID = "3" AND WebsiteID = "16";
bool SQLiteUtils::doDBQuerey(string selectData, vector<dbDataPair> whereData, string &output)
{
	string querey = "SELECT ";
	string whereString = " WHERE ";
	string fromString = (" FROM " + curTableName);


	querey += selectData;

	for (size_t i = 0; i < whereData.size(); i++)
	{
		if (i > 0)
			whereString += " AND ";
		whereString += (whereData[i].first + " = \"" + whereData[i].second + "\"");
	}
	whereString += ";";

	return executeSQL(querey + fromString + whereString, output);
}

//used for quereies like: SELECT ID,path FROM Gallery WHERE CategoryID = "3" AND WebsiteID = "16";
bool SQLiteUtils::doDBQuerey(vector<string> selectData, vector<dbDataPair> whereData, string &output)
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
			whereString += " AND ";
		whereString += (whereData[i].first + " = \"" + whereData[i].second + "\"");
	}
	whereString += ";";

	return executeSQL(querey + fromString + whereString, output);
}

//returns true when successful
bool SQLiteUtils::insertNewDataEntry(dbDataPair data, string &output)
{
	string querey = ("INSERT INTO " + curTableName + " (" + data.first + ")");
	string values = "VALUES ( \"" + data.second + "\");";

	return executeSQL(querey + values, output);

}

//returns true when successful
bool SQLiteUtils::insertNewDataEntry(vector<dbDataPair> data, string &output)
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

	return executeSQL(querey + values, output);

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
	return executeSQL(querey, output);
}

bool SQLiteUtils::UpdateIntEntry(vector<dbDataPair> data, dbDataPair WhereClause, string &output)
{
	//remove quotes
	string querey = GetUpdateQuereyString(data, WhereClause);

	for (size_t i = 0; i < querey.size(); i++)
		if (querey[i] == '\"')
			querey[i] = ' ';
	
	return executeSQL(querey, output);
}
string SQLiteUtils::dataGrabber(string &word, size_t &curPos)
{
	string curWord;
	curPos = word.find('|', curPos);
	curPos++;//move past the pipe and start to collect next word
	while (word[curPos] != '\n')
		curWord += word[curPos++];

	return curWord;
}

void SQLiteUtils::getAllValuesFromCol(string &inputData, string colName, vector<string> &returnData)
{
	bool done = false;
	string curWord;
	size_t c = 0;
	while (!done)
	{
		c = inputData.find(colName, c);
		if (c != string::npos)
			returnData.push_back(dataGrabber(inputData, c));

		else
			done = true;
	}
}

void SQLiteUtils::getDataPairFromOutput(string &inputData, string colName1, string colName2, vector<dbDataPair> &returnData)
{
	bool done = false;
	string firstWord;
	size_t c = 0;
	while (!done)
	{
		c = inputData.find(colName1, c);
		if (c != string::npos)
			firstWord = dataGrabber(inputData, c);

		c = inputData.find(colName2, c);
		if (c != string::npos)
			returnData.push_back(make_pair(firstWord, dataGrabber(inputData, c)));
		else
			done = true;

	}
}

int SQLiteUtils::GetLatestID()
{
	string output;
	//get the id that was just created from the insert
	executeSQL("SELECT last_insert_rowid()", output);
	//othe ouput looks like: last_insert_rowid()|6

	size_t found = output.find_last_of("|");
	string rowID = output.substr(found + 1);
	return atoi(rowID.c_str());
}
