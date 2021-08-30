#include "StringUtils.h"
#include <ctime>//for time  in random string method
#ifndef _WIN32
#include <cstring> //strtok_r
#endif
std::vector<std::string> StringUtils::Tokenize(std::string path, char delim)
{
	std::string delims(1, delim);
	return Tokenize(path, delims);
}
std::vector<std::string> StringUtils::Tokenize(std::string path, std::string delims)
{
	std::vector<std::string> returnVec;
	char *next_token = 0;
#ifdef _WIN32
	char *p = strtok_s(const_cast<char *>(path.c_str()), delims.c_str(), &next_token);
#else
	char *p = strtok_r(const_cast<char *>(path.c_str()), delims.c_str(), &next_token);
#endif
	while (p)
	{
		//printf ("Token: %s\n", p);
		returnVec.push_back(p);
#ifdef _WIN32
		p = strtok_s(0, delims.c_str(), &next_token);
#else
		p = strtok_r(0, delims.c_str(), &next_token);
#endif
	}
	return returnVec;
}
//tokenize 2 use a muklti char delimiter instead of usng many single char delims
std::vector<std::string> StringUtils::Tokenize2(std::string path, std::string delim)
{
	std::vector<std::string> returnVec;
	size_t start = 0;
	size_t i = 0; 
	while(i < path.size())
	{
		i = path.find(delim,start);
		if (i!=std::string::npos)
		{
			std::string temp = path.substr (start,i-start);
			returnVec.push_back(temp);
			start = i+delim.size();
		}
		//if we got here, maybe we got to the endof the string, if sol give whats left
		else
		{
			if (start < path.size())
			{
				std::string temp = path.substr (start);
				returnVec.push_back(temp);
				start = i+delim.size();
			}
			break;
		}
	}
	return returnVec;
}

//turns words or sentences into all proper nouns
void StringUtils::ToProperNoun(std::string &input)
{
	bool caps = true;
	for (size_t i = 0; i < input.size(); i++)
	{

		if (input[i] == ' ' || input[i] == '-' || input[i] == '_')
		{
			caps = true;
			continue;
		}
		if (caps)
		{
			input[i] = toupper(input[i]);
			caps = false;
		}
		else
			input[i] = tolower(input[i]);
	}
}

void StringUtils::ToUpper(std::string &input)
{
	for (size_t i = 0; i < input.size(); i++)
	{
		input[i] = toupper(input[i]);
	}
}

void StringUtils::ToLower(std::string &input)
{
	for (size_t i = 0; i < input.size(); i++)
	{
		input[i] = tolower(input[i]);
	}
}

std::string StringUtils::FlattenVector(std::vector<std::string> input)
{
	std::string returnString;

	for (size_t i = 0; i < input.size(); i++)
		returnString += (input[i] + ",");

	if(returnString.size() > 0)
		returnString.pop_back();//c++11,for older c++, use returnString.erase(returnString.size() - 1);

	return returnString;
}

void StringUtils::TrimWhiteSpace(std::string &input)
{
	if(input.size() == 0)
		return;

	input.erase(0, input.find_first_not_of(' '));       //prefixing spaces
	input.erase(input.find_last_not_of(' ')+1);         //surfixing spaces
}

std::string StringUtils::GetDataBetweenChars(std::string line, char char1, char char2, size_t &start)
{
	size_t dataStart = line.find(char1,start);
	if (dataStart == std::string::npos)
	{
		start = line.size();
		return "";
	}
	std::string returnString = "";

	//i like loops better thans substring for this...its easier to understand
	for (size_t i = dataStart+1; i < line.size(); i++)
	{
		//update the index, so we can do this again on the same line if nesc
		start = i;
		if(line[i] != char2)
			returnString+=line[i];
		else break;
	}
	return returnString;
}

