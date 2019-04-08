#ifndef WORDLIST_H
#define WORDLIST_H
#include 'WordWithFreq.h'
#include <vector>
#include <fstream>
class wordList
{
public:
	wordList()
	{
	}
	~wordList()
	{
	}
	
private:
 vector<WordWithFreq> Word_List;
};

#endif // WORDLIST_H
