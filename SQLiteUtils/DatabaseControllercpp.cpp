#include "DatabaseController.h"
#include "StringUtils.h"


DatabaseController::DatabaseController()
{
	db = NULL;
	dbName = "";
}
DatabaseController::DatabaseController(string path)
{
	OpenDB(path);
}

DatabaseController::~DatabaseController()
{
	if (IsDBOpen())
	{
		string output;
		db->closeSQLiteDB(output);
	}
}

bool DatabaseController::OpenDB(string path)
{
	//prob should parse the path and seperate that from the name at some point
	//right nopw, path should only eb the name of the db file
	dbName = path;
	db = new SQLiteUtils(path);
	
	string output;

	if (!db->openSQLiteDB(dbName, output))
	{
		printf("couldnt open %s, %s", path.c_str(), output.c_str());
		dbName = "";
		db->closeSQLiteDB(output);
		return false;
	}

	return true;
}

string DatabaseController::GetDBName()
{
	return dbName;
}
bool DatabaseController::IsDBOpen()
{
	return db->IsOpen();
}

bool DatabaseController::ExecuteSQL(string command, string &output)
{
	return db->ExecuteSQL(command, output);
	//curDBWindowData.clear();

	//sendDataToEditWindow(g_hMDIDBView, output);
	//SELECT * FROM images WHERE metaData ="stockings"
	//SELECT FirstName,modelLastName,path FROM images WHERE modelFirstName IS NOT NULL

}

bool DatabaseController::CreateNewDB(string newDBName, string createCommand)
{
	//close the current db before making a new one
	string output;
	if (IsDBOpen())
	{
		db->closeSQLiteDB(output);
		delete db;
		db = NULL;
	}

	//not sure if this is needed...test and remove if this is useless code
	/*FILE * pFile;
	pFile = fopen(newDBName.c_str(), "w");
	fclose(pFile);*/

	//create the new db
	db = new SQLiteUtils(newDBName);

	//create the table, if we have a create command
	if(createCommand != "")
		return db->ExecuteSQL(createCommand, output);

	return true;
}

string DatabaseController::GetLastError()
{
	return db->GetLastError();
}

string DatabaseController::GetTable(string tableName)
{
	string returnString;
	string command = ("SELECT * FROM " + tableName);
	if (!db->ExecuteSQL(command, returnString))
		return "";

	return returnString;
}

bool DatabaseController::DoDBQuerey(string querey, string &output)
{
	return db->ExecuteSQL(querey, output);
}

bool DatabaseController::DoDBQuerey(string table, string data, string &output)
{
	string querey = ("SELECT " + data + " FROM " + table + ";");
	return db->ExecuteSQL(querey, output);
}

bool DatabaseController::DoDBQuerey(string table, string data, dbDataPair fromWhere, string &output)
{
	string querey = ("SELECT " + data + " FROM " + table + " WHERE " + fromWhere.first + " = \"" + fromWhere.second + "\";");
	return db->ExecuteSQL(querey, output);
}

bool DatabaseController::DoDBQuerey(string table, vector<string> data, string &output)
{
	string querey = "SELECT "; 

	querey += StringUtils::FlattenVector(data);

	querey += (" FROM " + table + ";");
	return db->ExecuteSQL(querey, output);
}
//used for quereies like: SELECT path,CategoryID,WebsiteID FROM Gallery WHERE CategoryID = "3" AND WebsiteID = "16";
bool DatabaseController::DoDBQuerey(string table, vector<dbDataPair> data, string &output)
{
	string querey = "SELECT ";
	string whereString = " WHERE ";
	string fromString = (" FROM " + table);

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
			if(numWhereArgs > 1)
				whereString += " AND ";
			whereString += (data[i].first +" = \"" + data[i].second + "\"");
			numWhereArgs++;
		}

		
	}
	whereString += ";";

	return db->ExecuteSQL(querey+ fromString+ whereString, output);
}

//used for quereies like: SELECT path FROM Gallery WHERE CategoryID = "3" AND WebsiteID = "16";
bool DatabaseController::DoDBQuerey(string table, string selectData, vector<dbDataPair> whereData, string &output, bool useAnd)
{
	string querey = "SELECT ";
	string whereString = " WHERE ";
	string fromString = (" FROM " + table);


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

	return db->ExecuteSQL(querey + fromString + whereString, output);
}

