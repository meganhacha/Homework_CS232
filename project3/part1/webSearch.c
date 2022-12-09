#include "webSearch.h"


int main(int argc, char** argv) {

  long seed;
  int n = 0;
  struct listNode* startList;
  char startAddr[MAX_ADDR_LENGTH];
  // char displayArr[MAX_CHARS];
  char destArr[MAX_ADDR_LENGTH];
  char url[MAX_ADDR_LENGTH];
  FILE* fp;
  
  int hopNum, maxHops,MAX_PAGES;

  if(argc < 4) {
    printf("Not enough arguments were passed. Must at least include:\n1. A file name\n2. A max amount of pages that will be indexed \n3. A random number to create the seed.\n");
    return -1;
  }

  fp = fopen(argv[1], "r");
  MAX_PAGES = atoi(argv[2]);

  seed = atol(argv[3]);
  srand(seed);

  startList = malloc(sizeof(struct listNode));
  if(startList == NULL) {
    fprintf(stderr, "ERROR: unable to allocate memory.\n");
    return -2;
  }
  else {
    startList->next = NULL;
    startList->root = NULL;
  }

  printf("Indexing...\n");
  while((fscanf(fp, "%s %d", url, &maxHops) != EOF) && (n <= MAX_PAGES)) {

    strncpy(startAddr, url, MAX_ADDR_LENGTH);
    hopNum = 0;

    int continueLoop = 1;
    while(continueLoop) {

      if(contains(startList->next, startAddr) == 0) {
        printf("%s\n", startAddr);

        int totalTerms = 0;

        //Add a new node to the linkedList, with insertBack handling both the new LL node and Trie for the corresponding page.
        insertBack(startList, startAddr, &totalTerms);

        n++;
      }

      hopNum++;

      if((hopNum <=maxHops) && (n<=MAX_PAGES)) {
        int found = getLink(startAddr, destArr, MAX_ADDR_LENGTH);

        if(found == 0) {
        //found == 0 means that the link is invalid, so the loop has to be exited.
          break;
        }

        //Copy the new (valid) destination to the array used to index, then do the indexing for it.
        strncpy(startAddr, destArr, MAX_ADDR_LENGTH);
      }
      else {
        break;
      }
    }
  }
  printf("Indexing complete.\n");
  fclose(fp);
  destroyList(startList);

}