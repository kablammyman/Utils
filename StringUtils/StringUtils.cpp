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
		i = path.find(delim);
		if (i!=std::string::npos)
		{
			std::string temp = path.substr (start,i);
			returnVec.push_back(temp);
		}
		else
			break;
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
