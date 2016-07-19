#pragma once

#include <vector>
#include <string>

namespace StringUtils
{
	std::vector<std::string> Tokenize(std::string path, std::string delims);
	void ToProperNoun(std::string &input);
}