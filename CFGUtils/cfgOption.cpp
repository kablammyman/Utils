#include "cfgOption.h"

cfgOption::cfgOption(int t)
{
	type = t;
}

string cfgOption::ToString()
{
	return optionName +":" + value + "\n";
}

void cfgOption::inputValue(bool value)
{
	if(type != OPTION_TYPE_BOOLEAN)
		return;

}
void cfgOption::inputValue(int value)
{
	if(type != OPTION_TYPE_INTEGER)
		return;
}
void cfgOption::inputValue(float value)
{
	if(type != OPTION_TYPE_FLOAT)
		return;
}
void cfgOption::inputValue(string value)
{
	if(type != OPTION_TYPE_STRING)
		return;
}