bool DatabaseController::DoDBQuerey(string table, vector<string> selectData,  vector<dbDataPair> whereData, string &output, bool useAnd)
{
	string querey = "SELECT ";
	string whereString = " WHERE ";
	string fromString = (" FROM " + table);


	querey += StringUtils::FlattenVector(selectData);

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

	return db->ExecuteSQL(querey + fromString + whereString, output);
}

bool DatabaseController::DoDBQuerey(string table, vector<string> selectData, dbDataPair whereData, string &output)
{
	string querey = "SELECT ";
	string whereString = " WHERE ";
	string fromString = (" FROM " + table);

	//count how many specific items to querey
	//ex select * from table where name ="bla" and age = "bla"; \\2 whereArgs here
	
	for (size_t i = 0; i < selectData.size(); i++)
	{
		if (i > 0)
			querey += ",";

		querey += selectData[i];

	}
	
	whereString += (whereData.first + " = \"" + whereData.second + "\"");
	whereString += ";";

	return db->ExecuteSQL(querey + fromString + whereString, output);
}

bool DatabaseController::DoDBQuerey(string table, vector<string> selectData, string whereString, string &output)
{
	string querey = "SELECT ";
	string fromString = (" FROM " + table);

	//count how many specific items to querey
	//ex select * from table where name ="bla" and age = "bla"; \\2 whereArgs here
	
	for (size_t i = 0; i < selectData.size(); i++)
	{
		if (i > 0)
			querey += ",";

		querey += selectData[i];

	}
	
	whereString += ";";

	return db->ExecuteSQL(querey + fromString + whereString, output);
}

//returns true when successful
bool DatabaseController::InsertNewDataEntry(string table, dbDataPair data, string &output)
{
	string querey = ("INSERT INTO " + table + " (" + data.first + ")");
	string values = "VALUES ( \""+ data.second +  "\");";

	return db->ExecuteSQL(querey + values, output);

}