std::string StringUtils::GetDataBetweenSubStrings(std::string line, std::string str1, std::string str2, size_t start)
{
	size_t dataStart = line.find(str1, start);
	if (dataStart == std::string::npos)
		return "";
	dataStart += str1.size();

	size_t dataEnd = line.find(str2, dataStart);
	if (dataStart == std::string::npos)
		dataEnd = line.size();

	std::string returnString = "";

	//i like loops better thans substring for this...its easier to understand
	for (size_t i = dataStart; i < dataEnd; i++)
	{		
		returnString += line[i];
	}
	return returnString;
}

int StringUtils::GetStandAloneWordInLineIndex(std::string line,std::string word)
{
	size_t index = 0;
			
	while (index < line.size())
	{
		size_t start = line.find(word,index);
		//the word is not even in here!
		if (start == std::string::npos)
			return -1;
		index += start +1;
		
		size_t end = start + word.size()-1;
		bool startIsFirst = false;
		bool endIsLast = false;
		
		if (start == 0)
			startIsFirst = true;
		else if( start > 0)
			start--;
		else //the word is not in here at all!
			return -1;
			
		if (end >= line.size()-1)
		{
			endIsLast = true;
			end = line.size()-1;
		}
		else
			end++;
				
		if(	!startIsFirst && endIsLast)
		{
			if (! isalpha(line[start]))
				return (int)start;
		}
		else if( startIsFirst && endIsLast)
			return (int)start;
			
		else if( startIsFirst && !endIsLast)
		{
			if (! isalpha(line[end]))
				return (int)start;
		}
		else
		{
			//why am i doing this if statement???
		//	if (! isalpha(line[start]) && !isalpha(line[end]))							
				return (int)start;
		}
	}
	return -1;
}

bool StringUtils::IsWordFromListInLine(std::vector<std::string> &wordList,std::string line, bool isCaseSensitive)
{
	if (!isCaseSensitive)
		ToUpper(line);
	for(size_t i = 0; i < wordList.size(); i++)
	{
		std::string upperWord = wordList[i];
		if (!isCaseSensitive)
			ToUpper(upperWord);
		if (line.find(upperWord) != std::string::npos)
			if (GetStandAloneWordInLineIndex(line, upperWord) > -1)
				return true;
	}
	return false;
}

std::string StringUtils::GetWordFromListInLine(std::vector<std::string> &wordList,std::string line, bool isCaseSensitive)
{
	if(!isCaseSensitive)
		ToUpper(line);

	for (size_t i = 0; i < wordList.size(); i++)
	{
		std::string curWord = wordList[i];
		if (!isCaseSensitive)
			ToUpper(curWord);
		if(line.find(curWord) != std::string::npos )
			if( GetStandAloneWordInLineIndex(line, curWord) > -1)
				return wordList[i];
	}
	return "";
}

int StringUtils::GetIndexOfWordFromListInLine(std::vector<std::string> &wordList,std::string line, bool isCaseSensitive)
{
	if (!isCaseSensitive)
		ToUpper(line);
	for (size_t i = 0; i < wordList.size(); i++)
	{
		std::string curWord = wordList[i];
		if (!isCaseSensitive)
			ToUpper(curWord);

		if(line.find(curWord) != std::string::npos )
		{
			int index = GetStandAloneWordInLineIndex(line, curWord);
			if ( index > -1)
				return index;
		}
	}
	return -1;
}

int StringUtils::GetIndexOfWordFromList(std::vector<std::string>& wordList, std::string line, bool isCaseSensitive)
{
	if (!isCaseSensitive)
		ToUpper(line);
	for (size_t i = 0; i < wordList.size(); i++)
	{
		std::string curWord = wordList[i];
		if (!isCaseSensitive)
			ToUpper(curWord);

		if (line.find(curWord) != std::string::npos)
		{
			int index = GetStandAloneWordInLineIndex(line, curWord);
			if (index > -1)
				return i;
		}
	}
	return -1;
}

