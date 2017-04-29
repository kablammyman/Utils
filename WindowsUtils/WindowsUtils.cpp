#include "WindowsUtils.h"

std::string WindowsUtils::GetEXEPath()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(0, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	if (pos == std::string::npos)
		return "";

	return std::string(buffer).substr(0, pos);
}

