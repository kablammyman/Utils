#include <iostream>

#include "StringUtils.h"
#include "CSVHandler.h"

using namespace std;

bool CSVHandler::ReadCSVFile(std::string file, char d)
{
	inputDelim = d;
	inputCSV.open(file);
	string line;
	bool isHeader = true;

	if (inputCSV.is_open())
	{
		while (getline(inputCSV, line))
		{
			if (isHeader)
			{
				vector<string> tokens = CSVTokenize(line);
				for (size_t i = 0; i < tokens.size(); i++)
				{
					csvHeader.push_back(tokens[i]);//this preserves the order of the header
					headerLookup[tokens[i]] = i;
				}
				isHeader = false;
			}
			else
				csvEntry.push_back(line);
		}
		inputCSV.close();
		return true;
	}
	return false;
}

vector<string> CSVHandler::GetCSVHeader()
{
	if(!csvHeader.empty())
		return csvHeader;

	map<string, size_t>::iterator it;
	vector<string> ret;
	for (it = headerLookup.begin(); it != headerLookup.end(); it++)
	{
		ret.push_back(it->first);
	}
	return ret;
}



vector<string> CSVHandler::CSVTokenize(string line)
{
	vector<string> tokens;
	bool insideQuotesData = false;

	string curToken = "";
	int i = -1;
	int size = line.size();
	while (i < size)
	{
		i++;
		if (curToken.size() == 0 && line[i] == '"')
		{
			insideQuotesData = true;
			continue;
		}


		if (insideQuotesData)
		{
			if (line[i] == '"')
			{
				tokens.push_back(curToken);
				curToken.clear();
				insideQuotesData = false;
				//this hsould get teh cursor on to the next delim, and the end of the loop will get the cursor onto the next charater for the next token
				i++;
			}
			else
				curToken += line[i];		
		}
//---------------------
		else
		{
			if (line[i] != inputDelim)
				curToken += line[i];
			else
			{
				tokens.push_back(curToken);
				curToken.clear();
			}
		}	
	}
	
	if(!curToken.empty())
		tokens.push_back(curToken);
	
	return tokens;
}

string CSVHandler::GetColValue(std::string colName, size_t line)
{
	//vector<string> tokens = StringUtils::Tokenize(csvEntry[line], inputDelim);
	vector<string> tokens = CSVTokenize(csvEntry[line]);
	size_t i = headerLookup[colName];
	return tokens[i];
}

map<string, string> CSVHandler::GetAllDataFromLine(size_t line)
{
	map<string, string> ret;
	//vector<string> tokens = StringUtils::Tokenize(csvEntry[line], inputDelim);
	vector<string> tokens = CSVTokenize(csvEntry[line]);
	map<string, size_t>::iterator it;

	for (it = headerLookup.begin(); it != headerLookup.end(); it++)
	{
		if (it->second < tokens.size())
			ret[it->first] = tokens[it->second];
	}

	return ret;
}

size_t CSVHandler::GetCSVLength()
{
	return csvEntry.size();
}
///////////////////writing csv files//////////////////////////////
bool CSVHandler::CreateCSVFile(std::string outputFile,vector<string> header, char d, bool overwrite)
{
	outputDelim = d;
	if(overwrite)
		outputCSV.open(outputFile);
	else
		outputCSV.open(outputFile, std::ios_base::app);

	if(!outputCSV.is_open())
		return false;
	string entry = "";
	CreateCSVHeader(header);

	for (size_t i = 0; i < csvHeader.size(); i++)
	{
		entry += csvHeader[i] + d;
	}
	entry.pop_back();
	string line;
	getline(inputCSV, line);

	//only write the header if this is a brand new file
	if(line.empty())
		WriteCSVEntryRaw(entry);

	return true;
}

void CSVHandler::AddCSVHeaderEntry(std::string header)
{
	//make sure this header isnt already int eh list before adding it
	for (size_t i = 0; i < csvHeader.size(); i++)
		if (csvHeader[i] == header)
			return;

	csvHeader.push_back(header);
}

void CSVHandler::CreateCSVHeader(vector<string> headerList)
{
	csvHeader.clear();
	for(size_t i = 0; i <  headerList.size(); i++)
		AddCSVHeaderEntry(headerList[i]);
}

void CSVHandler::WriteCSVEntry(map<string, string>& dict)
{
	string entry = GetCsvEntryString(dict);
	WriteCSVEntryRaw(entry);
}

std::string CSVHandler::GetCsvEntryString(std::map<std::string, std::string>& dict)
{
	string entry;
	string token;
	//this loop will take the header values we have
	//and a map that has a value for each entry.
	//since we may collect these values for the entry in any order, we use the loop to put the reight value under the right header
	for (size_t i = 0; i < csvHeader.size(); i++)
	{
		token = csvHeader[i];//easier to read code
							 //make sure the map we passed in actually has the header entry we want right now
		if (dict.count(token) == 1)
		{
			//if we are using commas as the delimeter, and the entry we have has a comma within in, put it within quotes
			if (outputDelim == ',' && dict[token].find(",") != string::npos)
				entry += "\"" + dict[token] + "\"" + outputDelim;
			else
				entry += dict[token] + outputDelim;
		}
		else
		{
			entry += " " + outputDelim;
		}
	}

	entry.pop_back();
	return entry;
}
//special for click2mail api
std::string CSVHandler::GetCsvEntryStringAsXML(std::map<std::string, std::string>& dict)
{
	string entry;
	string token;

	for (size_t i = 0; i < csvHeader.size(); i++)
	{
		token = csvHeader[i];
		if(dict.count(token) == 1)
		{
			string val = StringUtils::FindAndReplace(dict[token],"&","&amp;");
			entry += "<"+token+">"+val+"</"+token+">\n";
		}
		
	}

	return entry;
}

void CSVHandler::WriteCSVEntryRaw(std::string entry)
{
	outputCSV << entry << endl;
}

std::string CSVHandler::GetCsvHeaderString()
{
	string entry;
	//create the header for the first line
	for (size_t i = 0; i < csvHeader.size(); i++)
	{
		entry += csvHeader[i] + outputDelim;
	}
	entry.pop_back();//remove last instance of the delimiter
	entry += "\n";

	return entry;
}

void CSVHandler::SetDelimeter(char delim)
{
	outputDelim = delim;
}