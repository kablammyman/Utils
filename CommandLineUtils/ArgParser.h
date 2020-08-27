#pragma once
#include <string>
#include <vector>


class ArgParser
{
public:
	enum Action { FLAG, STORE, PATH };
private:
	struct Args
	{
		std::string name;//nam of arg....maybe not needed?
		bool enabled;//set to true once we get it from commandline
		Action action;//what am i dont with this commandline arg
		std::string helpText;//"this is what this option means
		std::string shortFlag;// -i
		std::string longFlag;//-input
		std::string returnValue;//the part we care about
	};
	std::vector<Args> args;
	int GetArgFromFlag(std::string flag);
	std::string basePath;
	std::string exeName;
public:
	bool ParseArgs(int argc, const char* argv[]);
	void StoreFilePath(Args& curArg, std::string path);
	void StoreStringValue(Args& curArg, std::string value);
	void StoreFlagValue(Args& curArg);
	void AddArg(std::string name, Action action, std::string shortFlag, std::string longFlag, std::string helpText);
	bool IsArgEnabled(std::string name);
	bool NoValidArgs();
	std::string GetArgValue(std::string argName);
	std::string HelpText();
};