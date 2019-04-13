#ifndef WORDWITHFREQ_H
#define WORDWITHFREQ_h
#include <string>
#include <vector>
struct WordWithFreq {
    // Struct to count a word and its frequency
    std::string word;
    int frequency = 0;
	std::vector<int> linesWhereAppearing;
};

#endif