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

“words for problem.txt”, containing a word list, with 173k rows, for testing purposes.
