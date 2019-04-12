#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H
#include "WordWithFreq.h"
#include "wordFormatter.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
class wordCounter
{
public:
	wordCounter()
	{
		loadStopWords();
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
            descendingData();
		} else
		{
			throw "File not found!";
		}
		
	}
    
    void printWordList()
 {
     for(auto x : Word_List)
     {
         std::cout<<x.word<<" "<<x.frequency<<std::endl;
     }
 }
 
 
  void printTopKWords(int K)
{
    if(K > Word_List.size())
    {
        throw "K range is too large";
    } else {
        std::cout<<"Printing top "<<K<<" words:\n";
        for(auto i = 0; i < K; i++)
            std::cout<<Word_List.at(i).word<<std::endl;
    }
}
 
private:
 std::vector<WordWithFreq> Word_List;
 std::vector<std::string> StopWords;
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
     return result;
 }
 

 
 void groupWords(const std::vector<std::string>& inWords)
 {//Groups words together and increases their frequency
    bool found = 0;
    bool isAStopWord;
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
        //Check if is stop word
        isAStopWord = isStopWord(inWords.at(i));
        
        if(found && !isAStopWord)
        {//If the word was found, increase the frequency at that position
            Word_List.at(foundPosition).frequency++;
        } else if (!isAStopWord){//Else, add the word to the list
            WordWithFreq temp;
            temp.word = inWords.at(i);
            temp.frequency = 1;
            Word_List.push_back(temp);
        }
        found = 0;
    }
    
     
 }



 void loadStopWords()
 {//Loads the stop words from file
        std::string line;
		std::ifstream inputFile("stopWords.txt");		
		if(inputFile.is_open())
		{
			while(getline(inputFile,line))
			{
				StopWords.push_back(line);
			}
		} else
		{
			throw "File not found!";
		}
 }
 
 bool isStopWord(std::string word)
 {//Checks wheter the word is a stop word
     bool isStopWord = 0;
     for(auto i = 0; i < StopWords.size(); i++)
     {
         if(word == StopWords.at(i))
         {
             isStopWord = 1;
             break;
         }
     }
     return isStopWord;
 }
 
void descendingData()
{//Organises the frequency of the stop words in descending order
    //Using bubble sort as Sort was giving errors
    for(auto i = 0; i < Word_List.size()-1; i++)
    {
     for(auto j = 0; j < Word_List.size()-1; j++){   
        if(Word_List.at(j).frequency < Word_List.at(j+1).frequency)
        {
            WordWithFreq temp = Word_List.at(j);
            Word_List.at(j) = Word_List.at(j+1);
            Word_List.at(j+1) = temp;
        }
        
     }
    }
}

};

#endif // WORDCOUNTER_H
