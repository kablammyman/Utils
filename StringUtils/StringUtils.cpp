#include "StringUtils.h"
#include <fstream>

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

bool StringUtils::IsUpper(std::string input)
{
	for (size_t j = 0; j < input.size(); j++)
	{
		if (!isupper(input[j]) && isalpha(input[j]))
		{
			return false;
		}
	}
	return true;
}

bool StringUtils::IsLower(std::string input)
{
	for (size_t j = 0; j < input.size(); j++)
	{
		if (!islower(input[j]) && isalpha(input[j]))
		{
			return false;
		}
	}
	return true;
}

bool StringUtils::IsProperNoun(std::string input)
{
	//bool caps = true;
	if (input.empty())
		return false;

	if (!isupper(input[0]))
		return false;

	for (size_t i = 1; i < input.size(); i++)
	{
		if (!islower(input[i]) && isalpha(input[i]))
		{
			return false;
		}
	}
	return true;
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
	if(input.empty())
		return;

	input.erase(0, input.find_first_not_of(' '));       //prefixing spaces
	input.erase(0, input.find_first_not_of('\n'));       //prefixing new lines
	input.erase(0, input.find_first_not_of('\t'));       //prefixing tabs

	input.erase(input.find_last_not_of(' ')+1);         //surfixing spaces
	input.erase(input.find_last_not_of('\n')+1);         //surfixing new lines
	input.erase(input.find_last_not_of('\t')+1);         //surfixing tabs
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
	if (dataEnd == std::string::npos)
		return "";
		//dataEnd = line.size();

	std::string returnString = "";

	//i like loops better thans substring for this...its easier to understand
	for (size_t i = dataStart; i < dataEnd; i++)
	{
		returnString += line[i];
	}
	return returnString;
}

void StringUtils::StringSplit(std::string orig, std::string &str1, std::string &str2, size_t index, bool removeIndexChar)
{
	str1 = orig.substr(0, index);
	if (removeIndexChar)
		index++;
	str2 = orig.substr(index, orig.size());
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
			if (  !isalpha(line[start]) && !isalpha(line[end]))
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
			//this makes sure we get the word and not a substring
			//we want "surf" only in "this is the surf and turf" vs "we are surfers"
			int index = GetStandAloneWordInLineIndex(line, curWord);
			if ( index > -1)
				return i;
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
//if you dont want html stuff, look at GetDataBetweenSubStrings
//make sure you dont look for a "complete" tag i.e: <tr> unles you realllllly want to. sometimes the '>' comes mucher later
std::string StringUtils::CopyCharsBetweenHTMLTags(std::string html, std::string openTag, std::string closeTag,  size_t & offset)
{
	size_t start = html.find(openTag, offset);
	if (start == std::string::npos)
		return "";

	size_t end = 0;

	start = html.find('>', start) + 1;

	end = html.find(closeTag, start);
	std::string curData = html.substr(start, end - start);

	start = html.find(openTag, end);

	offset = end;

	return curData;
}
//finds all tag pairs and gets the strings between them. made for html table parsing
std::vector<std::string> StringUtils::CopyStringBetweenIdenticalTags(std::string html, std::string openTag, std::string closeTag, size_t& offset)
{
	std::vector<std::string> ret;
	size_t start = html.find(openTag, offset);
	size_t end = 0;

	while (start != std::string::npos)
	{
		//i dont want the tag included in the final string
		//ande some tags are long...there are the "<tr>" or there can be "<td class="currency">"...this takes care of both
		start = html.find('>', start)+1;

		end = html.find(closeTag, start);
		std::string curData = html.substr(start, end - start);
		ret.push_back(curData);
		start = html.find(openTag, end);
	}
	offset = end;

	return ret;
}
std::string StringUtils::CopyCharsBetweenChars(std::string word, char open,char closed,size_t index)
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
	//when I have string like "--" or "++" this should fail!
	bool hasDigits = false;

	for (size_t i = 0; i < word.size(); i++)
		//should I include the $ or %?
		if (!isdigit(word[i]) && word[i] != '.' && word[i] != '-' && word[i] != '$' && word[i] != ',')
			return false;
		else
		{
			//i prob should have a check to make sure that its a valid number so something like 9- or 6+7++4 isnt accepted or something
			if (isdigit(word[i]))
				hasDigits = true;
		}
	if(hasDigits)
		return true;
	return false;
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
std::string StringUtils::GetAlphaNumericString(std::string line, size_t index,std::string bonusChars)
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
std::string StringUtils::CopyUntilChar(std::string line, size_t index, char delim)
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
		std::string curTag = "<"+ CopyCharsBetweenChars(orig, '<', '>', found) + ">";
		orig = FindAndReplace(orig, curTag, "");
		found = orig.find("<",found);
	}

	TrimWhiteSpace(orig);
	//print "done: " + line
	return orig;
}

