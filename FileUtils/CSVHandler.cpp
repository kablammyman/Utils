#include <iostream>

#include "StringUtils.h"
#include "CSVHandler.h"

using namespace std;

void CSVHandler::ReadCSVFile(std::string file, char d)
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
				vector<string> tokens = StringUtils::Tokenize(line, inputDelim);
				for (size_t i = 0; i < tokens.size(); i++)
					headerLookup[tokens[i]] = i;
				isHeader = false;
			}
			else
				csvEntry.push_back(line);
		}
		inputCSV.close();
	}
}

vector<string> CSVHandler::CSVTokenize(string line)
{
	vector<string> tokens;
	bool usesQuotes = false;
	bool insideData = true;


	string curToken = "";
	for (size_t i = 0; i < line.size(); i++)
	{
		if (curToken.size() == 0 && line[i] == '"')
		{
			usesQuotes = true;
			insideData = false;
			continue;
		}


		if (usesQuotes)
		{
			if (line[i] == '"')
			{
				if (!insideData)//start of data
					insideData = true;
			}
			else
			{
				if (line[i] != inputDelim)
					curToken += line[i];
				else
				{
					tokens.push_back(curToken);
					curToken.clear();
					usesQuotes = false;
					insideData = false;
				}
			}
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
void CSVHandler::CreateCSVFile(std::string outputFile, char d)
{
	outputDelim = d;
	outputCSV.open(outputFile, std::ios_base::app);
	string entry = "";
	//create the header for the first line
	for (size_t i = 0; i < csvHeader.size(); i++)
	{
		entry += csvHeader[i] + outputDelim;
	}
	entry.pop_back();//remove last instance of the delimiter
	outputCSV << entry + "\n";
}

void CSVHandler::AddCSVHeader(std::string header)
{
	for (size_t i = 0; i < csvHeader.size(); i++)
		if (csvHeader[i] == header)
			return;

	csvHeader.push_back(header);
}

void CSVHandler::WriteCSVEntry(map<string, string>& dict, vector<string>& list)
{
	string entry;
	string token;
	for (size_t i = 0; i < list.size(); i++)
	{
		token = list[i];//easier to read code
		entry += dict[token] + outputDelim;
	}

	entry.pop_back();

	outputCSV << entry << endl;
}