std::string StringUtils::CopyCharsBetweenQuotes(std::string word, size_t index)
{
	std::string retString;
	index = word.find('"',index);
	
	if(index == std::string::npos)
		return "";

	index++;
	
	while (index < word.size())
	{
		if (word[index] == '"')
			break;
		retString += word[index];
		index++;
	}
	return retString;
}
std::string StringUtils::CopyCharsBetweenTags(std::string word, char open,char closed,size_t index)
{
	std::string retString;
	index = word.find(open,index);
	
	if(index == std::string::npos)
		return "";

	index++;
	
	while (index < word.size())
	{
		if (word[index] == closed)
			break;
		retString += word[index];
		index++;
	}
	return retString;
}
bool StringUtils::IsPrevCharNonAlpha(std::string line,std::string word)
{
	size_t start = line.find(word);
	if (start != std::string::npos)
		if (! isalpha(line[start-1]))
			return true;
	return false;
}
bool StringUtils::IsNumber(std::string word)
{
	for (size_t i = 0; i < word.size(); i++)
		//should I include the $ or %?
		if (!isdigit(word[i]) && word[i] != '.' && word[i] != '-' && word[i] != '$')
			return false;
	return true;
}

bool StringUtils::IsPostCharNonAlpha(std::string line,std::string word)
{
	size_t start = line.find(word);
	size_t end = start + word.size();
	
	if (! isalpha(line[end + 1]))
		return true;
	return false;
}
//copy all apha numeric values until the pattern is broken by puncation or space\tab
//bonus chars are teh non alpha numeric chars we want to include as well
std::string StringUtils::GetAlphaNumericString(std::string line,int index,std::string bonusChars)
{
	std::string ret = "";
	for(size_t i = index; i < line.size();i++)
	{
		if( isalpha(line[i]) ||  isdigit(line[i]))
		{
			ret += line[i];
		}
		else if(bonusChars.size() > 0)
		{
			for(size_t j = 0; j < bonusChars.size(); j++)
			{
				if(bonusChars[j] == line[i])
				{
					ret += line[i];
					break;
				}
			}
		}
		else
			break;
	}
	return ret;
}
std::string StringUtils::CopyUntilChar(std::string line,int index, char delim)
{
	std::string ret = "";
	for(size_t i = index; i < line.size();i++)
	{
		if(line[i] != delim)
			ret += line[i];
		else
			break;
	}
	return ret;
}
std::string  StringUtils::GetRandomString(int size)
{
	std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
	std::string ret = "";
	srand(0);
	for(int i = 0; i < size; i++)
	{
		int curIndex = rand()%chars.size()-1;
		ret.push_back(chars[curIndex]);
	}

	return ret;
}

std::string StringUtils::FindAndReplace(std::string orig, std::string findToken, std::string replaceToken)
{
	std::string ret = "";
	size_t i = 0;
	size_t index = orig.find(findToken);
	
	if(index == std::string::npos)
		return orig;
	
	while( i < orig.size())
	{
		index = orig.find(findToken,i);
		if(index != std::string::npos)
		{
			while(i < index)
			{
				ret += orig[i];
				i++;
			}
			i = index + findToken.size();
			ret += replaceToken;
		}
		else
		{
			ret += orig[i];
			i++;
		}
				
	}
	return ret;
}

std::string StringUtils::StringClean(std::string orig, bool includeNewLines = true)
{
	orig = FindAndReplace(orig, "&amp;", "&");
	orig = FindAndReplace(orig, "&nbsp;", "");
		//some names has a % in front...it NOT a typo or coding mistake they make, its short hand for "care of"
	orig = FindAndReplace(orig, "%", "");
	if (includeNewLines)
	{
		orig = FindAndReplace(orig, "\n", " ");
		orig = FindAndReplace(orig, "\r", " ");
	}
	orig = FindAndReplace(orig, "\t", " ");

	//now get rid of html tags
	size_t found = orig.find("<");
	while (found != std::string::npos)
	{
		std::string curTag = "<"+ CopyCharsBetweenTags(orig, '<', '>', found) + ">";
		orig = FindAndReplace(orig, curTag, "");
		found = orig.find("<",found+1);
	}

	TrimWhiteSpace(orig);
	//print "done: " + line
	return orig;
}
void StringUtils::SanitizeSQLDataString(std::string& value)
{
	std::string ret = "";
	//std::string correct = "lineone\\nline2";
	for (size_t i = 0; i < value.size(); i++)
	{
		if (value[i] == '"')
		{
			ret += "'";
		}
		else if (value[i] == '\r\n')
		{
			ret += "\n";
		}
		else
			ret += value[i];
	}
	value = ret;
}


