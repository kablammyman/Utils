#pragma once

#include <vector>
#include <string>

class StringUtils
{
public:
	static std::vector<std::string> Tokenize(std::string path, char delim);
	static std::vector<std::string> Tokenize(std::string path, std::string delims);
	static std::vector<std::string> Tokenize2(std::string path, std::string delims);
	static void ToProperNoun(std::string &input);
	static void ToUpper(std::string &input);
	static void ToLower(std::string &input);
	static std::string FlattenVector(std::vector<std::string> input); 
	static void TrimWhiteSpace(std::string &input);
	static std::string GetDataBetweenChars(std::string line,char char1, char char2, size_t &start);//no default valuer here
	static std::string GetDataBetweenSubStrings(std::string line, std::string str1, std::string str2, size_t start =0);
	static bool IsWordFromListInLine(std::vector<std::string> &wordList,std::string line);
	static int GetStandAloneWordInLineIndex(std::string line,std::string word);
	static std::string CopyCharsBetweenQuotes(std::string word, size_t index = 0);
	static std::string CopyCharsBetweenTags(std::string word, char open,char closed,size_t index = 0);//this one has default value
	static std::string GetWordFromListInLine(std::vector<std::string> &wordList,std::string line, bool isCaseSensitive = true);
	static int GetIndexOfWordFromListInLine(std::vector<std::string> &wordList,std::string line);
	static int GetIndexOfWordFromList(std::vector<std::string>& wordList, std::string line);
	static bool IsPostCharNonAlpha(std::string line,std::string word);
	static bool IsPrevCharNonAlpha(std::string line,std::string word);
	static bool IsNumber(std::string word);
	static std::string GetAlphaNumericString(std::string line,int index, std::string bonusChars = "");
	static std::string CopyUntilChar(std::string line,int index, char delim);
	static std::string GetRandomString(int size);

	static std::string FindAndReplace(std::string orig, std::string findToken, std::string replaceToken);
	static std::string StringClean(std::string orig, bool includeNewLines);

	//this may go into its own file....
	static void SanitizeJsonString(std::string& value);
	static void DesanitizeJsonString(std::string& value);
	static std::string CreateJsonEntry(std::string name, std::string value, bool noQuotes,bool lastEntry);
	
	static std::string CreateJsonEntry(std::string name, int val,bool lastEntry=false);
	static std::string CreateJsonEntry(std::string name, float val,bool lastEntry=false);
	static std::string CreateJsonEntry(std::string name, bool val,bool lastEntry=false);
	static std::string CreateJsonEntry(std::string name, std::string val,bool lastEntry=false);
	static std::string CreateJsonEntry(std::string name, const char* val,bool lastEntry=false);

	static std::string CreateJsonArrayEntry(std::string name, std::vector<std::string> value, bool noQuotes);
	static std::string CreateJsonArrayEntry(std::string name, std::vector<int> value);
	static std::string GetJsonEntryValue(std::string& json, std::string name);
	static std::string CreateJsonNestedObjectEntry(std::string name, std::vector<std::pair <std::string, std::string>> entries, bool noQuotes);


	static int GetJsonEntryIntValue(std::string& json, std::string name);
	static float GetJsonEntryFloatValue(std::string& json, std::string name);
	static float GetFloatFromString(std::string str);
	static int GetIntFromString(std::string str);
	static std::string ToMoneyString(float amount, bool prettyPrint = true);
	static std::string GetRandomAlphaNumericString(int size);
	static int HammingDistance(std::string str1, std::string str2);
	static std::string LongestCommonSubStr(std::string str1, std::string str2);
};