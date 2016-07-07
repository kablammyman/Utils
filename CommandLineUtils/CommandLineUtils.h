#pragma once

#include <string>
#include <vector>

//a struct to organize command line params, as well as commands to send over sockets
struct CmdArg
{
	//where do the results go...stdout(-1)? a socket(0-max socket) a text file (-2?)
	int dest;
	std::string cmd;
	std::vector<std::string> data;
	//what will the option be when executing this command

	//when we have errors, clear the data and whatever else we collected
	//so this wont try to run
	void clear()
	{
		cmd.clear();
		data.clear();
	}
	void SetCommand(std::string c)
	{
		//make sure all data we get is only for this command
		data.clear();
		cmd = c;
	}
};

class CommandLineUtils
{
public:
	static std::string GetExePath();
	static std::string SetProgramPath(std::string argv);
	
};