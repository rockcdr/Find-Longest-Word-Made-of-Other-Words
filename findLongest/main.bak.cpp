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

#define NUM_CHARS	  26
using namespace std;
typedef list<string> StringList;

typedef struct _trie {
    bool isWord;
    struct _trie *edges[NUM_CHARS];
}trie;

trie* initialize(trie *node)
{
    if (node == NULL)
        node = (trie *)malloc(sizeof(trie));
    
    node->isWord = false;
    for (int i=0; i < NUM_CHARS; i++)
        node->edges[i] = NULL;
    return node;
}

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

#define WORD_MATCH_NEED_MORE    0   //all matched, but need more, false
#define WORD_MATCHED            1   //true matched
#define WORD_MATCH_BREAK        2   //break for false
int isWordMore0(trie *node, const char *str, int start, int end)
{
    if(str[start] == '\0' || start > end)
        return (node->isWord ? WORD_MATCHED : WORD_MATCH_NEED_MORE);
    else {
        int ch = str[start] - 'a';
        if (node->edges[ch] == NULL)
            return WORD_MATCH_BREAK;
        return isWordMore0(node->edges[ch], str, start+1, end);
    }
}
int isWordMore1(trie *node, const char *str, int start, int end)
{
    trie *subnode = node;
    const char *pch = str + start;
    int i;
    for (i=start; i<=end && subnode!=NULL; i++) {
        int ch = *pch - 'a';
        if (subnode->edges[ch] == NULL) {
            return WORD_MATCH_BREAK;
        }
        
        //move forword
        pch++;
        subnode = subnode->edges[ch];
    }
    
    if (subnode == NULL)
        return (i>end ? WORD_MATCHED : WORD_MATCH_BREAK);
    else
        return (subnode->isWord ? WORD_MATCHED : WORD_MATCH_NEED_MORE);
}
//search a word-break until found(first position with >= mid) or unmatched or string-terminate
bool isWordMore(trie *node, const char *str, int start, int end, int &mid)
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

int isConcatWord(trie *node, const char *str, int start, int end, bool &result)
{
    result = false;
    if (start > end)
        return 0;//wrong input
    
    for (int i=start; i<=end; i++) {
        //bool bPart1 = isWord(node, str, start, i);
        //if (!bPart1)
        //    continue;
        bool bPart1 = isWordMore(node, str, start, end, i);
        //int iPart1 = isWordMore(node, str, start, i);
        //if (iPart1 == WORD_MATCH_NEED_MORE)
        //    continue;
        //if (iPart1 == WORD_MATCH_BREAK)
        //    return false;
        //bool bPart1 = iPart1 == WORD_MATCHED;
        
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
                 trie* &root,       //root of Trie
                 map<size_t, StringList> &Length2Dict,  //output for search
                 set<size_t> &LengthSet)        //sorted keys for map: Length2Dict
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
    const char *filename = "wordsforproblem.txt";//"test.txt";//
    if (argc > 1)filename = argv[1];
    
    trie *root = NULL;
    map<size_t, StringList> Length2Dict;
    set<size_t> LengthSet;
    int cntWords = ReadWordFile(filename, root, Length2Dict, LengthSet);
    cout << "Input words:" << cntWords << endl;
    
    int foundWords = 0;
    int cntConcat = 0;
    set<size_t>::reverse_iterator rit;
    const char* foundWordsFileName = "foundWords.txt";
    ofstream foundWordsFile(foundWordsFileName);
    for (rit=LengthSet.rbegin(); rit!=LengthSet.rend(); rit++) {
        size_t len = *rit;
        StringList& dict = Length2Dict[len];
        for (StringList::const_iterator it=dict.begin(); it!=dict.end(); it++) {
            bool found = false;
            cntConcat = isConcatWord(root, it->c_str(), 0, (int)it->size()-1, found);
            if (found && cntConcat>1) {
                if(foundWords<2)cout << *it << endl;
                foundWords++;
                foundWordsFile << *it << endl;
            }
        }
    }
    cout << "Found words (in foundWords.txt):" << foundWords << endl;
    
    uninitialize(root);
    return 0;
}

