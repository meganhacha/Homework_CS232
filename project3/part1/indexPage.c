#ifndef INDEXPAGE_H_
#define INDEXPAGE_H_

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_CHARS  300000
#define  ALPHABET 26

struct trieNode {
    char letter;
    int count;
    int isEnd;
    int *pTotalNums;
    struct trieNode *children[ALPHABET];
};

struct getNewTrieNode;

struct trieNode* urlWords(char* url, int* totalTerms);

struct trieNode* indexPage(const char* url, int* totalWords);

int addWordOccurrence(const char* word, const int wordLength, struct trieNode *root);

void printTrieContents(struct trieNode *root, char displayArr[], int index);

int freeTrieMemory(struct trieNode *tNode);

int getText(const char* srcAddr, char* buffer, const int bufSize);

#endif