//returns true when successful
bool DatabaseController::InsertNewDataEntry(string table, vector<dbDataPair> data, string &output)
{
	string querey = ("INSERT INTO " + table + " (");
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

	return db->ExecuteSQL(querey + values, output);

}
//--------------------------------------------------------------------------------------------------------------
bool DatabaseController::UpdateEntry(string table, dbDataPair data, dbDataPair WhereClause, string &output)
{
	string querey = ("UPDATE " + table + " SET ");
	string whereClause(" WHERE " + WhereClause.first + "= \""+ WhereClause.second +"\"");

	querey += (data.first + "= \""+ data.second +"\"");

	return db->ExecuteSQL(querey + whereClause, output);
}
//--------------------------------------------------------------------------------------------------------------
bool DatabaseController::UpdateEntry(string table, vector<dbDataPair> data, dbDataPair WhereClause, string &output)
{

	string querey = ("UPDATE " + table + " SET ");
	string whereClause(" WHERE " + WhereClause.first + "= \""+ WhereClause.second +"\"");

	for (size_t i = 0; i < data.size(); i++)
	{
		querey += (data[i].first + "= \""+ data[i].second +"\"");
		
		if (i < data.size() - 1)
		{
			querey += ",";
		}
	}

	return db->ExecuteSQL(querey + whereClause, output);
}
//--------------------------------------------------------------------------------------------------------------
bool DatabaseController::UpdateEntry(string table, vector<dbDataPair> data, vector<dbDataPair> whereData, string &output, bool useAnd)
{
	string querey = ("UPDATE " + table + " SET ");
	string whereString = " WHERE ";


	for (size_t i = 0; i < data.size(); i++)
	{
		querey += (data[i].first + "= \"" + data[i].second + "\"");

		if (i < data.size() - 1)
		{
			querey += ",";
		}
	}


	for (size_t i = 0; i < whereData.size(); i++)
	{
		if (i > 0)
		{
			if (useAnd)
				whereString += " AND ";
			else
				whereString += " OR ";
		}
		whereString += (whereData[i].first + " = \"" + whereData[i].second + "\"");
	}
	whereString += ";";

	return db->ExecuteSQL(querey + whereString, output);
}
//--------------------------------------------------------------------------------------------------------------
bool DatabaseController::UpdateNumberEntry(string table, dbDataPair data, dbDataPair WhereClause, string& output)
{
	string querey = ("UPDATE " + table + " SET ");
	string whereClause(" WHERE " + WhereClause.first + "= \"" + WhereClause.second + "\"");

	querey += (data.first + " = " + data.first + " + " + data.second );

	return db->ExecuteSQL(querey + whereClause, output);
}
//--------------------------------------------------------------------------------------------------------------
bool DatabaseController::UpdateNumberEntry(string table, vector<dbDataPair> data, dbDataPair WhereClause, string& output)
{

	string querey = ("UPDATE " + table + " SET ");
	string whereClause(" WHERE " + WhereClause.first + "= \"" + WhereClause.second + "\"");

	for (size_t i = 0; i < data.size(); i++)
	{
		querey += (data[i].first + " = " + data[i].first + " + " + data[i].second);

		if (i < data.size() - 1)
		{
			querey += ",";
		}
	}

	return db->ExecuteSQL(querey + whereClause, output);
}
//--------------------------------------------------------------------------------------------------------------
bool DatabaseController::UpdateNumberEntry(string table, vector<dbDataPair> data, vector<dbDataPair> whereData, string& output, bool useAnd)
{
	string querey = ("UPDATE " + table + " SET ");
	string whereString = " WHERE ";


	for (size_t i = 0; i < data.size(); i++)
	{
		querey += (data[i].first + " = " + data[i].first + " + " + data[i].second);

		if (i < data.size() - 1)
		{
			querey += ",";
		}
	}


	for (size_t i = 0; i < whereData.size(); i++)
	{
		if (i > 0)
		{
			if (useAnd)
				whereString += " AND ";
			else
				whereString += " OR ";
		}
		whereString += (whereData[i].first + " = \"" + whereData[i].second + "\"");
	}
	whereString += ";";

	return db->ExecuteSQL(querey + whereString, output);
}
void DatabaseController::ParseDBOutput(string &inputData, int numFields, vector<vector<string>> &returnData)
{
	returnData.clear();
	returnData.resize(numFields, vector<string>(0));//initialise tahe num vectors we need with 1 elements each
	int curField = 0;
	vector<string> tokens = StringUtils::Tokenize(inputData, "|\n");
	//the first is the field name, the second is the value we want
	for (size_t i = 1; i < tokens.size(); i += 2)
	{
		returnData[curField].push_back(tokens[i]);
		if (curField == numFields - 1)
			curField = 0;
		else
			curField++;

	}
}

void DatabaseController::ParseDBOutput(string &inputData, int numFields, vector<DBResult> &returnData)
{
	returnData.clear();
	
	int curField = 0;
	//this used to use tokenize2, but it didnt work for all cases, so ill have to figure it out later
	vector<string> tokens = StringUtils::Tokenize(inputData, "|\n");
	//the first is the field name, the second is the value we want

	size_t i = 0;
	DBResult curRow;
	while ( i < tokens.size())
	{
		if (numFields == 1 && i < tokens.size()-1)
		{
			curRow.insert(tokens[i], tokens[i + 1]);
			returnData.push_back(curRow);
			curRow.clear();
		}
		else if (numFields > 1 && curField < tokens.size() -1 )
		{
			curRow.insert(tokens[i], tokens[i + 1]);
			curField++;
		}
		else
		{
			returnData.push_back(curRow);
			curRow.clear();
			curField = 0;
		}
		i += 2;
	}

	if (curRow.data.size() > 0)
	{
		returnData.push_back(curRow);
	}
}
//this was needed so i can deal with data that has new lines within it
void DatabaseController::ParseDBOutputOLD(string &inputData, vector<string>fields, vector<DBResult> &returnData)
{
	returnData.clear();

	//the first is the field name, the second is the value we want
	size_t i = 0;
	DBResult curRow;
	size_t start = 0;
	size_t curFieldIndex = 0;
	while(i < inputData.size())
	{
		i = inputData.find('|',i);
		if (i!=string::npos)
		{
			string curField = inputData.substr(start,i-start);
			i++;//get cursor off '|'

			//now collect all chars between here and next field name
			if(curFieldIndex < fields.size()-1)
				curFieldIndex++;
			else
				curFieldIndex = 0;

			size_t end = inputData.find(fields[curFieldIndex],i);
			//if we dont fond the next delim, then we prob at the end of the input data
			if(end == string::npos)
				end = inputData.size();	
			size_t tokenLen = end - i;
			string curValue = inputData.substr(i,tokenLen);
			
			//remove trailing new line
			curValue.pop_back();

			curRow.insert(curField,curValue);
			start = end;

			//e just got hte last piece of data for this row, then add to return data, and start the next row
			if(curFieldIndex == 0)
			{
				returnData.push_back(curRow);
				curRow.clear();
			}
		}
		else
			break;
	}
	if (curRow.data.size() > 0)
	{
		returnData.push_back(curRow);
	}
}

