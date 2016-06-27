#pragma once

#include <vector>
#include <string>
#include "SpellChecker.h"
#include "Trie.h"

class StringUtils
{
private:
	//i need to make this so i can just create my own instances of spellchecker and trie...no need for static stuff
	static SpellChecker *spellchcek;
	static Trie *trie;
	static void CreateTrie();
	static void CreateSpellCheck();
public:
	static std::vector<std::string> Tokenize(std::string path, std::string delims);
	static void ToProperNoun(std::string &input);

	static void AddWordToDictionary(std::string word);
	static void AddAllWordsToDictionary(std::vector<string> words);
	static std::vector<std::string> GetNearestWords(const std::string& word);

	void StringUtils::AddWordToTrie(std::string s);
	bool StringUtils::SearchWordInTrie(std::string s);
	void StringUtils::DeleteWordInTrie(std::string s);
};