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
	//inherit from me if you use a custom item table (meaning you have more than a content field)
	struct TaggedItem
	{
		std::string name;
		int id;
		std::string content;
	};
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
	bool DeleteItem(std::string itemName);
	bool DeleteItem(int itemID);

	std::vector<std::string> GetAllTagsForItem(int itemID);
	std::vector<TaggedItem> GetAllItemsWithAnyOfTheseTags(std::vector<std::string> tags);
	std::vector<TaggedItem> GetAllItemsWithALLOfTheseTags(std::vector<std::string> tags);
	
	//how cani do this when i inherit without creating a all this all over again?
	TaggedItem GetItemFromID(int id /*, std::vector<std::string> itemFields*/);
	std::vector<TaggedItem> GetAllItemsWithTag(std::string tag /*,std::vector<std::string> itemFields*/);

	//not sure if this will be a thing...
	void RunAutoTagger(std::string text);
};