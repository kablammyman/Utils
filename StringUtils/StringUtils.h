#pragma once

#include <vector>
#include <string>
#include <map>
#include "DocumentTags.h"

class StringUtils
{
public:
	static std::vector<std::string> Tokenize(std::string path, char delim);
	static std::vector<std::string> Tokenize(std::string path, std::string delims);
	static std::vector<std::string> Tokenize2(std::string path, std::string delims);
	
	static void ToProperNoun(std::string &input);
	static void ToUpper(std::string &input);
	static void ToLower(std::string &input);
	
	static bool IsUpper(std::string input);
	static bool IsLower(std::string input);
	static bool IsProperNoun(std::string input);

	static std::string FlattenVector(std::vector<std::string> input); 
	static void TrimWhiteSpace(std::string &input);
	
	static std::string GetDataBetweenChars(std::string line,char char1, char char2, size_t &start);//no default valuer here
	static std::string GetDataBetweenSubStrings(std::string line, std::string str1, std::string str2, size_t start =0);
	
	static void StringSplit(std::string orig, std::string &str1, std::string &str2, size_t index, bool removeIndexChar = false);
	static std::string CopyCharsBetweenQuotes(std::string word, size_t index = 0);
	static std::string WrapStringInSingleQuotes(std::string text);
	static std::string WrapStringInDoubleQuotes(std::string text);

	static std::string CopyCharsBetweenChars(std::string word, char open, char closed, size_t index = 0);
	static std::string CopyCharsBetweenHTMLTags(std::string html, std::string openTag, std::string closeTag, size_t& offset);
	static std::vector<std::string> CopyStringBetweenIdenticalTags(std::string html, std::string openTag, std::string closeTag, size_t& offset);

	static bool IsWordFromListInLine(std::vector<std::string> &wordList, std::string line, bool isCaseSensitive = true);
	static int GetStandAloneWordInLineIndex(std::string line, std::string word);
	
	static std::string GetWordFromListInLine(std::vector<std::string> &wordList,std::string line, bool isCaseSensitive = true);
	static int GetIndexOfWordFromListInLine(std::vector<std::string> &wordList,std::string line, bool isCaseSensitive = true);
	static int GetIndexOfWordFromList(std::vector<std::string>& wordList, std::string line, bool isCaseSensitive = true);
	static bool IsPostCharNonAlpha(std::string line,std::string word);
	static bool IsPrevCharNonAlpha(std::string line,std::string word);
	static bool IsNumber(std::string word);
	static std::string GetAlphaNumericString(std::string line, size_t index, std::string bonusChars = "");
	static std::string CopyUntilChar(std::string line,size_t index, char delim);
	static std::string GetRandomString(int size);

	static std::string FindAndReplace(std::string orig, std::string findToken, std::string replaceToken);
	static std::string StringClean(std::string orig, bool includeNewLines);
	static std::string EmailStringClean(std::string orig);
	static void SanitizeSQLDataString(std::string& value);
	//this may go into its own file....
	static void SanitizeJsonString(std::string& value);
	static void DesanitizeJsonString(std::string& value);
	static std::string CreateJsonEntry(std::string name, std::string value, bool noQuotes,bool lastEntry);
	
	static std::string CreateJsonEntry(std::string name, int val,bool lastEntry=false);
	static std::string CreateJsonEntry(std::string name, float val,bool lastEntry=false);
	static std::string CreateJsonEntry(std::string name, bool val,bool lastEntry=false);
	static std::string CreateJsonEntry(std::string name, std::string val,bool lastEntry=false);
	static std::string CreateJsonEntry(std::string name, const char* val,bool lastEntry=false);
	static std::string CreateCompleteErrorJson(std::string message, std::string methodName = "");
	static std::string CreateCompleteOKJson(std::string message ="");

	static std::string CreateJsonArrayEntry(std::string name, std::vector<std::string> value, bool noQuotes = false);
	static std::string CreateJsonArrayEntry(std::string name, std::vector<int> value);
	static std::string ConvVecToJsonObj(std::vector<std::string> &values);

	
	static std::string CreateJsonNestedObjectEntry(std::string name, std::vector<std::pair <std::string, std::string>> entries, bool noQuotes);
	static std::string ReturnErrorAsJSONObj(std::string err);

	static std::string GetJsonEntryValue(std::string& json, std::string name);
	static int GetJsonEntryIntValue(std::string& json, std::string name);
	static float GetJsonEntryFloatValue(std::string& json, std::string name);
	static bool GetJsonEntryBoolValue(std::string& json, std::string name);
	static void RemoveLastCommaFromJson(std::string &json);

	static float GetFloatFromString(std::string str);
	static double GetDoubleFromString(std::string str);
	static int GetIntFromString(std::string str);
	static std::string ToMoneyString(float amount, bool prettyPrint = true);
	static std::string ToPercentString(float amount, bool convertFloatToPercet = true);
	static std::string GetRandomAlphaNumericString(int size);
	static int HammingDistance(std::string str1, std::string str2);
	static std::string LongestCommonSubStr(std::string str1, std::string str2);
	static bool IsSameWord(std::string word1, std::string word2);

	static std::string ConvertToE164(std::string input);
	static std::string PrettyPhone(std::string phoneToUpdate);
	static bool IsValidPhone(std::string phone);
	static std::string UrlDecode(std::string urlEncoded, char delim = '%');
	static std::string UnEscapeString(std::string origString);

	static std::map<std::string, int> GetAllTagsInFile(std::string filePath);
	static void LoadRotatorMessagesFromFile(std::string file, std::vector<std::string> &messages);
	static std::string ReplaceTagForValue(std::string input, DocumentTags tagMap);
	static int InsertMultilineInput(std::string replaceMe, std::string lines, size_t index, std::string newText);
	static std::string GetRandomWord(std::vector <std::string> &lists);
};