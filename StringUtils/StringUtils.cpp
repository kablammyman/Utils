#include "StringUtils.h"

std::vector<std::string> StringUtils::Tokenize(std::string path, std::string delims)
{
	std::vector<std::string> returnVec;
	char *next_token = NULL;
	char *p = strtok_s(const_cast<char *>(path.c_str()), delims.c_str(), &next_token);
	while (p)
	{
		//printf ("Token: %s\n", p);
		returnVec.push_back(p);
		p = strtok_s(NULL, delims.c_str(), &next_token);
	}
	return returnVec;
}

//turns words or sentences into all proper nouns
void StringUtils::ToProperNoun(std::string &input)
{
	bool caps = true;
	for (size_t i = 0; i < input.size(); i++)
	{

		if (input[i] == ' ' || input[i] == '-' || input[i] == '_')
		{
			caps = true;
			continue;
		}
		if (caps)
		{
			input[i] = toupper(input[i]);
			caps = false;
		}
		else
			input[i] = tolower(input[i]);
	}
}
void StringUtils::CreateTrie()
{
	if (trie != NULL)
		return;
	trie = new Trie();
}
void StringUtils::CreateSpellCheck()
{
	if (spellchcek != NULL)
		return;
	spellchcek = new SpellChecker();
}
void StringUtils::AddWordToDictionary(std::string word)
{
	CreateSpellCheck();
	spellchcek->AddWordToDictionary(word);
}
void StringUtils::AddAllWordsToDictionary(std::vector<string> words)
{
	CreateSpellCheck();
	spellchcek->AddWordsToDictionary(words);
}
std::vector<std::string> StringUtils::GetNearestWords(const std::string& word)
{
	CreateSpellCheck();
	return spellchcek->nearest_words(word);
}

void StringUtils::AddWordToTrie(std::string s)
{
	CreateTrie();
	trie->addWord(s);
}
bool StringUtils::SearchWordInTrie(std::string s)
{
	CreateTrie();
	return trie->searchWord(s);
}
void StringUtils::DeleteWordInTrie(std::string s)
{
	CreateTrie();
	trie->deleteWord(s);
}