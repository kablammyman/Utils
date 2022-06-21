#pragma once

#include <string>
#include <vector>
#include <map>

typedef std::pair <std::string, std::string> TagDataPair;

struct DocumentTags
{
	std::vector<TagDataPair> tags;
	bool isRtfFile = true;
	std::string GetTagValue(std::string tagName);
	size_t GetTagValueIndex(std::string tagName);

	size_t GetDistinctTagCount();
	std::string GetTagNameAt(size_t index);
	std::string GetTagValueAt(size_t index);
	std::vector<std::string> GetMissingFields();

	void AddOrUpdateTagAndValue(std::string tagName, std::string value);
	void AddOrUpdateTagAndValue(std::string tagName, int value);
	void AddOrUpdateTagAndValue(std::string tagName, float value);
};

class Tags
{
public:
	static std::string ReplaceTagForValue(std::string input, DocumentTags tagMap);
	static int InsertMultilineInput(std::string replaceMe, std::string lines, int index, std::string newText);
	static void CreateNewFileFromTagTemaplateFile(std::string templatePath, std::string newFilePath, DocumentTags tagMap);
	static std::map<std::string,int> GetAllTagsInFile(std::string filePath);
	static std::string GetRandomWord(std::vector <std::string> &lists);
	static void LoadRotatorMessagesFromFile(std::string file, std::vector<std::string> &messages);
};