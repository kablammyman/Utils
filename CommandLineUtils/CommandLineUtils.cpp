#include <iostream>
#include "StringUtils.h"

#include "CommandLineUtils.h"


using namespace std;

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

string CommandLineUtils::GetStringInput(string inputMessage)
{
	string input;
	cout << inputMessage << ": ";
	getline(cin, input);
	return input;
}
int CommandLineUtils::GetIntInput(string inputMessage)
{
	string input;
	cout << inputMessage << ": ";
	getline(cin, input);

	int selection;
	try
	{
		selection = StringUtils::GetIntFromString(input);
	}
	catch (invalid_argument e)
	{
		cout << "invalid input\n";
		return 0;
	}
	return selection;
}
float CommandLineUtils::GetFloatInput(string inputMessage)
{
	string input;
	cout << inputMessage << ": ";
	getline(cin, input);
	float ret;
	try
	{
		ret = stof(input);
	}
	catch (invalid_argument e)
	{
		cout << "invalid input\n";
		return 0.0f;
	}
	return ret;
}

bool CommandLineUtils::PressYTOContinue(string inputMessage)
{
	string input = GetStringInput(inputMessage);
	if (input == "y" || input == "Y")
		return true;
	return false;
}

int CommandLineUtils::SelectFromList(string inputMessage, vector<string> list)
{
	int input;
	for (size_t i = 0; i < list.size(); i++)
	{
		cout << i + 1 << ") " << list[i] << endl;
	}

	input = GetIntInput(inputMessage);

	if (input < 1 || input > list.size())
	{
		cout << "invalid selection...\n";
		return 0;
	}
	return input;
}