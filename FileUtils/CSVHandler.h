#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>

class CSVHandler
{
	std::ifstream inputCSV;
	std::ofstream outputCSV;
	char inputDelim;
	char outputDelim;
	std::vector<std::string> csvHeader;
	std::vector<std::string> csvEntry;
	//when reading in a csv, we want to quickly find the data wwe want without constant for loops
	std::map<std::string, size_t> headerLookup;
	std::vector<std::string> CSVTokenize(std::string line);
public:
	~CSVHandler()
	{
		if (inputCSV.is_open())
			inputCSV.close();
		if (outputCSV.is_open())
			outputCSV.close();
	}
	void ReadCSVFile(std::string file, char d);
	void CreateCSVFile(std::string outputFile, char d);
	std::string GetColValue(std::string colName, size_t line);

	std::map<std::string, std::string> GetAllDataFromLine(size_t line);
	size_t GetCSVLength();

	void AddCSVHeader(std::string header);
	void WriteCSVEntry(std::map<std::string, std::string>& dict, std::vector<std::string>& list);
	
};