std::string StringUtils::EmailStringClean(std::string orig)
{
	for (size_t i = 0; i < orig.size(); i++)
	{
		if (orig[i] == '%' || orig[i] == '\n' || orig[i] == '\r' ||
			orig[i] == '\t' || orig[i] == '<' || orig[i] == '>' || orig[i] == ':')
			orig[i] = ' ';
	}
	TrimWhiteSpace(orig);
	return orig;
}

void StringUtils::SanitizeSQLDataString(std::string& value)
{
	std::string ret = "";
	//std::string correct = "lineone\\nline2";
	for (size_t i = 0; i < value.size(); i++)
	{
		//the different tyepes of double quotes...including the fancy unicode ones
		if (value[i] == '"' || value[i] == '\u201c'|| value[i] == '\u201d')
		{
			ret += "\"\"";
		}
		else if (value[i] == '\'' || value[i] == '\u2019')
		{
			ret += "''";
		}
		else if (value[i] == '\r\n')
		{
			ret += "\n";
		}
		else if (value[i] == '\r')
		{
			continue;
		}
		else if (value[i] == '\u2026')
		{
			ret += "...";
		}
		else
			ret += value[i];
	}
	//sometimes the unicde chars value gets turned into a string...so we gotta check for that too
	ret = FindAndReplace(ret, "\\u201c", "\"\"");
	ret = FindAndReplace(ret, "\\u201d", "\"\"");
	ret = FindAndReplace(ret, "\\u2019", "''");
	ret = FindAndReplace(ret, "\\u2026", "...");


	value = ret;
}