void DatabaseController::ParseDBOutput(string &inputData, vector<string>fields, vector<DBResult> &returnData)
{
	returnData.clear();
	size_t found = 0,dataEnd = 0;
	size_t i;
	
	//the first is the field name, the second is the value we want
	DBResult curRow;
	string curColName;
	string curData;
	bool usesCurColName = false;

	while(found != string::npos)
	{
		found = inputData.find("|",found);
		if(found == string::npos)
			break;
		//once we have '|' work backward to see what the col name is
		//we cant rely on the names we passed in since we may only want a fraction of what we got
		
		
		//work backward to find whole col name
		//we also know a col name wont have a new line in it
		i = found;
		while(inputData[i] != '\n' && i != 0)
			i--;
		//ugh, until i make this better, i just gotta move the cursor off the new line
		if(inputData[i] == '\n')
			i++;

		curColName = inputData.substr(i,found-i);
		usesCurColName = false;
		for(size_t k = 0; k < fields.size(); k++)
			if(fields[k] == curColName)
			{
				usesCurColName = true;
				//get teh cursor off of the pipe
				found++;
				//now get the data for this col by finding the next pipe
				dataEnd = inputData.find("|",found);
				if(dataEnd == string::npos)
					dataEnd = inputData.size();

				//get "off" this current col name to find the data we want
				i = dataEnd;
				while(inputData[i] != '\n')
					i--;
				//now i is at the end of the data we want
				//and found+1 is at teh start, so we can take the data and put it in a string 
				curData = inputData.substr(found,i-found);
				
				//if(curData.size() > 0 && curData.back() =='\n')
				//	curData.pop_back();

				//now enter our results into the output vector
				curRow.insert(curColName,curData);
				break;
			}
		//if we are skipping the token we got back, then we need to advance the cursor by 1 so we can move on
		if(!usesCurColName)
			found++;

		//okay, we got all the items for this row, lets start this agian for the next one...lets make sure we are on the last of the feilds we want before getting more
		if(curRow.data.size() == fields.size() && curColName == fields.back())
		{
			returnData.push_back(curRow);
			curRow.clear();
		}
	}	
}

