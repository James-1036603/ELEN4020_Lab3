#ifndef WORDFORMATTER_H
#define WORDFORMATTER_H
#include <algorithm>
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>

class wordFormatter
{ // This class will provide functions which can be used to format words like reducing by delimeter and removing
  // punctuation
public:
    wordFormatter()
    {
    }
    ~wordFormatter()
    {
    }
    std::vector<std::string> SeparateWordsBySpace(const std::string* inLine)
    { // Splits a line into separate words by spacve. https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/
	std::vector<std::string> result;
	std::istringstream ss(*inLine);
	do {
	    std::string word;
	    ss >> word;
	    result.push_back(word);
	} while(ss);
	return result;
    }
    std::string removePunctuationfromWord(const std::string& word)
    { // Removes punctuation from a word. https://stackoverflow.com/questions/19138983/c-remove-punctuation-from-string
	std::string result;
	// Remove punctuation
	result = word;
	for(int i = 0, len = result.size(); i < len; i++) {

	    if(ispunct(result.at(i))) {
		result.erase(i--, 1);
		len = result.size();
	    }
	}
	// Take to lower case
	for(auto i = 0; i < result.size(); i++) {
	    result.at(i) = std::tolower(result.at(i));
	}

	return result;
    }
};

#endif // WORDFORMATTER_H
