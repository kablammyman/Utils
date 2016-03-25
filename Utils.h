#pragma once

#include <string>
#include <vector>

using namespace std;

//a struct to organize command line params, as well as commands to send over sockets
struct CmdArg
{
	//where do the results go...stdout(-1)? a socket(0-max socket) a text file (-2?)
	int dest;
	string cmd;
	vector<string> data;
	//what will the optiosn be when executing this command

	//when we have errors, clear the data and whatever else we collected
	//so this wont tryt o run
	void clear()
	{
		cmd.clear();
		data.clear();
	}
	void SetCommand(string c)
	{
		//make sure all data we get is only for this command
		data.clear();
		cmd = c;
	}
};

class Utils
{
public:
	static string getExePath();
	static string setProgramPath(string argv);
	static vector<string> tokenize(string path, string delims);
	static void toProperNoun(string &input);
	static string getTimeStamp(double milis);
};