void DatabaseController::ParseDBOutput(string &inputData, string col, vector<DBResult> &returnData)
{
	returnData.clear();
	size_t found = 0, dataEnd = 0;
	size_t i;

	//the first is the field name, the second is the value we want
	DBResult curRow;
	string curColName;
	string curData;
	bool usesCurColName = false;

	while (found != string::npos)
	{
		found = inputData.find("|", found);
		if (found == string::npos)
			break;
		//once we have '|' work backward to see what the col name is
		//we cant rely on the names we passed in since we may only want a fraction of what we got


		//work backward to find whole col name
		//we also know a col name wont have a new line in it
		i = found;
		while (inputData[i] != '\n' && i != 0)
			i--;
		//ugh, until i make this better, i just gotta move the cursor off the new line
		if (inputData[i] == '\n')
			i++;

		curColName = inputData.substr(i, found - i);
		usesCurColName = false;
		
		if (col == curColName)
		{
			usesCurColName = true;
			//get teh cursor off of the pipe
			found++;
			//now get the data for this col by finding the next pipe
			dataEnd = inputData.find("|", found);
			if (dataEnd == string::npos)
				dataEnd = inputData.size();

			//get "off" this current col name to find the data we want
			i = dataEnd;
			while (inputData[i] != '\n')
				i--;
			//now i is at the end of the data we want
			//and found+1 is at teh start, so we can take the data and put it in a string 
			curData = inputData.substr(found, i - found);

			//if(curData.size() > 0 && curData.back() =='\n')
			//	curData.pop_back();

			//now enter our results into the output vector
			curRow.insert(curColName, curData);
		}
		//if we are skipping the token we got back, then we need to advance the cursor by 1 so we can move on
		if (!usesCurColName)
			found++;

		//okay, we got all the items for this row, lets start this agian for the next one...lets make sure we are on the last of the feilds we want before getting more
		if (curColName == col)
		{
			returnData.push_back(curRow);
			curRow.clear();
		}
	}
}

string DatabaseController::DataGrabber(string &word, size_t &curPos)
{
	string curWord;
	curPos = word.find('|', curPos);
	curPos++;//move past the pipe and start to collect next word
	while (word[curPos] != '\n')
		curWord += word[curPos++];

	return curWord;
}

void DatabaseController::GetAllValuesFromCol(string &inputData, string colName, vector<string> &returnData)
{
	bool done = false;
	string curWord;
	size_t c = 0;
	while (!done)
	{
		c = inputData.find(colName,c);
		if (c != string::npos)
			returnData.push_back(DataGrabber(inputData,c));
		
		else
			done = true;
	}
}



void DatabaseController::GetDataPairFromOutput(string &inputData, string colName1, string colName2,vector<dbDataPair> &returnData)
{
	bool done = false;
	string firstWord;
	size_t c = 0;
	while (!done)
	{
		c = inputData.find(colName1,c);
		if (c != string::npos)
			firstWord = DataGrabber(inputData, c);

		c = inputData.find(colName2,c);
		if (c != string::npos)
			returnData.push_back(make_pair(firstWord, DataGrabber(inputData, c)));
		else
			done = true;

	}
}

string DatabaseController::GetCSVFromVector(vector<string> &fields)
{
	string ret = "";
	for(size_t i = 0; i < fields.size(); i++)
	{
		ret += fields[i]+",";
	}
	ret.pop_back();
	return ret;
}

//blah! the code is differnt enough that i cant reuse it woulth more loops... >=(
void DatabaseController::RemoveTableNameFromOutput(string &inputData, int numCols, int colToUse, vector<string> &returnData)
{
	if (colToUse > numCols || colToUse < 1)
		return;

	string delims = "|\n";
	int counter = 1;
	colToUse *= 2;
	
	vector<string> temp = StringUtils::Tokenize(inputData, delims);

	for (size_t i = 0; i < temp.size(); i++)
	{
		if (counter == colToUse)
			returnData.push_back(temp[i]);
		//x2 becasue it goes colName|data, we only want the data, not the col name
		if (counter < (numCols * 2))
			counter++;
		else
			counter = 1;
	}
}
void DatabaseController::RemoveTableNameFromOutput(string &inputData, int numCols, int colToUse, vector<int> &returnData)
{
	if (colToUse > numCols || colToUse < 1)
		return;

	string delims = "|\n";
	int counter = 1;
	colToUse *= 2;
	
	vector<string> temp = StringUtils::Tokenize(inputData, delims);

	for (size_t i = 0; i < temp.size(); i++)
	{
		if (counter == colToUse)
			returnData.push_back(StringUtils::GetIntFromString(temp[i]));
		//x2 becasue it goes colName|data, we only want the data, not the col name
		if (counter < (numCols * 2))
			counter++;
		else
			counter = 1;
	}
}
void DatabaseController::RemoveTableNameFromOutput(string &inputData, int numCols, int colToUse, vector<float> &returnData)
{
	if (colToUse > numCols || colToUse < 1)
		return;

	string delims = "|\n";
	int counter = 1;
	colToUse *= 2;
	
	vector<string> temp = StringUtils::Tokenize(inputData, delims);

	for (size_t i = 0; i < temp.size(); i++)
	{
		if (counter == colToUse)
			returnData.push_back(atof(temp[i].c_str()));
		//x2 becasue it goes colName|data, we only want the data, not the col name
		if (counter < (numCols * 2))
			counter++;
		else
			counter = 1;
	}
}
void DatabaseController::RemoveTableNameFromOutput(string &inputData)
{
	size_t f = inputData.find_first_of('|');
	if(f == string::npos)
		return;
	inputData.erase(0, f+1);

	string ret;
	for (size_t i = 0; i < inputData.size(); i++)
		if (inputData[i] == '\n')
			break;
		else
			ret += inputData[i];

	inputData = ret;

	if(inputData.back() == '\n')
		inputData.pop_back();
}
int DatabaseController::GetIdFromSingleQueryOutput(std::string quereyResult)
{
	if (quereyResult.empty())
		return -1;

	RemoveTableNameFromOutput(quereyResult);

	if (!quereyResult.empty())
		return stoi(quereyResult);
	return -1;
}
void DatabaseController::GetAllTablesInDB(std::vector<std::string> &retData)
{
	db->GetAllTablesInDB(retData);
}

