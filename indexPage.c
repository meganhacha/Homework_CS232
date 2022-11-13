/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_CHARS  300000
#define  ALPHABET 26

struct trieNode;
struct getNewTrieNode;



struct trieNode* indexPage(const char* url);

int addWordOccurrence(const char* word, const int wordLength, struct trieNode *root);

void printTrieContents(struct trieNode *root, char* displayArr, int index);

int freeTrieMemory(/* TODO: any parameters you need */);

int getText(const char* srcAddr, char* buffer, const int bufSize);


struct trieNode{
  int count;
  int isEnd;
  struct trieNode *children[ALPHABET];

};

struct trieNode* getNewTrieNode() {

  struct trieNode *new = (struct trieNode*)malloc(sizeof(struct trieNode));
  new->count = 0;
  new->isEnd = 0;

  for(int i = 0; i < ALPHABET; i++) {
    new->children[i] = NULL;
  }

  return new;
}

int main(int argc, char** argv){

  struct trieNode *root;
  char display[100]; 

  if(argc < 1) {
    printf("Ensure you are including a valid url as an argument.");
  }
  else {
    root = indexPage("hello");

    printTrieContents(root, display, 0);
  }
    free(root);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */


struct trieNode* indexPage(const char* url)
{
  struct trieNode *root = getNewTrieNode();

  char *fromURL = malloc(MAX_CHARS);
  char *singleWord = malloc(MAX_CHARS);
  int wordSize = 0;

  int const manyChar = getText(url,fromURL, MAX_CHARS);



  if(manyChar != 0) {
    for(int i = 0; i < manyChar; i++) {
      if(fromURL[i] == '\\' && fromURL[i+1] == 'n') {
        fromURL[i] = ' ';
        fromURL[i + 1] = ' ';
      }
      if(fromURL[i] == 'b' && fromURL[i+1] == '\'') {
        fromURL[i] = ' ';
        fromURL[i+1] = ' ';
      }
      if(fromURL[i] == '\'' && fromURL[i+1]=='\n' && fromURL[i+2] == 'b'){
        fromURL[i] = ' ';
        fromURL[i+1] = ' ';
        fromURL[i+2] = ' ';
      }
      if(isdigit(fromURL[i])) {
        fromURL[i] = ' ';
      }
      if(ispunct(fromURL[i])) {
        fromURL[i] = ' ';
      }

      fromURL[i] = tolower(fromURL[i]);
    }

    for(int j = 0; j < manyChar; j++) {
      if(!(isalpha(fromURL[j])) && isalpha(singleWord[0])) {
        addWordOccurrence(singleWord, strlen(singleWord), root);
        // free(singleWord);
      }
      else {
        singleWord[j] = fromURL[j];
        wordSize++;
      }
    }
    // free(fromURL);
    return root;
  }
  // free(fromURL);
  return NULL;
}

int addWordOccurrence(const char* word, const int wordLength, struct trieNode *root)
{
  struct trieNode *current = root;
  int index;

  for(int i = 0; i <wordLength; i++) {

    index = ((int)word[i] - (int)'a');

    if(!current->children[index]) {
      current->children[index] = getNewTrieNode();
    }
    
    else if(current->children[index] != NULL) {
      (current->children[index]->count)++;
    }
    current = current->children[index];
  }
  current->isEnd = 1;
  return 1;
}

void printTrieContents(struct trieNode *root, char* displayArr, int index)
{

    if(root == NULL) {
    printf("Oops, no stuff!");
  }

  if(root->isEnd) {
    displayArr[index] = '\0';
    printf("\t%s\n", displayArr);
  }

  for(int i = 0; i < ALPHABET; i++) {
    if(root->children[i] != NULL) {

      displayArr[index] = i +'a';
      printTrieContents(root->children[i], displayArr, index + 1);
    }
  }
}

int freeTrieMemory(/* TODO: any parameters you need */)
{
  return 0;
}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
