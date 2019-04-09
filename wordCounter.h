#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H
#include 'WordWithFreq.h'
#include <vector>
#include <fstream>
#include <string>
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
		ifstream inputFile(input.txt);		
		if(inputFile.is_open())
		{
			while(getline(inputFile,line))
			{//Do stuff
				std::cout<<"line stuff: "<<line<<std::endl;
			}
			inputFile.close();
		} else
		{
			throw "File not found!";
		}
		
	}
private:
 vector<WordWithFreq> Word_List;
 vector<string> wordSeparator()
 {//Separates the words in a line into individual strings
	 
 }
};

#endif // WORDCOUNTER_H
