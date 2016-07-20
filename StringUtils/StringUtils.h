#pragma once

#include <vector>
#include <string>

class StringUtils
{
public:
	static std::vector<std::string> Tokenize(std::string path, std::string delims);
	static void ToProperNoun(std::string &input);
};