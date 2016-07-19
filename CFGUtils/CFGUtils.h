#pragma once

#include <string>
#include <vector>


class CFGUtils
{
			
public:	
	static  bool ReadCfgFile(std::string filePath, char delim);

	static  bool GetCfgBoolValue(std::string optionName);
	static  int GetCfgIntValue(std::string optionName);
	static  float GetCfgFloatValue(std::string optionName);
	static  std::string GetCfgStringValue(std::string optionName);
	static  std::vector<std::string> GetCfgListValue(std::string optionName);

	static  bool GetBoolValueFromList(std::string listName, std::string optionName);
	static  int GetIntValueFromList(std::string listName, std::string optionName);
	static  float GetFloatValueFromList(std::string listName, std::string optionName);
	static  std::string GetStringValueFromList(std::string listName, std::string optionName);

	static  std::string GetRawValueFromList(std::string listName, std::string optionName);//dont parse he csv data


};

