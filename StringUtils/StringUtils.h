#pragma once

#include <vector>
#include <string>

class StringUtils
{
public:
	static std::vector<std::string> Tokenize(std::string path, std::string delims);
	static std::vector<std::string> Tokenize2(std::string path, std::string delims);
	static void ToProperNoun(std::string &input);
	static void ToUpper(std::string &input);
	static void ToLower(std::string &input);
	static std::string FlattenVector(std::vector<std::string> input); 
	static void TrimWhiteSpace(std::string &input);
	static std::string GetDataBetweenChars(std::string line,char char1, char char2, size_t start = 0);
	static std::string GetDataBetweenSubStrings(std::string line, std::string str1, std::string str2, size_t start =0);
};