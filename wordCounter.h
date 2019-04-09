#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H
#include "WordWithFreq.h"
#include "wordFormatter.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
class wordCounter
{
public:
	wordCounter()
	{
		
	}
	~wordCounter()
	{
	}
	
	void getWordsFromFile(string fileName = "input.txt")
	{//Will read the words in a file and determine each word's frequency
		string line;
		ifstream inputFile(fileName);
		std::vector<std::string> lines;
		if(inputFile.is_open())
		{
			std::cout<<"Opened file successfully"<<std::endl;
			while(getline(inputFile,line))
			{
				lines.push_back(line);
			}
			inputFile.close();
			doFormatting(lines);
		} else
		{
			throw "File not found!";
		}
		
	}
private:
 vector<WordWithFreq> Word_List;
 wordFormatter formatter;
 
 std::vector<std::string> doFormatting(const std::vector<std::string>& lines)
 {//Does the line formatting into words
	 std::vector<std::string> result;
	 std::vector<std::string> tempWords;	 
	 for(auto i = 0; i < lines.size(); i++)
	 { 
		 tempWords = formatter.SeparateWordsBySpace(&lines.at(i));
		 //put all the words to a common vector
		 for(auto i = 0; i<tempWords.size(); i++)
		 {
			 result.push_back(tempWords.at(i));
			 result.at(i) = formatter.removePunctuationfromWord(result.at(i));
		 }
	 }
	 
	 for(auto i = 0; i < result.size(); i++){
		 std::cout<<result.at(i)<<std::endl;
	 }
 }
 
};

#endif // WORDCOUNTER_H