void StringUtils::SanitizeJsonString(std::string& value)
{
	std::string ret = "";
	//std::string correct = "lineone\\nline2";
	for (size_t i = 0; i < value.size(); i++)
	{
		if (value[i] == '\n')
		{
			ret += "\\n";
		}
		else if (value[i] == '\r')
		{
			ret += "\\r";
		}
		else if (value[i] == '"')
		{
			ret += '\'';
		}
		else if (value[i] == '\\')
		{
			ret += "\\\\";
		}
		else
			ret += value[i];

	}
	value = ret;
}

void StringUtils::DesanitizeJsonString(std::string& value)
{
	std::string ret = "";
	size_t size = value.size();
	size_t  i = 0;
	while (i < size)
	{
		if (i < size - 2)
		{
			if (value[i] == '\\')
			{
				if (value[i + 1] == 'n')
				{
					ret += '\n';
					i += 2;
					continue;
				}
				else if (value[i + 1] == 'r')
				{
					ret += '\r';
					i += 2;
					continue;
				}
				else if (value[i + 1] == '"')
				{
					ret += '\'';
					i += 2;
					continue;
				}
			}
		}
		else if (i < size - 3)
		{
			if (value[i] == '\\' && value[i + 1] == '\\' && value[i + 2] == '\\')
			{
				ret += '\\';
				ret += '\\';
				i += 3;
				continue;
			}
		}


		ret += value[i];
		i++;
	}
	value = ret;
}
std::string StringUtils::CreateJsonEntry(std::string name, std::string value, bool noQuotes,bool lastEntry)
{
	std::string ret = "\"";
	ret += name + "\" : ";

	if (noQuotes)
		ret += value;
	else
	{
		SanitizeJsonString(value);
		ret += "\"" + value + "\"";
	}
	if(lastEntry)
		return ret +"\n";
	return ret +",\n";
}



std::string StringUtils::CreateJsonEntry(std::string name, int val,bool lastEntry)
{
	return CreateJsonEntry( name, std::to_string(val),true,lastEntry);
}

std::string StringUtils::CreateJsonEntry(std::string name, float val,bool lastEntry)
{
	return CreateJsonEntry( name, std::to_string(val),true,lastEntry);
}

std::string StringUtils::CreateJsonEntry(std::string name, bool val,bool lastEntry)
{
	std::string t = "true";
	if(!val)
		t = "false";
	return CreateJsonEntry( name, t,true,lastEntry);
}
std::string StringUtils::CreateJsonEntry(std::string name, std::string val,bool lastEntry)
{
	return CreateJsonEntry( name, val,false,lastEntry);
}

std::string StringUtils::CreateJsonEntry(std::string name, const char* val, bool lastEntry)
{
	std::string value = val;
	return CreateJsonEntry( name, value,false,lastEntry);
}

std::string StringUtils::CreateJsonArrayEntry(std::string name, std::vector<std::string> value, bool noQuotes)
{
	std::string ret = "\"";
	ret += name + "\" : [";
	for (size_t i = 0; i < value.size(); i++)
	{
		if (noQuotes)
			ret += value[i] + ",";
		else
		{
			SanitizeJsonString(value[i]);
			ret += "\"" + value[i] + "\",";
		}
	}
	ret += "]";
	size_t found = ret.find_last_of(",");
	if (found != std::string::npos)
		ret.replace(found,1,""); 

	return ret;
}

