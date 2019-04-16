/* Copyright (c) 2007-2011, Stanford University
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Stanford University nor the names of its 
*       contributors may be used to endorse or promote products derived from 
*       this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/ 

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sys/time.h>
#include <iostream>
#include <ctime>

#ifdef TBB
#include "tbb/scalable_allocator.h"
#endif

#include "map_reduce.h"
#define DEFAULT_DISP_NUM 10

// a passage from the text. The input data to the Map-Reduce
struct wc_string {
    char* data;
    uint64_t len;
};

// a single null-terminated word
struct wc_word {
    char* data;
    
    // necessary functions to use this as a key
    bool operator<(wc_word const& other) const {
        return strcmp(data, other.data) < 0;
    }
    bool operator==(wc_word const& other) const {
        return strcmp(data, other.data) == 0;
    }
};
 
// a hash for the word
struct wc_word_hash
{
    // FNV-1a hash for 64 bits
    size_t operator()(wc_word const& key) const
    {
        char* h = key.data;
        uint64_t v = 14695981039346656037ULL;
        while (*h != 0)
            v = (v ^ (size_t)(*(h++))) * 1099511628211ULL;
        return v;
    }
};

std::vector<std::string> StopWords;

void loadStopWords()
{ // Loads the stop words from file
    std::string line;
    std::ifstream inputFile("stopWords.txt");
    if(inputFile.is_open()) {
	    while(getline(inputFile, line)) {
		StopWords.push_back(line);
	    }
	} else {
	    throw "File not found!";
	}
}

std::vector<wc_word> stopWordVector;

void stopWords()
{
    std::string temp = " ";
   
    for(uint64_t i = 0; i < StopWords.size(); i++) 
    {
	wc_word word;
	word.data = &StopWords[i][0u];

	
	for (uint64_t j = 0; j < StopWords[i].length(); j++)
        {
           word.data[j] = toupper(word.data[j]); 
        }
        
	stopWordVector.push_back(word);
    }

}

bool isStopWord(wc_word word)
{ // Checks whether the word is a stop word
    bool isStopWord = 0;
	for(uint64_t i = 0; i < stopWordVector.size(); i++) {
	    if(word == stopWordVector.at(i)) {
	    		isStopWord = 1;
	    break;}
    }
    
    return isStopWord;
}

#ifdef MUST_USE_FIXED_HASH
class WordsMR : public MapReduceSort<WordsMR, wc_string, wc_word, uint64_t, fixed_hash_container<wc_word, uint64_t, sum_combiner, 32768, wc_word_hash
#else
class WordsMR : public MapReduceSort<WordsMR, wc_string, wc_word, uint64_t, hash_container<wc_word, uint64_t, sum_combiner, wc_word_hash 
#endif
#ifdef TBB
    , tbb::scalable_allocator
#endif
> >
{
    char* data;
    uint64_t data_size;
    uint64_t chunk_size;
    
    uint64_t splitter_pos;
public:
    explicit WordsMR(char* _data, uint64_t length, uint64_t _chunk_size) :
        data(_data), data_size(length), chunk_size(_chunk_size), 
            splitter_pos(0) {}

    void* locate(data_type* str, uint64_t len) const
    {
        return str->data;
    }

    void map(data_type const& s, map_container& out) const
    {
        for (uint64_t i = 0; i < s.len; i++)
        {
            s.data[i] = toupper(s.data[i]);
        }

        uint64_t i = 0;
        while(i < s.len)
        {            
            while(i < s.len && (s.data[i] < 'A' || s.data[i] > 'Z'))
                i++;
            uint64_t start = i;
            while(i < s.len && ((s.data[i] >= 'A' && s.data[i] <= 'Z') || s.data[i] == '\''))
                i++;
            if(i > start)
            {
                s.data[i] = 0;
                
                wc_word word = { s.data+start };
                
                bool isAStopWord = 0;
                isAStopWord = isStopWord(word);

                
                if (isAStopWord)
                {
                
                }
                
                else
                {
		    emit_intermediate(out, word, 1);
		}   
            }
        }
    }

    /** wordcount split()
     *  Memory map the file and divide file on a word border i.e. a space.
     */
    int split(wc_string& out)
    {
        /* End of data reached, return FALSE. */
        if ((uint64_t)splitter_pos >= data_size)
        {
            return 0;
        }

        /* Determine the nominal end point. */
        uint64_t end = std::min(splitter_pos + chunk_size, data_size);

        /* Move end point to next word break */
        while(end < data_size && 
            data[end] != ' ' && data[end] != '\t' &&
            data[end] != '\r' && data[end] != '\n')
            end++;

        /* Set the start of the next data. */
        out.data = data + splitter_pos;
        out.len = end - splitter_pos;
        
        splitter_pos = end;

        /* Return true since the out data is valid. */
        return 1;
    }

    bool sort(keyval const& a, keyval const& b) const
    {
        return a.val < b.val || (a.val == b.val && strcmp(a.key.data, b.key.data) > 0);
    }
};

