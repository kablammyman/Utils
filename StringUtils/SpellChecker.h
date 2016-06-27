#pragma once

#include <string>
#include <vector>

namespace StringUtils
{ 
	class SpellChecker
	{
	
	private:
		std::vector<std::string> dictionary;
		size_t levenshtein_distance(std::string first, std::string second);
	public:
		SpellChecker();
		SpellChecker(std::vector<std::string> &dict);

		void AddWordToDictionary(std::string word);
		void AddWordsToDictionary(std::vector<std::string> words);
		std::vector<std::string> nearest_words(const std::string& word);
	
	};
}