std::string StringUtils::CreateJsonArrayEntry(std::string name, std::vector<int> value)
{
	std::string ret = "\"";
	ret += name + "\" : [";
	for (size_t i = 0; i < value.size(); i++)
	{
		ret += std::to_string(value[i]) + ",";
	}

	ret += "]";
	//remove last comma
	size_t found = ret.find_last_of(",");
	if (found != std::string::npos)
		ret.replace(found,1,""); 

	return ret;
}

std::string StringUtils::ConvVecToJsonObj(std::vector<std::string> &values)
{
	std::string ret = "{\n";
	bool isEnd = false;
	for (size_t i = 0; i < values.size(); i++)
	{
		if (i == values.size() - 1)
			isEnd = true;
		ret += CreateJsonEntry("item" + std::to_string(i + 1), values[i], isEnd);
	}

	ret += "}\n";
	return ret;
}


std::string StringUtils::GetJsonEntryValue(std::string& json, std::string name)
{
	std::string ret;
	char quoteType = '"';
	std::string key = "\"" + name + "\"";
	size_t start = json.find(key);


	if (start == std::string::npos)
	{
		quoteType = '\'';
		key = quoteType + name + quoteType;
		start = json.find(key);

		if (start == std::string::npos)
			return ret;
	}


	//mopve the cursor off the key, and onto the value
	start = json.find(":", start) + 1;
	//first check if this is a std::string value
	size_t quote = json.find("\"", start);
	size_t comma = json.find(",", start);

	size_t brace = json.find("}", start);
	if (quote < comma)//we have a std::string valua, aka wrapped in quotes
	{
		//if this is true, then find the comma that singals the end of tis line int he json,a nd not a comma inside the quotes
		size_t endQuote = json.find("\"", quote + 1);
		comma = json.find(",", endQuote);
	}

	//prob bad json formatting
	if (comma == std::string::npos && brace == std::string::npos)
		return ret;

	size_t end = (comma < brace) ? comma : brace;

	//cop the chars to the output, fuck substr
	for (size_t i = start; i < end; i++)
	{
		if (json[i] != quoteType)
			ret += json[i];
	}
	StringUtils::TrimWhiteSpace(ret);
	DesanitizeJsonString(ret);
	return ret;
}

int StringUtils::GetJsonEntryIntValue(std::string& json, std::string name)
{
	std::string value = GetJsonEntryValue(json, name);
	if(value == "null" || value == "NULL")
		return 0;
	if (!value.empty())
		return GetIntFromString(value);
	return 0;
}

float StringUtils::GetJsonEntryFloatValue(std::string& json, std::string name)
{
	std::string value = GetJsonEntryValue(json, name);
	if(value == "null" || value == "NULL")
		return 0;
	if (!value.empty())
		return GetFloatFromString(value);
	return 0;
}

bool StringUtils::GetJsonEntryBoolValue(std::string& json, std::string name)
{
	std::string value = GetJsonEntryValue(json, name);
	if (value == "null" || value == "NULL")
		return false;
	if (!value.empty())
	{
		ToLower(value);
		if (value == "true" || value == "yes" || value == "1" || value == "on")
			return true;
	}
	return false;
}


std::string StringUtils::CreateJsonNestedObjectEntry(std::string name, std::vector<std::pair <std::string, std::string>> entries, bool noQuotes)
{
	std::string ret;
	if (!name.empty())
	{
		ret += "\"" + name + "\" : {";
	}
	else
		ret += "{";
	for (size_t i = 0; i < entries.size(); i++)
	{
		ret += +"\"" + entries[i].first + "\":";
		if (noQuotes)
			ret += entries[i].second + ",";
		else
		{
			SanitizeJsonString(entries[i].second);
			ret += "\"" + entries[i].second + "\",";
		}
	}
	size_t end = ret.find_last_of(",");
	ret.erase(end, 1);//erase the last comma, buyt leave the new line char
	ret += "}";

	return ret;
}

float StringUtils::GetFloatFromString(std::string str)
{
	if(str.empty())
		return 0.0;
	
	size_t f = str.find(",");
	if (f != std::string::npos)
		str.erase(f, 1);
	f = str.find("$");
	if (f != std::string::npos)
		str.erase(f, 1);

	if(IsNumber(str))
		return stof(str);

	return 0.0;
}

