#include <iostream>
#include <stdio.h>
#include "wordCounter.h"
#include "InvertedText.h"
using namespace std;

int main()
{

    try {
    
    //Performing operations for File1ForLab3.txt
	wordCounter myCounter;
	myCounter.getWordsFromFile("File1ForLab3.txt");
	myCounter.printWordList();
	
	int K = 10;
	myCounter.printTopKWords(K);
	K = 20;
	myCounter.printTopKWords(K);
	
	InvertedText invT(myCounter.wordList(), "File1ForLab3.txt");
	invT.FindWordLines();
	
	//Performing operations for File2ForLab3.txt
	myCounter.getWordsFromFile("File2ForLab3.txt");
	myCounter.printWordList();
	
	K = 10;
	myCounter.printTopKWords(K);
	K = 20;
	myCounter.printTopKWords(K);
	
	InvertedText invText(myCounter.wordList(), "File2ForLab3.txt");
	invText.FindWordLines();
    } catch(const char* e) {
	cout << e << endl;
    }
    return 0;
}
