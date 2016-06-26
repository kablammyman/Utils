#pragma once

#include <string>
#include "Windows.h"

class WindowsUtils
{
public:
	static std::string GetEXEPath();
	static std::string SetProgramPath(std::string argv);
};