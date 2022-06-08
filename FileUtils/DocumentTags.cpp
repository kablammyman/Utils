#include "DocumentTags.h"
#include "StringUtils.h"


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

size_t DocumentTags::GetDistinctTagCount()
{
	return tags.size();
}
string DocumentTags::GetTagNameAt(size_t index)
{
	if(index > tags.size())
		return "";
	return tags[index].first;
}
string DocumentTags::GetTagValueAt(size_t index)
{
	if(index > tags.size())
		return "";
	return tags[index].second;
}
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

int Tags::InsertMultilineInput(string replaceMe, string lines, int index, std::string newText)
{
	/*linez = PropertyHelper.CreateRTFLineBreak(addy)

		for i in range(0, len(linez)) :
			line = lines[index]

			if i == 0 :
				line = line.replace(replaceMe, linez[i])

			else:
	line = lines[index] + linez[i]

		index += 1

		offerLetter.write(line + "\n")

		return index*/
	return 0;
}

	
string Tags::ReplaceTagForValue(string input, DocumentTags tagMap)
{
	//no need to do anything if theres no tags on this line
	if (input.find("[") == string::npos)
		return input;
	for (size_t i = 0; i < tagMap.GetDistinctTagCount(); i++)
	{
		//now get rid of html tags
		size_t found = input.find("[");
		while (found != std::string::npos)
		{
			std::string curTag =  StringUtils::CopyCharsBetweenChars(input, '[', ']', found);
			if (curTag.empty())//empty tag
				found++;
			else
				input = StringUtils::FindAndReplace(input, "[" + curTag + "]", tagMap.GetTagValue(curTag));

			found = input.find("[", found+1);
		}

		/*std::size_t found = input.find(tagMap.GetTagNameAt(i));
		if (found != string::npos)
		{
			string newValue = tagMap.GetTagValueAt(i);
			
			//if type(newValue) is float :
			//newValue = "{:,.2f}".format(newValue)
						
			input.replace(found, newValue.size(),newValue );
		}*/
	}
	return input;
}


	
void Tags::CreateNewFileFromTagTemaplateFile(string templatePath, string newFilePath, DocumentTags tagMap)
{

//if not os.path.exists(outputFolderBase):
		//	os.makedirs(outputFolderBase)

	ifstream templateFile;
	ofstream newFile;

	templateFile.open(templatePath, std::fstream::in);
	newFile.open(newFilePath, std::fstream::out);

	if (!templateFile.is_open())
	{
		templateFile.close();
		printf("CreateNewFileFromTagTemaplateFile error 1: couldnt open template: %s\n" , templatePath.c_str());
		return;
	}

	//when you ha e 2 paercels in one deal, this needs to change, otherwise you get this:
	//\\SERVER\documents\land investing\parcels\Texas\Henderson\R000054401 R000054402\Documents\sellDeed_Frances A. Moinet.rtf
	if (!newFile.is_open())
	{
		newFile.close();
		printf("CreateNewFileFromTagTemaplateFile error 2: couldnt open new file: %s\n",newFilePath.c_str());
		return;
	}

	string line;
	int wordListIndex = 0;
	while (getline(templateFile, line))
	{
		string newLine = ReplaceTagForValue(line, tagMap);
		newFile << newLine <<endl;
	}

	templateFile.close();
	newFile.close();
}
	
std::map<string,int> Tags::GetAllTagsInFile(string filePath)
{
	std::map<string,int> tagMap;
	std::map<string,int>::iterator it;

	string	line;
	int index = 0;
	int lineNum = 0;
	ifstream templateFile;
	templateFile.open(filePath, std::fstream::in);
	if (!templateFile.is_open())
	{
		templateFile.close();
		printf("CountTagsInFile: couldnt open: %s", filePath.c_str());
		return tagMap;
	}

	size_t tagOpen;
	while (getline(templateFile, line))
	{
		lineNum++;
		index = 0;
		tagOpen = 0;
		while (tagOpen < line.size())
		{
			string tag = StringUtils::GetDataBetweenChars(line, '[', ']', tagOpen);

			if (tag.empty())
				continue;

			it = tagMap.find(tag);

			//not int he map, add it
			if (it != tagMap.end())
			{
				tagMap[tag] = 1;
			}
			//else add to num times we seen this tag
			else
			{
				tagMap[tag]++;
			}
		}

	}

	templateFile.close();
	return tagMap;
}
	