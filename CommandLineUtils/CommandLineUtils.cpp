#include <iostream>


#include "CommandLineUtils.h"



std::string CommandLineUtils::GetFilePath(std::string path)
{
	std::size_t found = path.find_last_of("/\\");
	if(found != std::string::npos)
		return path.substr(0, found);
	return path;
}

std::string CommandLineUtils::GetFileName(std::string path)
{
	std::size_t found = path.find_last_of("/\\");
	if (found != std::string::npos)
		return path.substr(found + 1);
	return path;
}
