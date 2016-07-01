#ifndef INC_CFGREADER_H
#define INC_CFGREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct cfgOption
{
	std::string optionName;
	std::vector<std::string> optionValueArray;
	std::string optionValue;
	bool isList;
};

class CFGReader
{
	public:
	bool readProfile(std::string file, char delim);
	bool getOptionBooleanValue(std::string optionName);
	int getOptionIntValue(std::string optionName);
	float getOptionFloatValue(std::string optionName);
	std::string getOptionStringValue(std::string optionName);
	std::vector<std::string> getOptionListValue(std::string optionName);
	
	//these methods are to grab individual data from a list we already read in
	bool getOptionListBooleanValue(std::string listName, std::string optionName);
	int getOptionListIntValue(std::string listName, std::string optionName);
	float getOptionListFloatValue(std::string listName, std::string optionName);
	std::string getOptionListStringValue(std::string listName, std::string optionName);
	std::string getOptionListRawString(std::string listName, std::string optionName);
	std::string getRawListData(std::string listName, std::string optionName);
	
	void clearCfgFile();
	std::string getAllStates();

	private:
		char delimiter;
		std::vector<cfgOption> cfgFile;
		cfgOption* getCfgOption(std::string optionName);
		std::string getListData(std::string listName, std::string optionName);
		bool getBooleanData(std::string data);
		int getIntData(std::string data);
		float getFloatData(std::string data);
		std::vector<int> getIntArrayData(std::string data);
		std::vector<float> getFloatArrayData(std::string data);
		std::vector<std::string> getStringArrayData(std::string data);
		//good for getting a particular piece of data from a csv line	
		size_t findInStringArray(std::vector<std::string> data, std::string searchItem);
		std::vector<std::string> getStringTokens(std::string origString, std::string delims);
		std::vector<std::string> getStringTokens(std::string origString, char delim);
		inline std::string trim(std::string& str)
		{
			str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
			str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
			return str;
		}
		inline std::string StringToLower(std::string strToConvert)
		{
			std::string returnString = strToConvert;
			for(size_t i=0; i < strToConvert.size(); i++)
				returnString[i] = tolower(strToConvert[i]);
			return returnString;
		}
};
#endif //INC_CFGREADER_H