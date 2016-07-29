#pragma once

#include <vector>
#include <string>

class StringUtils
{
public:
	static std::vector<std::string> Tokenize(std::string path, std::string delims);
	static void ToProperNoun(std::string &input);
	static void ToUpper(std::string &input);
	static void ToLower(std::string &input);
	static std::string FlattenVector(std::vector<std::string> input); 
	static void TrimWhiteSpace(std::string &input);
};