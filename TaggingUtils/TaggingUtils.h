#pragma once

#include "DatabaseController.h"
#include <string>
#include <vector>

//used to add tags/hashtags to my stuff and to interact with sqlite
#include <vector>
#include <string>
#include "BaseDBInterface.h"

class TaggingUtils : public BaseDBInterface
{
	int AddItemTagsEntry(int itemID,int tagID);
	static const std::string TAGS_TABLE;
	static const std::string ITEMS_TABLE;
	static const std::string ITEM_TAGS_TABLE;
	//bool hasCustomItemFields;
public:
	using BaseDBInterface::BaseDBInterface;
	std::string dbName;
	void CreateTagTables(DatabaseController* dbCtrl,std::vector<DatabaseController::dbDataPair> &extraInfo);
	int AddNewItem(std::string itemName,vector<DatabaseController::dbDataPair> &extraInfo );
	int AddNewItem(std::string itemName,std::string content );
	int AddTag(std::string tagName );
	int TagItem(std::string itemName, std::string tagName);
	int TagItem(std::string itemName, std::vector<std::string> tags);

	int TaggingUtils::GetId(std::string table, std::string itemName);
	int TaggingUtils::GetItemId(std::string itemName);
	int TaggingUtils::GetTagId(std::string tagName);
};