void StringUtils::SanitizeJsonString(std::string& value)
{
	if (value.empty())
		return;

	if (value.size() == 1 && value[0] == '\0')
	{
		value.clear();
		return;
	}

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
			ret += '\\';
			ret += "\"";
		}
		else if (value[i] == '\\')
		{
			ret += "\\\\";
		}
		else if (value[i] == '\t')
		{
			ret += "\\t";
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
				else if (value[i + 1] == '/')
				{
					ret += '/';
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
std::string StringUtils::ReturnErrorAsJSONObj(std::string err)
{
	std::string ret = "{" + CreateJsonEntry("error", err) + "}";
	return ret;
}
std::string StringUtils::CreateCompleteErrorJson(std::string message, std::string methodName)
{
	std::string ret = "{" + CreateJsonEntry("status", "ERROR");
	if(!methodName.empty())
		ret += CreateJsonEntry("Method", methodName);
	ret += CreateJsonEntry("Error", message,true) +"}"; 
	return ret;
}

std::string StringUtils::CreateCompleteOKJson(std::string message)
{
	if(message.empty())
		return "{" +CreateJsonEntry("status", "OK", true) + "}";
	std::string ret = "{" + CreateJsonEntry("status", "OK");
	ret += CreateJsonEntry("Details", message,true) + "}";
	return ret;
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
	//we have to find the first non space char (should be a quote or a number)

	start = json.find(":", start) +1;


	//first check if this is a std::string value
	size_t quote = json.find("\"", start);
	size_t comma = json.find(",", start);

	size_t brace = json.find("}", start);
	if (quote < comma)//we have a std::string valua, aka wrapped in quotes
	{
		while (json[start] == ' ' || json[start] == '\t')
			start++;

		//if this is true, then find the comma that singals the end of tis line in the json, and not a comma inside the quotes
		size_t endQuote = json.find("\"", quote + 1);

		while (endQuote != std::string::npos)
		{
			//GOTTA MAKE SURE THIS ISNT A QUOTE WITHIN THE STRING!!
			if (endQuote != std::string::npos && endQuote > 0)
			{
				//if this is an escaped quote string, then this isnt the end of the string
				if (json[endQuote - 1] != '\\')
					break;
				else
					endQuote = json.find("\"", endQuote + 1);
			}
		}

		//bad formatting of the json string?
		if(endQuote == std::string::npos)
			return ret;
		//what if the brace is inside a json string value, lets find the real end of json just in case
		if (brace > quote && brace < endQuote)
			brace = json.find("}", endQuote);

		comma = json.find(",", endQuote);
	}
	
	
	


	//prob bad json formatting
	if (comma == std::string::npos && brace == std::string::npos)
		return ret;

	size_t end = (comma < brace) ? comma : brace;
	
	if (json[end - 1] == '"')
		end--;
	if (quote < comma)
		start++;

	//when we are getting the last item in a json
	if (comma == std::string::npos && brace != std::string::npos)
		end = json.find("\"", quote + 1);
	
	//chop the chars to the output, fuck substr
	for (size_t i = start; i < end; i++)
		ret += json[i];

	//ret = substr(start+1, end - start);

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

void StringUtils::RemoveLastCommaFromJson(std::string &json)
{
	size_t comma = json.find_last_of(',');
	//size_t bracket = json.find_last_of('[');
	if (comma != std::string::npos/* && comma > bracket*/)
	{
		json[comma] = ' ';
	}
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

void RemoveNumberDressing(std::string &str)
{
	size_t f = 0;

	while (f != std::string::npos)
	{
		f = str.find(",");
		if (f != std::string::npos)
			str.erase(f, 1);
	}

	//prob wont be more than 1 dollar sign
	//while (f != string::npos)
	{
		f = str.find("$");
		if (f != std::string::npos)
			str.erase(f, 1);
	}
}

float StringUtils::GetFloatFromString(std::string str)
{
	if(str.empty())
		return 0.0;

	RemoveNumberDressing(str);

	TrimWhiteSpace(str);

	if(IsNumber(str))
		return stof(str);

	return 0.0f;
}



double StringUtils::GetDoubleFromString(std::string str)
{
	if(str.empty())
		return 0.0;
	
	RemoveNumberDressing(str);

	TrimWhiteSpace(str);

	if(IsNumber(str))
		return stod(str);

	return 0.0l;
}

int StringUtils::GetIntFromString(std::string str)
{
	if(str.empty())
		return 0;

	RemoveNumberDressing(str);

	TrimWhiteSpace(str);

	if(IsNumber(str))
		return stoi(str);

	return 0;
}
//prettyprint adds commas and shit
std::string StringUtils::ToMoneyString(float amount, bool prettyPrint)
{
	char charBuffer[15];//15 is enoguh to squeeze in 3 commas
	std::string ret;

	if (prettyPrint)
	{
		char temp[15];
		sprintf(temp, "%.2f", amount);
		int numExtraSpaces = 0;
		//see how many extra chars we need when adding in commas:
		//first 6 chars are 123.45
		if (strlen(temp) > 6)
		{
			numExtraSpaces++;
			//if we have over amillion, add another comma
			//1234567.00
			if (strlen(temp) > 10)
				numExtraSpaces++;
		}


		//neg sign is already included!
		//if(amount < 0)//need space for neg sign
		//	numExtraSpaces++;

		int comma = 0;
		int retIndex = strlen(temp) + numExtraSpaces;

		//add the decimal numbers and decimal point
		charBuffer[retIndex] = '\0';
		charBuffer[retIndex - 1] = temp[strlen(temp) - 1];
		charBuffer[retIndex - 2] = temp[strlen(temp) - 2];
		charBuffer[retIndex - 3] = temp[strlen(temp) - 3];
		retIndex -= 3;

		for (int i = strlen(temp) - 3; i >= 0; i--)//start from the whole dollar amount
		{
			if (comma < 4 || i == 0)
			{
				charBuffer[retIndex] = temp[i];
				retIndex--;
			}
			else if (comma >= 4 && i > 0)
			{
				charBuffer[retIndex] = ',';
				charBuffer[retIndex - 1] = temp[i];
				retIndex -= 2;
				comma = 0;
			}
			comma++;
		}

		//now only return the good parts of the char array
		if ((amount >= 1000 && amount < 10000) || (amount < 0 && amount > -1000) )
		{
			std::string newVer = charBuffer;
			size_t start = newVer.find(temp);
			ret = newVer.substr(start);
		}
		else ret = charBuffer;
	}
	else
	{
		sprintf(charBuffer, "%.2f", amount);
		ret = charBuffer;
	}
	return ret;
}

std::string StringUtils::ToPercentString(float amount, bool convertFloatToPercet)
{
	char charBuffer[8];//xx.xx%
	std::string ret;

	if(convertFloatToPercet)
		amount *= 100;

	sprintf(charBuffer, "%.2f", amount);
	ret = charBuffer;
	ret += "%";
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

	for (size_t i = 0; i < len; i++)
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

std::string StringUtils::WrapStringInSingleQuotes(std::string text)
{
	return "'" + text + "'";
}
std::string StringUtils::WrapStringInDoubleQuotes(std::string text)
{
	return "\"" + text + "\"";
}

bool StringUtils::IsSameWord(std::string word1, std::string word2)
{
	ToLower(word1);
	ToLower(word2);
	if (word1 == word2)
		return true;
	return false;
}

std::string StringUtils::ConvertToE164(std::string input)
{
	std::string ret;
	size_t start = 0;
	if (input[0] == '+')
	{
		ret = "+1";
		start += 2;
	}
	else if (input[0] == '1')
	{
		start++;
	}
	else
	{
		ret = "1";
	}

	for (size_t i = start; i < input.size(); i++)
	{
		if (isdigit(input[i]))
			ret += input[i];
	}
	return ret;
}

//stole this from SMSContact. not sure how to ombine this without adding a ton of other dependencies
std::string StringUtils::PrettyPhone(std::string phoneToUpdate)
{
	//sly text makes phone numbers look like : 3072773219
	//skip tracer gives phone like : (307) 277 - 3219
	//so i need to be able to check in anyformat

	//first start from a clean canvas :
	std::string phone;

	for (size_t i = 0; i < phoneToUpdate.size(); i++)
	{
		if (isdigit(phoneToUpdate[i]))
			phone += phoneToUpdate[i];
	}

	//if the first digit is a 1 fort he us contry code, then remove it
	if (phone[0] == '1')
		phone.erase(0, 1);

	//now the number should be in this format:  3072773219
	//so lets change it to(307) 277 - 3219

	std::string newPhone = "(";
	for (size_t i = 0; i < phone.size(); i++)
	{
		newPhone += phone[i];
		if (i == 2)
			newPhone += ") ";
		else if (i == 5)
			newPhone += "-";
	}

	return newPhone;
}


bool StringUtils::IsValidPhone(std::string phone)
{
	//if the string itself is too small, then it cant be valid
	if (phone.size() < 10 )
		return false;

	//you can trick the system by padding the spaces between numbers with any char. is this a problem?
	std::string acutalPhone;

	for (size_t i = 0; i < phone.size(); i++)
	{
		//to make sure strange chars dont crash this, cast to unsigned char
		unsigned char curDigit = (unsigned char)phone[i];
		if (isdigit(curDigit))
		{
			acutalPhone += phone[i];
		}
	}
	if (acutalPhone.size() == 10)
		return true;

	//if we have a 1 in front it will be 11 digits
	if (acutalPhone.size() == 11 && acutalPhone[0] == '1')
		return true;

	return false;
}
bool IsCharHexValue(char val)
{
	if ((val >= 48 && val <= 57) || (val >= 65 && val <= 70))
		return true;
	return false;
}
std::string StringUtils::UrlDecode(std::string urlEncoded,char delim)
{
	std::string ret;
	char inBytes[3];
	size_t i = 0;
	while( i < urlEncoded.size())
	{
		if (urlEncoded[i] == delim && IsCharHexValue(urlEncoded[i+1]) && IsCharHexValue(urlEncoded[i + 2]))
		{
			inBytes[0] = urlEncoded[i + 1];
			inBytes[1] = urlEncoded[i + 2];
            inBytes[2] = '\0';
			//ret += std::stoul(inBytes, nullptr, 16);
            ret += static_cast<char>(std::stoul(inBytes, nullptr, 16));
			i += 3;
		}
		else
		{
			if (urlEncoded[i] == '+')
				ret += " ";
			else
				ret += urlEncoded[i];
			i++;
		}
	}
	return ret;
}

std::string StringUtils::UrlDecode2(std::string urlEncoded,char delim)
{
	std::string ret;
	char inBytes[3];
	size_t i = 0;
	while( i < urlEncoded.size())
	{
		if (urlEncoded[i] == delim && IsCharHexValue(urlEncoded[i+1]) && IsCharHexValue(urlEncoded[i + 2]))
		{
			inBytes[0] = urlEncoded[i + 1];
			inBytes[1] = urlEncoded[i + 2];
            inBytes[2] = '\0';
			ret += std::stoul(inBytes, nullptr, 16);
            //ret += static_cast<char>(std::stoul(inBytes, nullptr, 16));
			i += 3;
		}
		else
		{
			if (urlEncoded[i] == '+')
				ret += " ";
			else
				ret += urlEncoded[i];
			i++;
		}
	}
	return ret;
}

std::string StringUtils::UnEscapeString(std::string origString)
{
	return "";
}
std::string StringUtils::GetRandomWord(std::vector <std::string> &words)
{
	//srand(time(0));
	//int index = rand() % words.size();
	srand(time(nullptr));
	int index = rand() % words.size() - 1;
	if (index < 0)
		index = 0;
	if (index >= words.size())
		index = words.size() - 1;
	return words[index];
}
//---------------------------------------------------------------------------------------------------------------
int StringUtils::InsertMultilineInput(std::string replaceMe, std::string lines, size_t index, std::string newText)
{
	/*linez = PropertyHelper.CreateRTFLineBreak(addy)

	for i in range(0, len(linez)) :
	line = lines[index]

	if i == 0 :
	line = line.replace(replaceMe, linez[i])

	else:
	line = lines[index] + linez[i]

	index += 1

	offerLetter.write(line + "\n")

	return index*/
	return 0;
}
//---------------------------------------------------------------------------------------------------------------
std::string StringUtils::ReplaceTagForValue(std::string input, DocumentTags tagMap)
{
	//no need to do anything if theres no tags on this line
	if (input.find("[") == std::string::npos)
		return input;
	for (size_t i = 0; i < tagMap.GetDistinctTagCount(); i++)
	{
		//now get rid of html tags
		size_t found = input.find("[");
		while (found != std::string::npos)
		{
			std::string curTag = CopyCharsBetweenChars(input, '[', ']', found);
			if (curTag.empty())//empty tag
				found++;
			else
			{
				std::string newWord;
				//now tags can have words that we can "rotate" or get a random one from the list.
				//this word does not have to be in the map
				if (curTag.find('|') != std::string::npos)
				{
					std::vector<std::string>words = Tokenize(curTag, '|');
					newWord = GetRandomWord(words);
				}
				else
					newWord = tagMap.GetTagValue(curTag);
				input = FindAndReplace(input, "[" + curTag + "]", newWord);
			}

			found = input.find("[", found + 1);
		}

		/*std::size_t found = input.find(tagMap.GetTagNameAt(i));
		if (found != string::npos)
		{
		string newValue = tagMap.GetTagValueAt(i);

		//if type(newValue) is float :
		//newValue = "{:,.2f}".format(newValue)

		input.replace(found, newValue.size(),newValue );
		}*/
	}
	return input;
}

//---------------------------------------------------------------------------------------------------------------
std::map<std::string, int> StringUtils::GetAllTagsInFile(std::string filePath)
{
	std::map<std::string, int> tagMap;
	std::map<std::string, int>::iterator it;

	std::string	line;
	int index = 0;
	int lineNum = 0;
	std::ifstream templateFile;
	templateFile.open(filePath, std::fstream::in);
	if (!templateFile.is_open())
	{
		templateFile.close();
		//printf("CountTagsInFile: couldnt open: %s", filePath.c_str());
		return tagMap;
	}

	size_t tagOpen;
	while (getline(templateFile, line))
	{
		lineNum++;
		index = 0;
		tagOpen = 0;
		while (tagOpen < line.size())
		{
			std::string tag = GetDataBetweenChars(line, '[', ']', tagOpen);

			if (tag.empty())
				continue;

			it = tagMap.find(tag);

			//not int he map, add it
			if (it != tagMap.end())
			{
				tagMap[tag] = 1;
			}
			//else add to num times we seen this tag
			else
			{
				tagMap[tag]++;
			}
		}

	}

	templateFile.close();
	return tagMap;
}
//---------------------------------------------------------------------------------------------------------------
void StringUtils::LoadRotatorMessagesFromFile(std::string file, std::vector<std::string> &messages)
{

	std::ifstream templateFile;

	templateFile.open(file, std::fstream::in);

	if (!templateFile.is_open())
	{
		printf("LoadRotatorMessagesFromFile error 1: couldnt open template: %s\n", file.c_str());
		return;
	}


	//put all of the file into 1 string for easy parsing
	std::string data((std::istreambuf_iterator<char>(templateFile)),
		std::istreambuf_iterator<char>());
	size_t i = 0;


	while (i < data.size())
	{
		i = data.find("<START>", i);

		if (i != std::string::npos)
		{
			std::string curMessage = StringUtils::GetDataBetweenSubStrings(data, "<START>", "<END>", i);
			i += 8;//move it passed the start token to find the next start
			messages.push_back(curMessage);
		}
	}
	templateFile.close();
}
