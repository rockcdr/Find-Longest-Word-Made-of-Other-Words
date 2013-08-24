//
//  main.cpp
//  findLongest
//
//  Created by Rock on 8/9/13.
//  Copyright (c) 2013 Rock. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <set>
#include <iterator>

#define NUM_CHARS	  26       //'a' ~ 'z'
using namespace std;
typedef list<string> StringList;

// tree node of Trie
typedef struct _trie {
    bool isWord;
    struct _trie *edges[NUM_CHARS];
}trie;

// create an empty Trie
trie* initialize(trie *node)
{
    if (node == NULL)
        node = (trie *)malloc(sizeof(trie));
    
    node->isWord = false;
    for (int i=0; i < NUM_CHARS; i++)
        node->edges[i] = NULL;
    return node;
}

// destroy the Trie
void uninitialize(trie* &node)
{
    if (node == NULL)
        return;
    for (int i=0; i < NUM_CHARS; i++) {
        uninitialize(node->edges[i]);
    }
    free(node);
    node = NULL;
}

// add a word into Trie
trie* addWord(trie *node, const char *str)
{
    if(str[0] == '\0') {
        node->isWord = true;
    } else {
        int ch = str[0] - 'a';
        trie* &edge = node->edges[ch];
        if ( edge == NULL)
            edge = initialize(edge);
        str++;
        edge = addWord(edge, str);
    }
    return node;
}

#ifdef TEST
bool findWord(trie *node, const char *str)
{
    if(str[0] == '\0')
        return node->isWord;
    else {
        int ch = str[0] - 'a';
        if (node->edges[ch] == NULL)
            return false;
        str++;
        return findWord(node->edges[ch], str);
    }
}

bool isWord(trie *node, const char *str, int start, int end)
{
    if(str[start] == '\0' || start > end)
        return node->isWord;
    else {
        int ch = str[start] - 'a';
        if (node->edges[ch] == NULL)
            return false;
        return isWord(node->edges[ch], str, start+1, end);
    }
}
#endif

//search a word-break until found(first position where >= mid) or unmatched or string-terminate
bool isWordBreak(trie *node, const char *str, int start, int end, int &mid)
{
    trie *subnode = node;
    const char *pch = str + start;
    int i;
    for (i=start; i<=end; i++) { 
        int ch = *pch - 'a';
        if (subnode->edges[ch] == NULL) {
            return false; //unmatched
        }
        
        //move forword
        pch++;
        subnode = subnode->edges[ch];
        if (i>=mid && subnode->isWord) {
            mid = i;
            return true; //found
        }
    }
    mid = end;
    return subnode->isWord;
}

// decide a word whether are made of other words
// return count of sub-words
int isConcatWord(trie *node, const char *str, int start, int end, bool &result)
{
    result = false;
    if (start > end)
        return 0;//wrong input
    
    for (int i=start; i<=end; i++) {
#ifdef TEST
        bool bPart1 = isWord(node, str, start, i);
        if (!bPart1)
            continue;
#endif
        bool bPart1 = isWordBreak(node, str, start, end, i);
        
        if (i == end) {
            result = bPart1;
            return (bPart1 ? 1 : 0);
        }
        
        // start the second part match
        bool bPart2 = false;
        int cntWords = isConcatWord(node, str, i+1, end, bPart2);
        if (bPart1 && bPart2) {
            result = true;
            return 1 + cntWords;
        }
    }
    return 0;
}

// read input words int Trie.
int ReadWordFile(const char *filename,      //input file name
                 trie* &root,               //output: root of Trie
                 map<size_t, StringList> &Length2Dict,  //output for search, group the same length
                 set<size_t> &LengthSet)        //output: sorted keys for map 'Length2Dict'
{
    root = NULL;
    root = initialize(root);
    int cntWords = 0;
    ifstream ifs(filename, ifstream::in); //should check more on this file context.
    istream_iterator<string> itr_word(ifs), itr_word_end;
    for ( ; itr_word != itr_word_end; itr_word++, cntWords++ ) {
        addWord(root, (*itr_word).c_str());  //assume input lower case words, no spaces, on word per line
        size_t len = itr_word->size();
        Length2Dict[len].push_back(*itr_word);
        LengthSet.insert(len);
    }
    ifs.close();
    return cntWords;
}

int main(int argc, const char * argv[])
{
    if (argc <= 1) {
        cout << "usage: findLongest [default name: wordsforproblem.txt]\n";
    }
    const char *filename = "wordsforproblem.txt";
    if (argc > 1)filename = argv[1];
    
    trie *root = NULL;  //root of Trie of all words
    map<size_t, StringList> Length2Dict;    //all input words, group the same length
    set<size_t> LengthSet;     //all length values of words, sorted
    int cntWords = ReadWordFile(filename, root, Length2Dict, LengthSet);
    cout << "Input words: " << cntWords << endl;
    
    int foundWords = 0;
    int cntConcat = 0;
    set<size_t>::reverse_iterator rit;
    const char* foundWordsFileName = "foundWords.txt";
    ofstream foundWordsFile(foundWordsFileName);
    for (rit=LengthSet.rbegin(); rit!=LengthSet.rend(); rit++) { //begin with the longest length
        size_t len = *rit;
        StringList& dict = Length2Dict[len];
        for (StringList::const_iterator it=dict.begin(); it!=dict.end(); it++) { //loop all of the same length
            bool found = false;
            cntConcat = isConcatWord(root, it->c_str(), 0, (int)it->size()-1, found);
            if (found && cntConcat>1) { // found one, output it
                if(foundWords==0)cout << "The (1) longest one: " << *it << endl;
                else if(foundWords==1)cout << "The (2) longest one: " << *it << endl;
                foundWords++;
                foundWordsFile << *it << endl;
            }
        }
    }
    cout << "Found words (in foundWords.txt): " << foundWords << endl;
    
    uninitialize(root);
    return 0;
}

