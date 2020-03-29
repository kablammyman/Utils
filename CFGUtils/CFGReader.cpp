#include <fstream>
#include "CFGReader.h"
#include "StringUtils.h"


//--------------------------------------------------------------------------------------------------------------------	
/*std::vector<std::string> CFGReader::GetStringTokens(std::string origString, std::string delims)
	{
		std::vector<std::string> returnVec;

		size_t found=origString.find(delims);
		if (found== std::string::npos)
			returnVec.push_back(origString);
		else
		{
			//add first token to list
			std::string newToken = origString.substr (0,found); // "generalities"
			returnVec.push_back(newToken);
			bool done = false;
			size_t curPos = found+1;// the plus on is to not include the colon
			//now loop until we have no more tokens
			do{
				found=origString.find(delims,found+1);
				if (found!= std::string::npos)//we have more delims!
				{
					 newToken = origString.substr (curPos,found);   // get from "live" to the end
					 newToken = Trim(newToken);
					 returnVec.push_back(newToken);
					 curPos = found;
				}
				else//didnt find any more delemiters, so treat rest of std::string as 1 big token
				{
					newToken = origString.substr (curPos);   // get from "live" to the end
					newToken = Trim(newToken);
					returnVec.push_back(newToken);
					done = true;
				}
			}while(!done );
		}
		return returnVec;
	}
//--------------------------------------------------------------------------------------------------------------------	
std::vector<std::string> CFGReader::GetStringTokens(std::string origString, char delim)
{
	std::string delims;
	delims += delim;
	return GetStringTokens(origString, delims);
}*/
//--------------------------------------------------------------------------------------------------------------------	
bool CFGReader::ReadProfile(std::string file, char delim)
{
	//its already been opened
	if (cfgFile.size() > 0)
	{
		if (cfgFileName == file)
			return true;
		else
			cfgFile.clear();
	}

	bool inList = false;
	delimiter = delim;
	std::string line = "";
	int lineNum = 1;

	std::ifstream inputFile (file);
		
	if (inputFile.is_open())
	{
		cfgFileName = file;
		std::vector<std::string> dataList;//this will hold any collection of items (anythign between { and }) 
		std::string curListName ="";

		while ( getline (inputFile,line) )
		{
			
			lineNum++;
				
			line = Trim(line); //remove all white space before and after
#ifndef _WIN32 
			//linux bullshit
			if (line.back() == '\r')
				line.pop_back();
#endif
			//look for comments
			if(line.size() > 2)
			{
				if(line[0] == '/' && line[1] == '/')//dont proccess comments
					continue;
				else if(line.find("//") && !line.find("http")) //we have a comment somewhere else AND ITS NOT A URL!
				{
					size_t index = line.find('/'); //this shoudl awlays work since we found it exists alrady with Contains
					if(line[index+1] == '/') //if the next char after the first one is als a '/', ignore the rest
						line = line.substr(0,index); //return everything before the comment
				}
			}
			else 
			{
				if(!inList)//lists can have anything, so we will include all non whitespace chars
					continue; //skip blank lines
			}
			//get list of single items (like a list of strings)		
			//keyword isnt needed, but maybe in the future it will be?
			/*danceAnimations LIST {
				someanimtion1
				anotherAnimation
				aThirdAnimation
				}
				or
				animationState LIST {
				name|someStateName
				animationName|idle
				animationSpeed|5
				movementSpeed|3
				}
			*/
			//if(line.Contains("LIST")) //we have a list of some sort, i guess the keyword isnt needed. we dont support nested lists yet
			//debug code //int ii = line.IndexOf('{');if(ii != -1)
				
			if(line.find('{') != std::string::npos)
			{
				if(inList)//if we are already in a list, ignore the rest. nested list may come later
					continue;
				inList = true;
				dataList.clear();//we will keep any data we collect as a std::string, we can change or parse it later.
				size_t index = line.find('{');
				curListName = line.substr(0,index-1);
				continue; //a line that starts a list should only have the name of the list and the "{" all else will be ignored!
			}
			if(inList)
			{
				if(line.find('}') != std::string::npos) //we are done list collecting data
				{
					inList = false;
					cfgOption newOption;
					newOption.optionName = curListName;
					newOption.optionValueArray = dataList;//dump all collcted data to the datastruct for this item
					newOption.isList = true;
					inList = false;
					cfgFile.push_back(newOption);
					dataList.clear();//clear list
					continue;//again, when we end a list, nothing else should be on this line, cuz it will be ignored
				}
				else
					dataList.push_back(line);
			}
			else //single value option
			{
				cfgOption newOption;
				std::vector<std::string> token = StringUtils::Tokenize(line, delimiter);
				newOption.optionName = token[0];
				newOption.optionValue = token[1];//put that data in there
				newOption.isList = false;
				cfgFile.push_back(newOption);
			}
		}
		
	}
	else
		return false;

	inputFile.close();
	return true;

	//Debug.Log("collected files: " + itemTypes.Count);
	//Debug.Log("num items in " + itemTypes[1].getItemType() +":"+ itemTypes[1].getNumItems());//should be coats
	//catch (Exception e)Debug.LogError("error loading file: "+ file + "\nline number" + lineNum+ "\nError: "+e);

}
//--------------------------------------------------------------------------------------------------------------------			
cfgOption* CFGReader::GetCfgOption(std::string optionName)
{
	//return cfgFile.Find(x => x.optionName == optionName);
	for(size_t i = 0; i < cfgFile.size(); i++)
	{
		if(cfgFile[i].optionName == optionName)
			return &cfgFile[i];
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::GetListData(std::string listName,std::string optionName)
{
	if (GetCfgOption(listName) == 0)
		return "";
	cfgOption* listOption = GetCfgOption(listName);
	if(!listOption->isList)
		return "";
	

	std::vector<std::string> listArray = listOption->optionValueArray;//the list data array
	for(size_t i = 0; i < listArray.size(); i++)
	{
		if(listArray[i].find(optionName)!=std::string::npos)//if the keyword is in this std::string
		{
			std::vector<std::string> curOption = StringUtils::Tokenize(listArray[i], delimiter);
			return curOption[curOption.size()-1];//give us the data only!
		}
	}
	return "";//wasnt found :(
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::GetRawListData(std::string listName,std::string optionName)//no delimiter splitting
{
	cfgOption* listOption = GetCfgOption(listName);//get the list obj
	if(!listOption->isList)
		return "";
	

	std::vector<std::string> listArray = listOption->optionValueArray;//the list data array
	for(size_t i = 0; i < listArray.size(); i++)
	{
		if(listArray[i].find(optionName) != std::string::npos)//if the keyword is in this std::string
			return listArray[i];
	}
	return "";//wasnt found :(
}
//--------------------------------------------------------------------------------------------------------------------			
bool CFGReader::GetBooleanData(std::string data)
{
	std::string tempString = Trim(data);
	tempString = StringToLower(tempString);
	if(tempString == "true")
		return true;
	return false;
}
//--------------------------------------------------------------------------------------------------------------------		
int CFGReader::GetIntData(std::string data)
{
	std::string tempString = Trim(data);
	int returnData = atoi(tempString.c_str());
	return returnData;
}
//--------------------------------------------------------------------------------------------------------------------		
float CFGReader::GetFloatData(std::string data)
{
	std::string tempString = Trim(data);
	float returnData = (float)atof(tempString.c_str());
	return returnData;
}
//--------------------------------------------------------------------------------------------------------------------		
std::vector<int> CFGReader::GetIntArrayData(std::string data)//good for csv values
{
	std::string tempString = Trim(data);
	std::vector<std::string> intValues = StringUtils::Tokenize(tempString, delimiter);
	std::vector<int>returnArray(intValues.size());
	for(size_t i = 0; i < intValues.size(); i++)
		returnArray[i] = GetIntData(intValues[i]);
	return returnArray;
}
//--------------------------------------------------------------------------------------------------------------------		
std::vector<float> CFGReader::GetFloatArrayData(std::string data)//good for csv values
{
	std::string tempString = Trim(data);
	std::vector<std::string> floatValues = StringUtils::Tokenize(tempString, delimiter);
	std::vector<float> returnArray(floatValues.size());
	for(size_t i = 0; i < floatValues.size(); i++)
		returnArray[i] = GetFloatData(floatValues[i]);
	return returnArray;
}
//--------------------------------------------------------------------------------------------------------------------		
std::vector<std::string> CFGReader::GetStringArrayData(std::string data)//good for csv values
{
	std::string tempString = Trim(data);
	return StringUtils::Tokenize(tempString, delimiter);
}
//--------------------------------------------------------------------------------------------------------------------		
size_t CFGReader::FindInStringArray(std::vector<std::string> data, std::string searchItem)//good for getting a particular piece of data from a csv line
{
	for(size_t i = 0; i < data.size(); i++)
		if(data[i] == searchItem)
			return i;
	return std::string::npos;
}
//--------------------------------------------------------------------------------------------------------------------		
bool CFGReader::GetOptionBooleanValue(std::string optionName)
{
	cfgOption* option = GetCfgOption(optionName);
	if(option != 0)
		return GetBooleanData(option->optionValue);
	return false;
}
//--------------------------------------------------------------------------------------------------------------------		
int CFGReader::GetOptionIntValue(std::string optionName)
{
	cfgOption* option = GetCfgOption(optionName);
	if(option != 0)
		return GetIntData(option->optionValue);
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------		
float CFGReader::GetOptionFloatValue(std::string optionName)
{
	cfgOption* option = GetCfgOption(optionName);
	if(option != 0)
		return GetFloatData(option->optionValue);
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::GetOptionStringValue(std::string optionName)
{
	cfgOption* option = GetCfgOption(optionName);
	if(option != 0)
		return option->optionValue;
	return "";
}
//--------------------------------------------------------------------------------------------------------------------		
std::vector<std::string> CFGReader::GetOptionListValue(std::string optionName)
{
	cfgOption* option = GetCfgOption(optionName);
	if(option == 0)
	{
		std::cout << optionName<<" is not in the list!";
		std::vector<std::string> empty;
		return empty;
	}
	if(!option->isList)
	{
		std::vector<std::string> emptyVec;
		return emptyVec;
	}
	return option->optionValueArray;
}
//--------------------------------------------------------------------------------------------------------------------			
//these mothds are to grab individual data from a list we already read in
bool CFGReader::GetOptionListBooleanValue(std::string listName, std::string optionName)
{
	std::string value = GetListData(listName, optionName);
	if (value == "")
		return false;
	return GetBooleanData(value);
}
//--------------------------------------------------------------------------------------------------------------------		
int CFGReader::GetOptionListIntValue(std::string listName,std::string optionName)
{
	std::string value = GetListData(listName, optionName);
	if (value == "")
		return 0;
	return GetIntData(value);
}
//--------------------------------------------------------------------------------------------------------------------		
float CFGReader::GetOptionListFloatValue(std::string listName,std::string optionName)
{
	std::string value = GetListData(listName, optionName);
	if (value == "")
		return 0;
	return GetFloatData(value);
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::GetOptionListStringValue(std::string listName,std::string optionName)
{
	return  GetListData(listName, optionName);
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::GetOptionListRawString(std::string listName,std::string optionName)//no delimiter stuff, just return the raw std::string, good fro csv files
{
	return  GetRawListData(listName, optionName);
}
//--------------------------------------------------------------------------------------------------------------------		
void CFGReader::ClearCfgFile()
{
	cfgFile.clear();
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::GetAllStates()
{
	std::string returnString = "";
	for(size_t i = 0; i < cfgFile.size(); i++)
		returnString += cfgFile[i].optionName + "\n";
	return returnString;
}
