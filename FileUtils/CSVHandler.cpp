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

string CSVHandler::GetColValue(std::string colName, int line)
{
	vector<string> tokens = StringUtils::Tokenize(csvEntry[line], inputDelim);
	int i = headerLookup[colName];
	return tokens[i];
}

map<string, string> CSVHandler::GetAllDataFromLine(int line)
{
	map<string, string> ret;
	vector<string> tokens = StringUtils::Tokenize(csvEntry[line], inputDelim);
	map<string, int>::iterator it;

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