void DatabaseController::GetAllColsInTable(string tableName,std::vector<std::string> &retData)
{
	db->GetAllColsInTable(tableName, retData);
}


void DatabaseController::CreateTable(string tableName, string fields)
{
	db->createTable(tableName, fields);
}

bool DatabaseController::CreateTable(string tableName, vector<dbDataPair> fields, string& output)
{
	string querey = "CREATE TABLE IF NOT EXISTS " + tableName + " (";
	for (size_t i = 0; i < fields.size(); i++)
	{
		if (i > 0)
			querey += ", ";
		querey += "'" + fields[i].first + "' " + fields[i].second;
	}
	querey += ")";
	return db->ExecuteSQL(querey, output);
}

void DatabaseController::AddStringDataToQuerey(vector<dbDataPair> & data, string colName, string colValue)
{
	StringUtils::SanitizeSQLDataString(colValue);
	data.push_back(make_pair(colName,colValue));
}
void DatabaseController::AddFloatDataToQuerey(vector<dbDataPair> & data, string colName, float colValue)
{
	data.push_back(make_pair(colName,to_string(colValue)));
}
void DatabaseController::AddIntDataToQuerey(vector<dbDataPair> & data, string colName, int colValue)
{
	data.push_back(make_pair(colName,to_string(colValue)));
}
void DatabaseController::AddBoolDataToQuerey(vector<dbDataPair> & data, string colName, bool colValue)
{
	int val = 1;
	if(colValue == false)
		val = 0;
	AddIntDataToQuerey(data,  colName,  val);
}

void DatabaseController::AddDateDataToQuerey(vector<dbDataPair> & data, string colName, DateTime colValue)
{
	AddStringDataToQuerey(data,  colName, colValue.ToString());
}

void DatabaseController::AddDateAndTimeDataToQuerey(vector<dbDataPair> & data, string colName, DateTime colValue)
{
	AddStringDataToQuerey(data,  colName, colValue.ToString(true));
}

void DatabaseController::AddTimeDataToQuerey(vector<dbDataPair> & data, string colName, DateTime::Time colValue)
{
	AddStringDataToQuerey(data, colName, colValue.ToString());
}
void DatabaseController::AddDateTimeDataToQuerey(vector<dbDataPair> & data, string colName, DateTime colValue)
{
	string col = colValue.ToString(true);
	AddStringDataToQuerey(data, colName, col);
}

int DatabaseController::GetIdFromQuereyResult(std::string input)
{
	if(input.empty())
		return -1;

	RemoveTableNameFromOutput(input);
	return StringUtils::GetIntFromString(input);
}

