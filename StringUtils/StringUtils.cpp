#include "StringUtils.h"

#ifndef _WIN32
#include <cstring> //strtok_r
#endif

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
//this will only trim a single white space char from each side...should be more robust...
void StringUtils::TrimWhiteSpace(std::string &input)
{
	if(input.size() == 0)
		return;

	if(input.back() == ' ')
		input.pop_back();

	if(input[0] == ' ')
		input.erase(0, 1);
}

std::string StringUtils::GetDataBetweenChars(std::string line, char char1, char char2, size_t start)
{
	size_t dataStart = line.find(char1,start);
	if(dataStart == std::string::npos)
		return "";
	std::string returnString = "";

	//i like loops better thans substring for this...its easier to understand
	for (size_t i = dataStart+1; i < line.size(); i++)
	{
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
		index += start+1;
		
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
				return start;
		}
		else if( startIsFirst && endIsLast)
			return start;
			
		else if( startIsFirst && !endIsLast)
		{
			if (! isalpha(line[end]))
				return start;
		}
		else
		{
			if (! isalpha(line[start]) && !isalpha(line[end]))				
				return start;
		}
	}
	return -1;
}

bool StringUtils::IsWordFromListInLine(std::vector<std::string> &wordList,std::string line)
{
	//if any(word in line for word in wordList):
	ToUpper(line);
	for(size_t i = 0; i < wordList.size(); i++)
	{
		if (wordList[i] == line)
			if (GetStandAloneWordInLineIndex(line,wordList[i]) > -1)
				return true;
	}
	return false;
}

std::string StringUtils::GetWordFromListInLine(std::vector<std::string> &wordList,std::string line)
{
	ToUpper(line);
	for (size_t i = 0; i < wordList.size(); i++)
	{
		if(line.find( wordList[i] ) != std::string::npos )
			if( GetStandAloneWordInLineIndex(line,wordList[i]) > -1)
				return wordList[i];
	}
	return "";
}

int StringUtils::GetIndexOfWordFromListInLine(std::vector<std::string> &wordList,std::string line)
{
	ToUpper(line);
	for (size_t i = 0; i < wordList.size(); i++)
	{
		if(line.find( wordList[i] ) != std::string::npos )
		{
			int index = GetStandAloneWordInLineIndex(line,wordList[i]);
			if ( index > -1)
				return index;
		}
	}
	return -1;
}
std::string StringUtils::CopyCharsBetweenQuotes(std::string word, size_t index)
{
	std::string retString;
	index = word.find('"',index);
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

bool StringUtils::IsPrevCharNonAlpha(std::string line,std::string word)
{
	size_t start = line.find(word);
	if (start != std::string::npos)
		if (! isalpha(line[start-1]))
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