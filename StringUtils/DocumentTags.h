#pragma once

#include <string>
#include <vector>
#include <map>

typedef std::pair <std::string, std::string> TagDataPair;

class DocumentTags
{
public:
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