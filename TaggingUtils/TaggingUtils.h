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
	void CreateTagTables();
	void CreateTagTables(std::vector<DatabaseController::dbDataPair> &extraInfo);

	//the id can be sued to override the auto increment in case you want to "reuse" the id with one another table created
	int AddNewItem(std::string itemName,vector<DatabaseController::dbDataPair> &extraInfo, int id = -1 );
	int AddNewItem(std::string itemName,std::string content,int id = -1 );
	int AddTag(std::string tagName );
	int TagItem(std::string itemName, std::string tagName);
	int TagItem(std::string itemName, std::vector<std::string> tags);
	int TagItem(int itemID, std::string tagName);
	int TagItem(int itemID, std::vector<std::string> tags);

	int GetId(std::string table, std::string itemName);
	int GetItemId(std::string itemName);
	int GetTagId(std::string tagName);
	int GetItemTagId(int itemID,int tagID);
};