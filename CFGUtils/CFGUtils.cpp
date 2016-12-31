// This is the main DLL file.

#include "CFGUtils.h"
#include <windows.h>
#include <iostream>
#include <fstream>

#include "CFGReader.h"
CFGReader cfgr;//can't use it as a member variable due to the static conflict


bool CFGUtils::ReadCfgFile(std::string filePath, char delim)
{
	return cfgr.readProfile(filePath, delim);
}

bool CFGUtils::GetCfgBoolValue(std::string optionName)
{
	return cfgr.getOptionBooleanValue(optionName);
}
int CFGUtils::GetCfgIntValue(std::string optionName)
{
	return cfgr.getOptionIntValue(optionName);
}
float CFGUtils::GetCfgFloatValue(std::string optionName)
{
	return cfgr.getOptionFloatValue(optionName);
}
std::string CFGUtils::GetCfgStringValue(std::string optionName)
{
	std::string returnVal = cfgr.getOptionStringValue(optionName);
	//if(returnVal == "")
	//	return "could not find option: " +optionName;
	return returnVal;
}
std::vector<std::string> CFGUtils::GetCfgListValue(std::string optionName)
{
	std::vector<std::string> returnVec = cfgr.getOptionListValue(optionName);
	if(returnVec.empty())
		returnVec.push_back("could not find option: " +optionName);
	return returnVec;
}
bool CFGUtils::GetBoolValueFromList(std::string listName, std::string optionName)
{
	return cfgr.getOptionListBooleanValue(listName, optionName);
}
int CFGUtils::GetIntValueFromList(std::string listName, std::string optionName)
{
	return cfgr.getOptionListIntValue(listName,optionName);
}
float CFGUtils::GetFloatValueFromList(std::string listName, std::string optionName)
{
	return cfgr.getOptionListFloatValue(listName,optionName);
}
std::string CFGUtils::GetStringValueFromList(std::string listName, std::string optionName)
{
	std::string returnVal = cfgr.getOptionListStringValue(listName,optionName);
	if(returnVal == "")
		return "could not find option: " +optionName;
	return returnVal;
}

std::string CFGUtils::GetRawValueFromList(std::string listName, std::string optionName)//dont parse he csv data
{
	std::string returnVal = cfgr.getOptionListRawString(listName,optionName);
	if(returnVal == "")
		return "could not find option: " +optionName;
	return returnVal;
}
//need a way to give expected values, with default and data limits
//then we can check that with what we read in