int DatabaseController::DuplicateEntry(string table, int id)
{
	std::vector<std::string> fields;
	GetAllColsInTable(table, fields);
	dbDataPair whereData = make_pair("ID",to_string(id));
	string output;

	if(!DoDBQuerey(table, fields,whereData , output))
	{
		return -1;
	}

	vector<DBResult> returnData;
	ParseDBOutputOLD(output, fields, returnData);

	output.clear();
	vector<dbDataPair> dupeData;

	for (size_t i = 0; i < fields.size(); i++)
	{
		if(fields[i] != "ID")
		{
			dbDataPair newPair = make_pair(fields[i],returnData[0].GetValueFromKey(fields[i]));
			dupeData.push_back(newPair);
		}
	}

	if(!InsertNewDataEntry(table, dupeData, output))
	{
		return -1;
	}

	return GetLatestRowID();
}

string DatabaseController::GetIsNullOREmptyString(string fieldName)
{
	return "(" + fieldName + " IS NULL OR " + fieldName + " = \"\")";
}

bool DatabaseController::CheckIfTableExists(string tableName)
{
	string output;
	//TABLE_NAME_TO_CHECK';
	ExecuteSQL("SELECT count(type) from sqlite_master WHERE type='table' and name=\"" + tableName + "\";", output);

	RemoveTableNameFromOutput(output);

	if (output == "0")
		return false;
	return true;

}

bool DatabaseController::AddColToExistingTable(string tableName, string colName, string attrib)
{
	string output;
	//TABLE_NAME_TO_CHECK';
	return ExecuteSQL("ALTER TABLE \""+ tableName+"\" ADD COLUMN \"" + colName +"\" " + "\"" + attrib + "\";", output);


}

std::vector<std::string> DatabaseController::GetMissingCols(std::string tableName, std::vector<DatabaseController::dbDataPair>& masterColList)
{
	std::vector<std::string> ret;
	std::vector<std::string> cols;
	GetAllColsInTable(tableName, cols);
	bool foundCol;
	for (size_t i = 0; i < masterColList.size(); i++)
	{
		foundCol = false;
		for (size_t j = 0; j < cols.size(); j++)
		{
			if (masterColList[i].first == cols[j])
			{
				foundCol = true;
				break;
			}
		}
		if (!foundCol)
			ret.push_back(masterColList[i].first);
	}
	return ret;
}

void DatabaseController::CreateOrUpdateTable(string tableName, vector<DatabaseController::dbDataPair>& fields)
{
	string output;
	if (!CheckIfTableExists(tableName))
		CreateTable(tableName, fields, output);
	else
	{
		std::vector<std::string> missingCols = GetMissingCols(tableName, fields);
		for (size_t i = 0; i < missingCols.size(); i++)
		{
			for (size_t j = 0; j < fields.size(); j++)
			{
				if (fields[j].first == missingCols[i])
				{
					AddColToExistingTable(tableName, fields[j].first, fields[j].second);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////test methods
void DatabaseController::testGetTable()
{
	string result = GetTable("BodyPart");
	printf("%s", result.c_str());

	/*SQLiteDBDataParser temp;
	temp.parseDBOutput(result, 2);*/
}

void DatabaseController::testDBEntry()
{
	string output;
	dbDataPair test1 = make_pair("name", "test");
	InsertNewDataEntry("Category", test1, output);

	vector<dbDataPair> data;

	data.push_back(make_pair("ID", "000"));
	data.push_back(make_pair("WebsiteID", "001"));
	data.push_back(make_pair("SubWebsiteID", "002"));
	data.push_back(make_pair("path", "C:\\some\\fake\\path"));
	data.push_back(make_pair("tags", "test,testData,wow doge"));

	InsertNewDataEntry("Gallery", data, output);


}


void DatabaseController::testDBQuerey()
{
	string output;
	 
	DoDBQuerey("Category", "name", output);

	vector <vector<string>> returnData1; 
	ParseDBOutput(output, 2, returnData1);

	vector<string> test2;
	test2.push_back("WebsiteID");
	test2.push_back("path");
	DoDBQuerey("Gallery", test2, output);

	vector<dbDataPair> data;
	data.push_back(make_pair("ID", "000"));
	data.push_back(make_pair("WebsiteID", "001"));
	data.push_back(make_pair("SubWebsiteID", "002"));
	data.push_back(make_pair("path", "C:\\some\\fake\\path"));

	DoDBQuerey("Gallery", data, output);

	
}