#define NO_MMAP

int main(int argc, char *argv[]) 
{
    int fd;
    char * fdata;
    unsigned int disp_num;
    struct stat finfo;
    char * fname, * disp_num_str, *stopwords;
    struct timeval start, end;
    
    loadStopWords();
    stopWords();

    //get_time (begin);

    // Make sure a filename is specified
    if (argv[1] == NULL)
    {
        printf("USAGE: %s <filename> [Top # of results to display]\n", argv[0]);
        exit(1);
    }

    fname = argv[1];
    disp_num_str = argv[2];
    
    

    printf("Wordcount: Running...\n");

    // Read in the file
    CHECK_ERROR((fd = open(fname, O_RDONLY)) < 0);
    // Get the file info (for file length)
    CHECK_ERROR(fstat(fd, &finfo) < 0);
#ifndef NO_MMAP
#ifdef MMAP_POPULATE
    // Memory map the file
    CHECK_ERROR((fdata = (char*)mmap(0, finfo.st_size + 1, 
        PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0)) == NULL);
#else
    // Memory map the file
    CHECK_ERROR((fdata = (char*)mmap(0, finfo.st_size + 1, 
        PROT_READ, MAP_PRIVATE, fd, 0)) == NULL);
#endif
#else
    uint64_t r = 0;

    fdata = (char *)malloc (finfo.st_size);
    CHECK_ERROR (fdata == NULL);
    while(r < (uint64_t)finfo.st_size)
        r += pread (fd, fdata + r, finfo.st_size, r);
    CHECK_ERROR (r != (uint64_t)finfo.st_size);
#endif    
    
    // Get the number of results to display
    CHECK_ERROR((disp_num = (disp_num_str == NULL) ? 
      DEFAULT_DISP_NUM : atoi(disp_num_str)) <= 0);

    //get_time (end);

#ifdef TIMING
    print_time("initialize", begin, end);
#endif

    printf("Wordcount: Calling MapReduce Scheduler Wordcount\n");
    
    clock_t begin, stop;
    begin = clock();
    std::vector<WordsMR::keyval> result;    
    WordsMR mapReduce(fdata, finfo.st_size, 1024*1024);
    CHECK_ERROR( mapReduce.run(result) < 0);
    stop = clock();
    
    double time_taken = double(stop - begin)/double(CLOCKS_PER_SEC);
    std::cout << "Time taken to count words:" << std::fixed << time_taken << std::setprecision(10);
    std::cout << " sec"<< std::endl;

    printf("Wordcount: MapReduce Completed\n");


    unsigned int dn = 10;
    printf("\nWordcount: Results (TOP %d of %lu):\n", dn, result.size());
    
    begin = clock();
    for (size_t i = 0; i < dn; i++)
    {
        printf("%15s - %lu\n", result[result.size()-1-i].key.data, result[result.size()-1-i].val);
    }
    stop = clock();
    
    time_taken = double(stop - begin)/double(CLOCKS_PER_SEC);
    std::cout << "Time taken to determine top 10 most occuring words:" << std::fixed << time_taken << std::setprecision(10);
    std::cout << " sec"<< std::endl;
    
    dn = 20;
    printf("\nWordcount: Results (TOP %d of %lu):\n", dn, result.size());
    uint64_t total = 0;
    
    begin = clock();
    for (size_t i = 0; i < dn; i++)
    {
        printf("%15s - %lu\n", result[result.size()-1-i].key.data, result[result.size()-1-i].val);
    }
    stop = clock();
    
    time_taken = double(stop - begin)/double(CLOCKS_PER_SEC);
    std::cout << "Time taken to determine top 20 most occuring words:" << std::fixed << time_taken << std::setprecision(10);
    std::cout << " sec"<< std::endl;

    for(size_t i = 0; i < result.size(); i++)
    {
        total += result[i].val;
    }
    
    
    
    printf("Total: %lu\n", total);

#ifndef NO_MMAP
    CHECK_ERROR(munmap(fdata, finfo.st_size + 1) < 0);
#else
    free (fdata);
#endif
    //CHECK_ERROR(close(fd) < 0);

    //get_time (end);

#ifdef TIMING
    //print_time("finalize", begin, end);
#endif

    return 0;
}

// vim: ts=8 sw=4 sts=4 smarttab smartindent
