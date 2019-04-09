#ifndef WORDFORMATTER_H
#define WORDFORMATTER
#include <vector>
#include <string>
#include <iostream>
#include <bits/stdc++.h> 
class wordFormatter
{//This class will provide functions which can be used to format words like reducing by delimeter and removing punctuation
public:
	wordFormatter()
	{
	}
	~wordFormatter()
	{
	}
	std::vector<std::string> SeparateWordsBySpace(const std::string* inLine)
	 {//Splits a line into separate words by spacve. https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/
		std::vector<std::string> result;
		std::istringstream ss(*inLine);	
		do{
			std::string word;
			ss >> word;
			result.push_back(word);
		}while(ss);
		return result;
	 }
	std::string removePunctuationfromWord(const std::string& word)
	{//Removes punctuation from a word. https://stackoverflow.com/questions/19138983/c-remove-punctuation-from-string
		std::string result;
		std::remove_copy_if(word.begin(), word.end(),            
                        std::back_inserter(result), //Store output           
                        std::ptr_fun<int, int>(&std::ispunct)  
                       );
		return result;
	}
};

#endif // WORDFORMATTER_H
