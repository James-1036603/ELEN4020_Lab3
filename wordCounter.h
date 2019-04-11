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
	
	void getWordsFromFile(std::string fileName = "input.txt")
	{//Will read the words in a file and determine each word's frequency
		std::string line;
		std::ifstream inputFile(fileName);
		std::vector<std::string> lines;
        std::vector<std::string> formattedWords;
		if(inputFile.is_open())
		{
			std::cout<<"Opened file successfully"<<std::endl;
			while(getline(inputFile,line))
			{
				lines.push_back(line);
			}
			inputFile.close();
			formattedWords = doFormatting(lines);
            groupWords(formattedWords);
		} else
		{
			throw "File not found!";
		}
		
	}
private:
 std::vector<WordWithFreq> Word_List;
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
     return result;
 }
 
 
 void groupWords(const std::vector<std::string>& inWords)
 {//Groups words together and increases their frequency
    bool found = 0;
    int foundPosition;
    for(auto i = 0; i < inWords.size(); i++)
    {//Iterate through each word
        for(auto j = 0; j < Word_List.size(); j++)
        {//Search each word in the word list
            if(inWords.at(i) == Word_List.at(j).word)
            {                
                found = 1;
                foundPosition = j;
                break;
            }
        }
        
        if(found)
        {//If the word was found, increase the frequency at that position
            Word_List.at(foundPosition).frequency++;
        } else{//Else, add the word to the list
            WordWithFreq temp;
            temp.word = inWords.at(i);
            temp.frequency = 1;
            Word_List.push_back(temp);
        }
        found = 0;
    }
     
 }

};

#endif // WORDCOUNTER_H
