#include "WindowsUtils.h"

std::string WindowsUtils::GetEXEPath()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	if (pos == std::string::npos)
		return "";

	return std::string(buffer).substr(0, pos);
}

std::string WindowsUtils::SetProgramPath(std::string argv)
{
	char full[MAX_PATH];
	std::string temp = _fullpath(full, argv.c_str(), MAX_PATH);
	size_t found = temp.find_last_of("/\\");
	return temp.substr(0, found);
}
