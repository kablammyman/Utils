#include <fstream>
#include "CFGReader.h"


//--------------------------------------------------------------------------------------------------------------------	
std::vector<std::string> CFGReader::getStringTokens(std::string origString, std::string delims)
	{
		std::vector<std::string> returnVec;

		/*char *p = strtok(origString.c_str(), delims.c_str());
		while (p) 
		{
			std::string newString = p;
			//printf ("Token: %s\n", newString.c_str());
			returnVec.push_back(newString);
			p = strtok(NULL, delims.c_str());
		}*/
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
					 std::string newToken = origString.substr (curPos,found);   // get from "live" to the end
					 newToken = trim(newToken);
					 returnVec.push_back(newToken);
					 curPos = found;
				}
				else//didnt find any more delemiters, so treat rest of std::string as 1 big token
				{
					std::string newToken = origString.substr (curPos);   // get from "live" to the end
					newToken = trim(newToken);
					returnVec.push_back(newToken);
					done = true;
				}
			}while(!done );
		}
		return returnVec;
	}
//--------------------------------------------------------------------------------------------------------------------	
std::vector<std::string> CFGReader::getStringTokens(std::string origString, char delim)
{
	std::string delims;
	delims += delim;
	return getStringTokens(origString, delims);
}
//--------------------------------------------------------------------------------------------------------------------	
bool CFGReader::readProfile(std::string file, char delim)
{
	bool inList = false;
	delimiter = delim;
	std::string line = "";
	int lineNum = 1;

	std::ifstream inputFile (file);
		
	if (inputFile.is_open())
	{
		std::vector<std::string> dataList;//this will hold any collection of items (anythign between { and }) 
		std::string curListName ="";

		while ( getline (inputFile,line) )
		{
			lineNum++;
				
			line = trim(line); //remove all white space before and after
			//look for comments
			if(line.size() > 2)
			{
				if(line[0] == '/' && line[1] == '/')//dont proccess comments
					continue;
				else if(line.find("//")) //we have a comment somewhere else
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
				std::vector<std::string> token = getStringTokens(line, delimiter);
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
cfgOption* CFGReader::getCfgOption(std::string optionName)
{
	//return cfgFile.Find(x => x.optionName == optionName);
	for(size_t i = 0; i < cfgFile.size(); i++)
	{
		if(cfgFile[i].optionName == optionName)
			return &cfgFile[i];
	}
	return NULL;
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::getListData(std::string listName,std::string optionName)
{
	if (getCfgOption(listName) == NULL)
		return "";
	cfgOption* listOption = getCfgOption(listName);
	if(!listOption->isList)
		return "";
	

	std::vector<std::string> listArray = listOption->optionValueArray;//the list data array
	for(size_t i = 0; i < listArray.size(); i++)
	{
		if(listArray[i].find(optionName)!=std::string::npos)//if the keyword is in this std::string
		{
			std::vector<std::string> curOption = getStringTokens(listArray[i], delimiter);
			return curOption[curOption.size()-1];//give us the data only!
		}
	}
	return "";//wasnt found :(
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::getRawListData(std::string listName,std::string optionName)//no delimiter splitting
{
	cfgOption* listOption = getCfgOption(listName);//get the list obj
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
bool CFGReader::getBooleanData(std::string data)
{
	std::string tempString = trim(data);
	tempString = StringToLower(tempString);
	if(tempString == "true")
		return true;
	return false;
}
//--------------------------------------------------------------------------------------------------------------------		
int CFGReader::getIntData(std::string data)
{
	std::string tempString = trim(data);
	int returnData = atoi(tempString.c_str());
	return returnData;
}
//--------------------------------------------------------------------------------------------------------------------		
float CFGReader::getFloatData(std::string data)
{
	std::string tempString = trim(data);
	float returnData = (float)atof(tempString.c_str());
	return returnData;
}
//--------------------------------------------------------------------------------------------------------------------		
std::vector<int> CFGReader::getIntArrayData(std::string data)//good for csv values
{
	std::string tempString = trim(data);
	std::vector<std::string> intValues = getStringTokens(tempString, delimiter);
	std::vector<int>returnArray(intValues.size());
	for(size_t i = 0; i < intValues.size(); i++)
		returnArray[i] = getIntData(intValues[i]);
	return returnArray;
}
//--------------------------------------------------------------------------------------------------------------------		
std::vector<float> CFGReader::getFloatArrayData(std::string data)//good for csv values
{
	std::string tempString = trim(data);
	std::vector<std::string> floatValues = getStringTokens(tempString, delimiter);
	std::vector<float> returnArray(floatValues.size());
	for(size_t i = 0; i < floatValues.size(); i++)
		returnArray[i] = getFloatData(floatValues[i]);
	return returnArray;
}
//--------------------------------------------------------------------------------------------------------------------		
std::vector<std::string> CFGReader::getStringArrayData(std::string data)//good for csv values
{
	std::string tempString = trim(data);
	return getStringTokens(tempString, delimiter);
}
//--------------------------------------------------------------------------------------------------------------------		
size_t CFGReader::findInStringArray(std::vector<std::string> data, std::string searchItem)//good for getting a particular piece of data from a csv line
{
	for(size_t i = 0; i < data.size(); i++)
		if(data[i] == searchItem)
			return i;
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------		
bool CFGReader::getOptionBooleanValue(std::string optionName)
{
	cfgOption* option = getCfgOption(optionName);
	if(option != NULL)
		return getBooleanData(option->optionValue);
	return false;
}
//--------------------------------------------------------------------------------------------------------------------		
int CFGReader::getOptionIntValue(std::string optionName)
{
	cfgOption* option = getCfgOption(optionName);
	if(option != NULL)
		return getIntData(option->optionValue);
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------		
float CFGReader::getOptionFloatValue(std::string optionName)
{
	cfgOption* option = getCfgOption(optionName);
	if(option != NULL)
		return getFloatData(option->optionValue);
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::getOptionStringValue(std::string optionName)
{
	cfgOption* option = getCfgOption(optionName);
	if(option != NULL)
		return option->optionValue;
	return "";
}
//--------------------------------------------------------------------------------------------------------------------		
std::vector<std::string> CFGReader::getOptionListValue(std::string optionName)
{
	cfgOption* option = getCfgOption(optionName);
	if(option == NULL)
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
bool CFGReader::getOptionListBooleanValue(std::string listName, std::string optionName)
{
	std::string value = getListData(listName, optionName);
	if (value == "")
		return false;
	return getBooleanData(value);
}
//--------------------------------------------------------------------------------------------------------------------		
int CFGReader::getOptionListIntValue(std::string listName,std::string optionName)
{
	std::string value = getListData(listName, optionName);
	if (value == "")
		return 0;
	return getIntData(value);
}
//--------------------------------------------------------------------------------------------------------------------		
float CFGReader::getOptionListFloatValue(std::string listName,std::string optionName)
{
	std::string value = getListData(listName, optionName);
	if (value == "")
		return 0;
	return getFloatData(value);
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::getOptionListStringValue(std::string listName,std::string optionName)
{
	return  getListData(listName, optionName);
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::getOptionListRawString(std::string listName,std::string optionName)//no delimiter stuff, just return the raw std::string, good fro csv files
{
	return  getRawListData(listName, optionName);
}
//--------------------------------------------------------------------------------------------------------------------		
void CFGReader::clearCfgFile()
{
	cfgFile.clear();
}
//--------------------------------------------------------------------------------------------------------------------		
std::string CFGReader::getAllStates()
{
	std::string returnString = "";
	for(size_t i = 0; i < cfgFile.size(); i++)
		returnString += cfgFile[i].optionName + "\n";
	return returnString;
}
