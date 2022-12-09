
#include "indexPage.h"

struct trieNode* getNewTrieNode() {
  struct trieNode *new = (struct trieNode*)malloc(sizeof(struct trieNode));
  new->count = 0;
  new->isEnd = 0;
  new->letter = ' ';
  new->pTotalNums = NULL;

  for(int i = 0; i < ALPHABET; i++) {
    new->children[i] = NULL;
  }

  return new;
}


struct trieNode* indexPage(const char* url, int* totalWords)
{
  struct trieNode *root = getNewTrieNode();
 
  char singleWord[MAX_CHARS];
  char fromURL[MAX_CHARS];

  int manyChar = getText(url, fromURL, MAX_CHARS);


  if(manyChar != 0) {
    int stringLoc = 0;


    for(int i = 0; i <strlen(fromURL); i++) {

      //If the character is an alphabetical character, it converts it to lowercase, checks for a 'b' character, then turns it to a string.
      //If it is b, it's converted to the single b as a string, then pushed to addWordOccurrence. 
      //If a normal char, then it's simply added to the singleWord array.
     if(isalpha(fromURL[i]) != 0) {
        singleWord[stringLoc] = tolower(fromURL[i]);
        stringLoc++;
        if(singleWord[0] == 'b') {
          singleWord[1] = '\0';
          addWordOccurrence(singleWord, strlen(singleWord), root);
          printf("\t%s\n", singleWord);

          stringLoc = 0;
        }
      }

      //A whitespace is used to indicate the end of a single word, so the singleWord array is converted to a string and passed to addWordOccurrence.
     else if ((isspace(fromURL[i]) != 0) || (isdigit(fromURL[i]) != 0) || (ispunct(fromURL[i]) != 0)) {
      if(stringLoc != 0) {
          singleWord[stringLoc] = '\0';

          addWordOccurrence(singleWord, strlen(singleWord), root);
          printf("\t%s\n", singleWord);
          totalWords++;
          stringLoc = 0;
      }
      else {
        continue;
      }
      }
    }
  } 
  return root;
} 

int addWordOccurrence(const char* word, const int wordLength, struct trieNode *root)
{
  struct trieNode *current = root;
  int index =  0;

  for(int i = 0; i < wordLength; i++) {

    //index takes the int value of a lowercase letter and subtracts a's int value so that it goes to the corresponding index.
    index = ((int)word[i] - (int)'a');

    if(!current->children[index]) {
      current->children[index] = getNewTrieNode();
      current->children[index]->letter = word[i];
      current->children[index]->count++;

    }

    //Checks if the current letter exists, and if the string begin submitted is at its last char.
    else if((current->children[index] != NULL) && (i == (wordLength - 1))) {

      /*
      If the next char in the trie is a leaf, then it increments the spot at the current index.
      This was mostly found through trial and error, but also has to do with adding plural and singular versions of words.
      For example: we had "computers" submitted first, then "computer". since r is the last char of the submitted word, but
      computerS exists, it increments the value of r's counter, then changes the value of r's isEnd to true. 
      */
      if(current->children[index]->isEnd != 0) {
        current->children[index]->count++;
      }
    }

    current = current->children[index];
  }
  current->isEnd = 1;
  return 1;
}


void printTrieContents(struct trieNode *root, char* displayArr, int index)
{

struct trieNode* current = root;

  if(current == NULL) {
    return;
  }

  if(current->isEnd) {

    //Mark the end of the displayArr string when a leaf is reached.
    displayArr[index] = '\0';
  }

  for(int i = 0; i < ALPHABET; i++) {
    if(current->children[i] != NULL) {

      //The current letter is added to the displayArr array to create a string.
      displayArr[index] = current->children[i]->letter;
      printTrieContents(current->children[i], displayArr, index +1);
    }
  }

  }



int freeTrieMemory(struct trieNode *tNode)
{

  if(tNode == 0) {
    return 0;
  }

  for(int i = 0; i < ALPHABET; i++) {
    freeTrieMemory(tNode->children[i]);
  }
  free(tNode);
  return 1;

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