int StringUtils::GetIntFromString(std::string str)
{
	if(str.empty())
		return 0;

	size_t f = str.find(",");
	if (f != std::string::npos)
		str.erase(f, 1);
	f = str.find("$");
	if (f != std::string::npos)
		str.erase(f, 1);

	if(IsNumber(str))
		return stoi(str);

	return 0;
}
//prettyprint adds commas and shit
std::string StringUtils::ToMoneyString(float amount, bool prettyPrint)
{
	
	char ret[15];//15 is enoguh to squeeze in 3 commas
	

	if (prettyPrint)
	{
		char temp[15];
		sprintf(temp, "%.2f", amount);
		int numExtraSpaces = 0;
		//see how many extra chars we need when adding in commas:
		if (strlen(temp) > 6)
			numExtraSpaces = 1;

		int comma = 0;
		int retIndex = strlen(temp) + numExtraSpaces;

		//add the commas
		ret[retIndex] = '\0';
		ret[retIndex - 1] = temp[strlen(temp) - 1];
		ret[retIndex - 2] = temp[strlen(temp) - 2];
		ret[retIndex - 3] = temp[strlen(temp) - 3];
		retIndex -= 3;
		for (int i = strlen(temp) - 3; i >= 0; i--)//start from the whole dollar amount
		{
			if (comma < 4)
			{
				ret[retIndex] = temp[i];
				retIndex--;
			}
			else
			{
				ret[retIndex] = ',';
				ret[retIndex - 1] = temp[i];
				retIndex -= 2;
				comma = 0;
			}
			comma++;
		}
	}
	else
	{
		sprintf(ret, "%.2f", amount);
	}
	return ret;
}

std::string StringUtils::GetRandomAlphaNumericString(int size)
{

	std::string ret;
	time_t t;
	srand((unsigned)time(&t));
	static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < size; i++)
	{
		int index = rand() % sizeof(alphanum-1);
		ret += alphanum[index];
	}
	return ret;
}

int StringUtils::HammingDistance(std::string str1, std::string str2)
{
	int difference = 0;
	size_t len = 0;
	size_t diff = 0;
	//use the smaller string size
	if (str1.size() > str2.size())
	{
		len = str2.size();
		diff = str1.size() - str2.size();
	}
	else
	{
		len = str1.size();
		diff = str2.size() - str1.size();
	}

	for (int i = 0; i < len; i++)
	{
		if (str1[i] != str2[i])
			difference++;
	}
	difference += diff;
	return difference;
}

std::string StringUtils::LongestCommonSubStr(std::string str1, std::string str2)
{
	// converted 2d array into a 1d array
	// length of longest common suffix
	// of X[0..i-1] and Y[0..j-1].
	int m = str1.size();
	int n = str2.size();
	int maxlen = 0;         // stores the max length of LCS
	int endingIndex = m;    // stores the ending index of LCS in str1

	int *lookup = new int[(m + 1) * (n + 1)];
	memset(lookup, 0, sizeof(lookup));


	// Following steps build lookup[m+1][n+1] in bottom up fashion. 
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (str1[i - 1] ==str2[j - 1]) 
			{
				lookup[i * n + j] = lookup[(i-1) * n + (j-1)] + 1;
				// update the maximum length and ending index
				if (lookup[i * n + j] > maxlen)
				{
					maxlen = lookup[i * n + j];
					endingIndex = i;
				}
			}
		}
	}
	// return longest common substring having length `maxlen`
	return str1.substr(endingIndex - maxlen, maxlen);
}
std::string StringUtils::ReturnErrorAsJSONObj(std::string err)
{
	std::string ret = "{" + CreateJsonEntry("error", err) + "}";
	return ret;
}
std::string StringUtils::WrapStringInSingleQuotes(std::string text)
{
	return "'" + text + "'";
}
std::string StringUtils::WrapStringInDoubleQuotes(std::string text)
{
	return "\"" + text + "\"";
}