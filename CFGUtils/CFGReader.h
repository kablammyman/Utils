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
	bool ReadProfile(std::string file, char delim);
	bool GetOptionBooleanValue(std::string optionName);
	int GetOptionIntValue(std::string optionName);
	float GetOptionFloatValue(std::string optionName);
	std::string GetOptionStringValue(std::string optionName);
	std::vector<std::string> GetOptionListValue(std::string optionName);
	
	//these methods are to grab individual data from a list we already read in
	bool GetOptionListBooleanValue(std::string listName, std::string optionName);
	int GetOptionListIntValue(std::string listName, std::string optionName);
	float GetOptionListFloatValue(std::string listName, std::string optionName);
	std::string GetOptionListStringValue(std::string listName, std::string optionName);
	std::string GetOptionListRawString(std::string listName, std::string optionName);
	std::string GetRawListData(std::string listName, std::string optionName);
	
	void ClearCfgFile();
	std::string GetAllStates();

	private:
		char delimiter;
		std::string cfgFileName;
		std::vector<cfgOption> cfgFile;
		
		cfgOption* GetCfgOption(std::string optionName);
		std::string GetListData(std::string listName, std::string optionName);
		bool GetBooleanData(std::string data);
		int GetIntData(std::string data);
		float GetFloatData(std::string data);
		std::vector<int> GetIntArrayData(std::string data);
		std::vector<float> GetFloatArrayData(std::string data);
		std::vector<std::string> GetStringArrayData(std::string data);
		//good for getting a particular piece of data from a csv line	
		size_t FindInStringArray(std::vector<std::string> data, std::string searchItem);
		std::vector<std::string> GetStringTokens(std::string origString, std::string delims);
		std::vector<std::string> GetStringTokens(std::string origString, char delim);
		inline std::string Trim(std::string& str)
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