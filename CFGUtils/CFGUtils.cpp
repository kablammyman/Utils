// This is the main DLL file.

#include "CFGUtils.h"
#include <iostream>
#include <fstream>

#include "CFGReader.h"
CFGReader cfgr;//can't use it as a member variable due to the static conflict


bool CFGUtils::ReadCfgFile(std::string filePath, char delim)
{
	return cfgr.ReadProfile(filePath, delim);
}

bool CFGUtils::GetCfgBoolValue(std::string optionName)
{
	return cfgr.GetOptionBooleanValue(optionName);
}
int CFGUtils::GetCfgIntValue(std::string optionName)
{
	return cfgr.GetOptionIntValue(optionName);
}
float CFGUtils::GetCfgFloatValue(std::string optionName)
{
	return cfgr.GetOptionFloatValue(optionName);
}
std::string CFGUtils::GetCfgStringValue(std::string optionName)
{
	std::string returnVal = cfgr.GetOptionStringValue(optionName);
	//if(returnVal == "")
	//	return "could not find option: " +optionName;
	return returnVal;
}
std::vector<std::string> CFGUtils::GetCfgListValue(std::string optionName)
{
	std::vector<std::string> returnVec = cfgr.GetOptionListValue(optionName);
	if(returnVec.empty())
		returnVec.push_back("could not find option: " +optionName);
	return returnVec;
}
bool CFGUtils::GetBoolValueFromList(std::string listName, std::string optionName)
{
	return cfgr.GetOptionListBooleanValue(listName, optionName);
}
int CFGUtils::GetIntValueFromList(std::string listName, std::string optionName)
{
	return cfgr.GetOptionListIntValue(listName,optionName);
}
float CFGUtils::GetFloatValueFromList(std::string listName, std::string optionName)
{
	return cfgr.GetOptionListFloatValue(listName,optionName);
}
std::string CFGUtils::GetStringValueFromList(std::string listName, std::string optionName)
{
	std::string returnVal = cfgr.GetOptionListStringValue(listName,optionName);
	if(returnVal == "")
		return "could not find option: " +optionName;
	return returnVal;
}

std::string CFGUtils::GetRawValueFromList(std::string listName, std::string optionName)//dont parse he csv data
{
	std::string returnVal = cfgr.GetOptionListRawString(listName,optionName);
	if(returnVal == "")
		return "could not find option: " +optionName;
	return returnVal;
}
//need a way to give expected values, with default and data limits
//then we can check that with what we read in
