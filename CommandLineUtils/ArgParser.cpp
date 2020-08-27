#include "ArgParser.h"
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h> //for linux to get current wroking dir
#include <linux/limits.h>
#endif
using namespace std;

void ArgParser::AddArg(string name, Action action,  string shortFlag, string longFlag ,string helpText )
{
	Args newArg;
	newArg.name = name;
	newArg.action = action;
	newArg.shortFlag = shortFlag;
	newArg.longFlag = longFlag;
	newArg.helpText = helpText;
	newArg.enabled = false;
	args.push_back(newArg);
}
void ArgParser::StoreFilePath(Args &curArg, string path)
{
	if (path[0] == '\"')
		path.erase(0, 1);
	if (path[path.size() - 1] == '\"')
		path.pop_back();
	curArg.enabled = true;
	
	size_t found = path.find_last_of("/\\");
	bool inSubdir = (path[0] == '.' && path.find_first_of("/\\") == 1);
	//untested
	bool inCurDir = (path.size() == 0 || (path[0] == '.' && path.find_first_of("/\\") != 1));

	if (found == string::npos || inCurDir)
	{
		if (inSubdir)
		{
			path.erase(0, 2);
		}
		curArg.returnValue = basePath.substr(0, basePath.find_last_of("/\\") + 1) + path;

	}
	else
		curArg.returnValue = path;
	
}
bool ArgParser::IsArgEnabled(std::string name)
{
	for (size_t i = 0; i < args.size(); i++)
		if(args[i].name == name && args[i].enabled)
			return true;
	return false;
}
bool ArgParser::NoValidArgs()
{
	for (size_t i = 0; i < args.size(); i++)
		if (args[i].enabled)
			return false;
	return true;
}

void ArgParser::StoreStringValue(Args& curArg, string value)
{
	curArg.enabled = true;
	curArg.returnValue = value;
}


void ArgParser::StoreFlagValue(Args& curArg)
{
	curArg.enabled = true;
}

int ArgParser::GetArgFromFlag(string flag)
{
	for (size_t i = 0; i < args.size(); i++)
	{
		if (flag == args[i].shortFlag || flag == args[i].longFlag)
			return i;
	}
	return -1;
}

bool ArgParser::ParseArgs(int argc, const char* argv[])
{
	int start = 0;
#ifdef _WIN32
	string temp = argv[0];
	std::size_t found = temp.find_last_of("/\\");
	basePath = temp.substr(0, found);
	exeName = temp.substr(found + 1);
	start = 1;
	
#else
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) 
		basePath = cwd;
#endif
	int argsCounted = 0;
	for (int i = start; i < argc; i++)
	{
		string curArg = argv[i];
		int index = GetArgFromFlag(curArg);

		if (index == -1)
		{
			cout << "unrecognized param: " << curArg <<endl;
			continue;
			//return false;
		}
		if (args[index].action == Action::PATH)//store file path
		{
			StoreFilePath(args[index], argv[i + 1]);
			i++;
			argsCounted++;
		}
		else if (args[index].action == Action::STORE)//store file path
		{
			StoreStringValue(args[index], argv[i + 1]);
			i++;
			argsCounted++;
		}
		else if (args[index].action == Action::FLAG)//store file path
		{
			StoreFlagValue(args[index]);
			argsCounted++;
		}
	}//end for loop
	if(argsCounted)
		return true;
	return false;
}

string ArgParser::GetArgValue(string argName)
{
	for (size_t i = 0; i < args.size(); i++)
		if (args[i].name == argName && args[i].enabled)
			return args[i].returnValue;
	return "";
}

string ArgParser::HelpText()
{
	string ret = "";
	for (size_t i = 0; i < args.size(); i++)
	{
		ret += args[i].name + " " + args[i].shortFlag + " " + args[i].longFlag + " " + args[i].helpText + "\n";
	}
	return ret;
}