#include <iostream>
#include <stdio.h>
#include <wordCounter.h>
#include "InvertedText.h"
using namespace std;

int main()
{

    try {
	wordCounter myCounter;
	myCounter.getWordsFromFile();
	myCounter.printWordList();
	
	InvertedText invT(myCounter.wordList());
	invT.FindWordLines();
    } catch(const char* e) {
	cout << e << endl;
    }
    return 0;
}
