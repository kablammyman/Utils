#include "DocumentTags.h"
#include "StringUtils.h"
#include <time.h>
#include <fstream>

using namespace std;



void DocumentTags::AddOrUpdateTagAndValue(std::string tagName, std::string value)
{
	size_t index = GetTagValueIndex(tagName); 
	if(isRtfFile)//we only want to do this for rtf files since this dont use \n for teh document
	{
		if(value.find("\n") != string::npos)
		{
			value = StringUtils::FindAndReplace(value,"\n"," \\par ");
		}
	}
	if (index == string::npos)
	{
		tags.push_back(make_pair(tagName, value));
	}
	else
	{
		tags[index].second = value;
	}
}
//---------------------------------------------------------------------------------------------------------------
void DocumentTags::AddOrUpdateTagAndValue(std::string tagName, int value)
{
	size_t index = GetTagValueIndex(tagName); 
	if (index == string::npos)
	{
		tags.push_back(make_pair(tagName, to_string(value)));
	}
	else
	{
		tags[index].second = to_string(value);
	}
}
//---------------------------------------------------------------------------------------------------------------
void DocumentTags::AddOrUpdateTagAndValue(std::string tagName, float value)
{
	size_t index = GetTagValueIndex(tagName); 
	if (index == string::npos)
	{
		tags.push_back(make_pair(tagName, StringUtils::ToMoneyString(value)));
	}
	else
	{
		tags[index].second = StringUtils::ToMoneyString(value);
	}
}
//---------------------------------------------------------------------------------------------------------------
string DocumentTags::GetTagValue(string tagName)
{
	for (size_t i = 0; i < tags.size(); i++)
	{
		if (tagName == tags[i].first)
		{
			return tags[i].second;
		}
	}
	return "";
}
//---------------------------------------------------------------------------------------------------------------
size_t DocumentTags::GetTagValueIndex(string tagName)
{
	for (size_t i = 0; i < tags.size(); i++)
	{
		if (tagName == tags[i].first)
		{
			return i;
		}
	}
	return string::npos;
}
//---------------------------------------------------------------------------------------------------------------
size_t DocumentTags::GetDistinctTagCount()
{
	return tags.size();
}
//---------------------------------------------------------------------------------------------------------------
string DocumentTags::GetTagNameAt(size_t index)
{
	if(index > tags.size())
		return "";
	return tags[index].first;
}
//---------------------------------------------------------------------------------------------------------------
string DocumentTags::GetTagValueAt(size_t index)
{
	if(index > tags.size())
		return "";
	return tags[index].second;
}
//---------------------------------------------------------------------------------------------------------------
vector<string> DocumentTags::GetMissingFields()
{
	vector<string> retList;
	for (size_t i = 0; i < tags.size(); i++)
	{
		if (tags[i].second.empty())
			retList.push_back(tags[i].first);
	}
	return retList;
}
//---------------------------------------------------------------------------------------------------------------
