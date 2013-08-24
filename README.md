Find-Longest-Word-Made-of-Other-Words
=====================================

Programming Problem - Find Longest Word Made of Other Words

Write a program that reads a file containing a sorted list of words (one word per line, no spaces, all lower case), then identifies the longest word in the file that can be constructed by concatenating copies of shorter words also found in the file. The program should then go on to report the second longest word found as well as how many of the words in the list can be constructed of other words in the list.

For example, if the file contained:

       cat
       cats
       catsdogcats
       catxdogcatsrat
       dog
       dogcatsdog
       hippopotamuses
       rat
       ratcatdogcat

The longest word would be 'ratcatdogcat' - at 12 letters, it is the longest word made up of other words in the list.  

Performance: the program should return results quickly even for very large lists (100,000+ items).

“wordsforproblem.txt”, containing a word list, with 173k rows, for testing purposes.
The results are: 
The (1) longest one: ethylenediaminetetraacetates
The (2) longest one: electroencephalographically
The count of words that can be constructed: 97107

To run it in a Terminal, just follow the steps (verified on Mac OS X 10.8):
1) save the "main_rock.cpp" in a directory, and copy the "wordsforproblem.txt" into this directory.
2) open a Terminal, change directory to there.
3) "g++ -ggdb -Wall -I. -o findLongest main_rock.cpp".
4) "./findLongest"
5) you may find the printed result in console, and a file "foundWords.txt" is created.

  About the source code, 
1) read the "wordsforproblem.txt" line by line, in the same time, build a Trie tree.
2) check all words "isConcatWord()", sorted by length, began with the longest length.
  The key problem is how to speed up "isConcatWord()", where I used recursive function and used "isWordBreak()" to search sub-word break position for performance.
3) TODO: something can be done to optimize more, for example, a) multi-threading, b) now assume that memory is enough for the input words. Maybe it is an issue, too.
