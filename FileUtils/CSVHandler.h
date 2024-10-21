#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>

class CSVHandler
{
	std::ifstream inputCSV;
	std::ofstream outputCSV;
	//bool saveCSVDataInMem;//not sure if this is needed
	char inputDelim = 0;
	char outputDelim = 0;
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
	std::vector<std::string> GetCSVHeader();
	bool ReadCSVFile(std::string file, char d, bool hasHeader = true);
	bool ReadCSVFile2(std::string file, char d, bool hasHeader = true);
	bool CreateCSVFile(std::string outputFile,std::vector<std::string> header, char d,bool overwrite = false);
	std::string GetColValue(std::string colName, size_t line);

	std::map<std::string, std::string> GetAllDataFromLine(size_t line);
	size_t GetCSVLength();

	void AddCSVHeaderEntry(std::string header);
	void WriteCSVEntry(std::map<std::string, std::string>& dict);
	
	void WriteCSVEntryRaw(std::string entry);
	void CreateCSVHeader(std::vector<std::string> headerList);
	std::string GetCsvEntryString(std::map<std::string, std::string>& dict);
	std::string GetAndStoreCsvEntryString(std::map<std::string, std::string>& dict);
	std::string GetCsvEntryStringAsXML(std::map<std::string, std::string>& dict);//special for click2mail api
	std::string GetCsvHeaderString();
	void SetDelimeter(char delim);
	std::vector<std::string> GetCSVDataAsVector();
	std::vector<std::string> GetCSVLineAsVector(size_t index);
	std::vector<std::string> GetCSVLineAsVector(std::string line);
	void WriteCSVMemToFile();
	bool IsEntryInCSV(std::string headerName, std::string value);
	int GetHeaderIndex(std::string headerName);
	//void SetSaveDataInMemFlag(bool saveInMem);//not sure if this is needed
	void DeleteAllEntriesInMem();
	void DeleteEntry(int index);
};