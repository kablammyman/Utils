#include "TaggingUtils.h"
 
using namespace std;
//http://howto.philippkeller.com/2005/04/24/Tags-Database-schemas/

const std::string TaggingUtils::TAGS_TABLE = "Tags";
const std::string TaggingUtils::ITEMS_TABLE = "Items";
const std::string TaggingUtils::ITEM_TAGS_TABLE = "ItemTags";

void TaggingUtils::CreateTagTables()
{
	vector<DatabaseController::dbDataPair> extraInfo;
	CreateTagTables(extraInfo);
}

void TaggingUtils::CreateTagTables(vector<DatabaseController::dbDataPair> &extraInfo)
{
	
	string output;
	vector<DatabaseController::dbDataPair> fields;
	std::vector<std::string> currentTables;
	dbController->GetAllTablesInDB(currentTables);
	
	//if we already made TAGS_TABLE then we prob have the other 2...and that menas no need to create anything
	for(size_t i = 0; i < currentTables.size(); i++)
	{
		if(currentTables[i] == TAGS_TABLE)
			return;
	}

	fields.push_back(make_pair("ID", "INTEGER PRIMARY KEY AUTOINCREMENT"));
	fields.push_back(make_pair("Name", "TEXT"));

	if(extraInfo.empty())
		fields.push_back(make_pair("Content", "TEXT"));
	else
	{
		for(size_t i = 0; i < extraInfo.size(); i++)
		{
			fields.push_back(extraInfo[i]);
		}
	}
	//the item are objects we need t tag/describe.

	//liek cars, emais, files, bookmarks, etc
	//the extraInfo are the different fields we may want to ad
	//otherwise, we hae a stock column of "content" to give more data about the item
	dbController->CreateTable(ITEMS_TABLE, fields, output);


	fields.clear();
	fields.push_back(make_pair("ID", "INTEGER PRIMARY KEY AUTOINCREMENT"));
	fields.push_back(make_pair("ItemID", "INTEGER"));
	fields.push_back(make_pair("TagID", "INTEGER"));
	//the middle man between items and the tags.
	//we need this because items can have more than 1 tag
	//and tags can belong to more than 1 item
	//ex item1 can be a car, and the tags can be red,sedan,fast,
	//item 2 can be a truck and the tags can be red, slow, pickup
	//notice red is tagged with both the truck and car
	dbController->CreateTable(ITEM_TAGS_TABLE, fields, output);

	fields.clear();
	fields.push_back(make_pair("ID", "INTEGER PRIMARY KEY AUTOINCREMENT"));
	fields.push_back(make_pair("Name", "TEXT"));
	//these are teh actual tags\words that we can use
	//ex: red, sedan,truck, slow, fast, etc
	dbController->CreateTable(TAGS_TABLE, fields, output);
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::AddTag(std::string tagName )
{
	string output;
	DatabaseController::dbDataPair data = make_pair("Name",tagName);
	
	if (!dbController->InsertNewDataEntry(TAGS_TABLE,data ,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "AddTag error: " + dbController->GetLastError();
			return -1;
		}
	}

	return dbController->GetLatestRowID();
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::AddNewItem(std::string itemName,vector<DatabaseController::dbDataPair> &extraInfo, int id )
{
	
	string output;
	vector<DatabaseController::dbDataPair> data;
	if(id > -1)
		DatabaseController::AddIntDataToQuerey(data, "ID",id);
	DatabaseController::AddStringDataToQuerey(data, "Name",itemName);
	for(size_t i = 0; i < extraInfo.size(); i++)
	{
		data.push_back(extraInfo[i]);
	}

	if (!dbController->InsertNewDataEntry(ITEMS_TABLE,data ,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "AddNewItem error: " + dbController->GetLastError();
		}
	}

	return dbController->GetLatestRowID();
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::AddNewItem(std::string itemName,std::string content, int id )
{
	string output;
	vector<DatabaseController::dbDataPair> data;
	if(id > -1)
		DatabaseController::AddIntDataToQuerey(data, "ID",id);
	DatabaseController::AddStringDataToQuerey(data, "Name",itemName);
	DatabaseController::AddStringDataToQuerey(data, "Content", content);

	if (!dbController->InsertNewDataEntry(ITEMS_TABLE,data ,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "AddNewItem error: " + dbController->GetLastError();
		}
	}

	return dbController->GetLatestRowID();
}
//---------------------------------------------------------------------------------------------------------------
//this is private since we control this
int TaggingUtils::AddItemTagsEntry(int itemID,int tagID)
{
	string output;
	vector<DatabaseController::dbDataPair> data;
	DatabaseController::AddIntDataToQuerey(data, "ItemID",itemID);
	DatabaseController::AddIntDataToQuerey(data, "TagID", tagID);

	if (!dbController->InsertNewDataEntry(ITEM_TAGS_TABLE,data ,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "AddItemTagsEntry error: " + dbController->GetLastError();
		}
	}

	return dbController->GetLatestRowID();
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::TagItem(std::string itemName, std::string tagName)
{
	int itemID =  GetItemId(itemName);
	if(itemID < 1)
	{
		lastError.errorMessage= "this item doesnt exist in the db yet...you gotta add it first";
		return -1;
	}

	int tagID =  GetTagId(tagName);
	if(tagID < 1)
	{
		//we CAN add the tag if it doesnt exist yet since tags are simple to add
		tagID = AddTag(tagName );
	}

	return AddItemTagsEntry( itemID, tagID);
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::TagItem(std::string itemName, std::vector<std::string> tags)
{
	int itemID =  GetItemId(itemName);
	if(itemID < 1)
	{
		lastError.errorMessage= "this item doesnt exist in the db yet...you gotta add it first";
		return -1;
	}

	for(size_t i = 0; i < tags.size(); i++)
	{
		int tagID =  GetTagId(tags[i]);
		if(tagID < 1)
		{
			//we CAN add the tag if it doesnt exist yet since tags are simple to add
			tagID = AddTag(tags[i] );
		}
		AddItemTagsEntry( itemID, tagID);
	}

	return dbController->GetLatestRowID();
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::TagItem(int itemID, std::string tagName)
{
	
	int tagID =  GetTagId(tagName);
	if(tagID < 1)
	{
		//we CAN add the tag if it doesnt exist yet since tags are simple to add
		tagID = AddTag(tagName );
	}
	//only add if the link between these 2 dont exist
	int ret = GetItemTagId(itemID,tagID);
	if(ret == -1)
		return AddItemTagsEntry( itemID, tagID);
	return ret;
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::TagItem(int itemID, std::vector<std::string> tags)
{
	for(size_t i = 0; i < tags.size(); i++)
	{
		int tagID =  GetTagId(tags[i]);
		if(tagID < 1)
		{
			//we CAN add the tag if it doesnt exist yet since tags are simple to add
			tagID = AddTag(tags[i] );
		}

		//only add if the link between these 2 dont exist
		if(GetItemTagId(itemID,tagID) == -1)
			AddItemTagsEntry( itemID, tagID);
	}

	return dbController->GetLatestRowID();
}
//---------------------------------------------------------------------------------------------------------------
//this will work for any of the 3 tables, since they all have an ID and a name
int TaggingUtils::GetId(string table, string itemName)
{
	string output;
	string querey = "SELECT ID FROM "+ table + " WHERE Name = '" + itemName + "'"; 

	if (!dbController->DoDBQuerey(querey, output))
	{
		if (dbController->GetLastError() != "")
		{
			lastError.errorMessage= "Error getting id: " + dbController->GetLastError();
			return -1;
		}
	}

	int itemID = dbController->GetIdFromQuereyResult(output);

	if(itemID < 1)
	{
		lastError.errorMessage= "this doesnt exist in the db yet...you gotta add it first";
		return -1;
	}
	return itemID;
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::GetTagId(std::string tagName)
{
	return GetId(TAGS_TABLE, tagName);
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::GetItemId(std::string itemName)
{
	return GetId(ITEMS_TABLE, itemName);
}
//---------------------------------------------------------------------------------------------------------------
int TaggingUtils::GetItemTagId(int itemID,int tagID)
{
	string output;
	string querey = "SELECT ID FROM "+ ITEM_TAGS_TABLE+" WHERE ItemID = "+ to_string(itemID) + " AND TagID = " + to_string(tagID);
	if (!dbController->DoDBQuerey(querey,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "GetItemTagsEntry error: " + dbController->GetLastError();
		}
	}

	return dbController->GetIdFromQuereyResult(output);
}
//---------------------------------------------------------------------------------------------------------------
vector<string> TaggingUtils::GetAllTagsForItem(int itemID)
{
	string output;
	vector<string> ret;
	string querey = "SELECT t.Name FROM "+ITEM_TAGS_TABLE+" it, "+ITEMS_TABLE +" i, "+TAGS_TABLE+" t WHERE ";
	querey += " it.ItemID = "+to_string(itemID)+" AND it.TagID = t.ID AND it.ItemID = i.ID";

	if (!dbController->DoDBQuerey(querey,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "GetAllTagsForItem error: " + dbController->GetLastError();
			return ret;
		}
	}

	vector <DatabaseController::DBResult>  dbResults;
	//notice the differnte PArseDBOutput method (i think this was original one)
	DatabaseController::ParseDBOutput(output,1, dbResults);
	

	if(!dbResults.empty())
	{
		//notice we have to crack open the 1 dbResult oject to get to the sweet meat of the tags
		for (int i = 0; i < dbResults[0].data.size(); i++)
		{
			string tag = dbResults[0].data[i].second;
			ret.push_back(tag);
		}
	}
	return ret;
}
//---------------------------------------------------------------------------------------------------------------
TaggingUtils::TaggedItem TaggingUtils::GetItemFromID(int id)
{
	TaggedItem ret;
	string output;
	vector<string> itemFields = {"ID","Name","Content"};
	string fields = StringUtils::FlattenVector(itemFields);
	string querey = "SELECT " + fields +" FROM " + ITEMS_TABLE + " WHERE ID = " + to_string(id);
	
	if (!dbController->DoDBQuerey(querey,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "GetItemFromID error: " + dbController->GetLastError();
			return ret;
		}
	}

	if(output.empty())
		return ret;

	vector <DatabaseController::DBResult>  dbResults;
	DatabaseController::ParseDBOutput(output,itemFields, dbResults);

	ret.id = id;
	ret.content = dbResults[0].GetValueFromKey("Content");
	ret.name = dbResults[0].GetValueFromKey("Name");

	return ret;
}
//---------------------------------------------------------------------------------------------------------------
vector<TaggingUtils::TaggedItem> TaggingUtils::GetAllItemsWithTag(string tag)
{
	vector<TaggedItem> ret;
	//vector<string> itemFields = {"ID","Name","Content"};
	int tagID = GetTagId(tag);
	string output;
	string querey = "SELECT ItemID FROM " + ITEM_TAGS_TABLE + " WHERE TagID = " + to_string(tagID);
	if (!dbController->DoDBQuerey(querey,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "GetAllItemsWithTag error: " + dbController->GetLastError();
			return ret;
		}
	}

	vector <DatabaseController::DBResult>  dbResults;
	//notice the differnte PArseDBOutput method (i think this was original one)
	DatabaseController::ParseDBOutput(output,1, dbResults);

	if(dbResults.empty())
		return ret;
	
	//notice we have to crack open the 1 dbResult oject to get to the sweet meat of the tags
	for (size_t i = 0; i < dbResults[0].data.size(); i++)
	{
		//TaggedItem nextItem = GetItemFromID( dbResults[i].GetIntValueFromKey("ItemID") );
		TaggedItem nextItem = GetItemFromID( stoi(dbResults[0].data[i].second ) );
		ret.push_back(nextItem);
	}
	

	return ret;
}

//---------------------------------------------------------------------------------------------------------------
vector<TaggingUtils::TaggedItem> TaggingUtils::GetAllItemsWithAnyOfTheseTags(vector<string> tags)
{
	vector<TaggedItem> ret;
	//vfirst get all of the tag ids
	string output;
	//string tagsString = StringUtils::FlattenVector(tags);
	string querey = "SELECT ID FROM " + TAGS_TABLE + " WHERE ";
	for(size_t i = 0; i < tags.size(); i++)
	{
		if(i == 0)
			querey += "Name = '" + tags[i] + "'";
		else
			querey += " OR Name = '" + tags[i] + "'";

	}
	
	if (!dbController->DoDBQuerey(querey,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "GetAllItemsWithAnyOfTheseTags error: " + dbController->GetLastError();
			return ret;
		}
	}

	vector <DatabaseController::DBResult>  dbResults;
	//notice the differnte PArseDBOutput method (i think this was original one)
	DatabaseController::ParseDBOutput(output,1, dbResults);

	if(dbResults.empty())
		return ret;

	//notice we have to crack open the 1 dbResult oject to get to the sweet meat of the tags
	for (size_t i = 0; i < dbResults[0].data.size(); i++)
	{
		//TaggedItem nextItem = GetItemFromID( dbResults[i].GetIntValueFromKey("ItemID") );
		TaggedItem nextItem = GetItemFromID( stoi(dbResults[0].data[i].second ) );
		ret.push_back(nextItem);
	}

	return ret;
}
//---------------------------------------------------------------------------------------------------------------
vector<TaggingUtils::TaggedItem> TaggingUtils::GetAllItemsWithALLOfTheseTags(vector<string> tags)
{
	vector<TaggedItem> ret;
	vector<string> itemFields = {"ID","Name","Content"};
	string output;
	string tagsString;
	//cant use regular flatten becasue we need to surround each keyword with quotes
	for(size_t i =0; i < tags.size(); i++)
	{
		tagsString += "'" + tags[i] + "',";
	}
	tagsString.pop_back();//remove last instance of comma

	string querey = "SELECT b.* FROM ItemTags bt, Items b, Tags t WHERE bt.TagID = t.ID ";
	querey += " AND (t.name IN ( " + tagsString + ")) AND b.ID = bt.ItemID GROUP BY b.ID HAVING COUNT( b.ID )= "+ to_string(tags.size());
	

	if (!dbController->DoDBQuerey(querey,output))
	{
		if(dbController->GetLastError() != "")
		{
			lastError.errorMessage= "GetAllItemsWithALLOfTheseTags error: " + dbController->GetLastError();
			return ret;
		}
	}

	vector <DatabaseController::DBResult>  dbResults;
	//notice the differnte PArseDBOutput method (i think this was original one)
	DatabaseController::ParseDBOutput(output,itemFields, dbResults);

	if(dbResults.empty())
		return ret;

	//this fancy querey already returns Item stuff
	for (size_t i = 0; i < dbResults.size(); i++)
	{
		//TaggedItem nextItem = GetItemFromID( dbResults[i].GetIntValueFromKey("ItemID") );
		TaggedItem nextItem;
		nextItem.id = dbResults[i].GetIntValueFromKey("ID");
		nextItem.content = dbResults[i].GetValueFromKey("Content");
		nextItem.name = dbResults[i].GetValueFromKey("Name");
		ret.push_back(nextItem);
	}

	return ret;
}

void TaggingUtils::RunAutoTagger(std::string text)
{
	//go thru the text, and finx any key words we want to use
	//eitehr find repeasted words (execpt for and,or, the, etc)
	//if a reapeated word appears more than X times, add a tag
}