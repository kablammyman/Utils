#include "StringUtils.h"

vector<string> Utils::tokenize(string path, string delims)
{
	vector<string> returnVec;
	char *next_token = NULL;
	char *p = strtok_s(const_cast<char *>(path.c_str()), delims.c_str(), &next_token);
	while (p)
	{
		//printf ("Token: %s\n", p);
		returnVec.push_back(p);
		p = strtok_s(NULL, delims.c_str(), &next_token);
	}
	return returnVec;
}

//turns words or sentences into all proper nouns
void Utils::toProperNoun(string &input)
{
	bool caps = true;
	for (size_t i = 0; i < input.size(); i++)
	{

		if (input[i] == ' ' || input[i] == '-' || input[i] == '_')
		{
			caps = true;
			continue;
		}
		if (caps)
		{
			input[i] = toupper(input[i]);
			caps = false;
		}
		else
			input[i] = tolower(input[i]);
	}
}