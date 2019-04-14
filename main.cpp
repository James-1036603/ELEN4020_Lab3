#include <iostream>
#include <stdio.h>
#include "wordCounter.h"
#include "InvertedText.h"
using namespace std;

int main()
{

    try {
	wordCounter myCounter;
	myCounter.getWordsFromFile();
	myCounter.printWordList();
	
	int K = 10;
	myCounter.printTopKWords(K);
	K = 20;
	myCounter.printTopKWords(K);
	
	InvertedText invT(myCounter.wordList());
	invT.FindWordLines();
    } catch(const char* e) {
	cout << e << endl;
    }
    return 0;
}
