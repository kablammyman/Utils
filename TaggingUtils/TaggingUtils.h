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
	static const std::string DEFAULT_ITEM_TABLE_NAME;
	static const std::string ITEM_TAGS_TABLE;
	
	std::string itemTableName;
	std::string itemNameColName;
	std::string itemContentColName;
	//bool hasCustomItemFields;
public:
	//inherit from me if you use a custom item table (meaning you have more than a content field)
	struct TaggedItem
	{
		std::string name;
		int id;
		std::string content;
	};
	TaggingUtils();
	using BaseDBInterface::BaseDBInterface;
	//std::string dbName;
	void OverrideItemTable(std::string newItemTable, std::string newNameCol, std::string newCotentName);
	//i rleaized that often times, I will already have a table for items (customer, owner, etc)
	//so having a 2nd table with the same info is dumb. 
	//so before I get rid of item table stuff completely, let me override the code and make sure im right
	
	void CreateTagTables();
	void CreateTagTables(std::vector<DatabaseController::dbDataPair> &extraInfo);

	//the id can be sued to override the auto increment in case you want to "reuse" the id with one another table created
	int AddOrUpdateItems(std::string itemName,vector<DatabaseController::dbDataPair> &extraInfo, int id = -1 );
	int AddOrUpdateItem(std::string itemName,std::string content);
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
	bool DeleteAllTagsForItem(int itemID);
	bool DeleteTagFromDB(int tagID);
	//not sure if this is needed
	//void DeleteUnusedTags(int tagID)
	void ResetItemTags(int itemID, std::vector<std::string> &curTags);

	std::vector<std::string> GetAllTagsForItem(int itemID);
	std::vector<TaggedItem> GetAllItemsWithAnyOfTheseTags(std::vector<std::string> tags);
	std::vector<TaggedItem> GetAllItemsWithALLOfTheseTags(std::vector<std::string> tags);
	
	//how cani do this when i inherit without creating a all this all over again?
	TaggedItem GetItemFromID(int id /*, std::vector<std::string> itemFields*/);
	std::vector<TaggedItem> GetAllItemsWithTag(std::string tag /*,std::vector<std::string> itemFields*/);

	//not sure if this will be a thing...
	void RunAutoTagger(std::string text);
};