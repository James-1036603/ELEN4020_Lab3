#ifndef INVERTEDTEXT_H
#define INVERTEDTEXT_H
#include <vector>
#include <string>
#include "wordFormatter.h"
#include <iostream>
class InvertedText
{
public:
	InvertedText(std::vector<WordWithFreq> inWordList, std::string file = "input.txt")
	{//Initialises lines from text file
	getLinesFromFile(file);
	this->wordListForSearching = inWordList;
	}
	~InvertedText()
	{
	}
	
	void FindWordLines(int K = 10)
	{//Finds the first K words and their line numbers
		std::string stringToSearch;
		for(auto i = 0; i < K; i++)
		{
			stringToSearch = wordListForSearching.at(i).word;
			for(auto j = 0; j < lines.size() ; j++)
			{
				if(lines.at(j).find(stringToSearch))
					{
					wordListForSearching.at(i).linesWhereAppearing.push_back(j+1);
					std::cout<<"word found in line: "<<j+1<<std::endl;
					}
			}
		}
	}
	
private:

std::vector<WordWithFreq> wordListForSearching;
std::vector<std::string> lines;
wordFormatter formatter;

void getLinesFromFile(std::string file = "input.txt")
{
	std::string line;
	std::ifstream inputFile(file);	
	if(inputFile.is_open()) {
	    std::cout << "Opened file successfully" << std::endl;
	    while(getline(inputFile, line)) {
			std::string temp = formatter.removePunctuationfromWord(line);
			lines.push_back(temp);
	    }
	    inputFile.close();
		
		for(auto x : lines)
			std::cout<<x<<std::endl;
	} else {
	    throw "File not found!";
	}
}


};

#endif // INVERTEDTEXT_H
