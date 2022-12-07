#include "webSearch.h"


int main(int argc, char** argv) {

  long seed;
  int n = 0;
  struct listNode* pListStart;
  char startAddr[MAX_ADDR_LENGTH];
  char displayArr[MAX_CHARS];
  char destArr[MAX_ADDR_LENGTH];
  char url[MAX_ADDR_LENGTH];
  FILE* fp;
  
  int hopNum, maxHops,MAX_PAGES;

  if(argc != 4) {
    printf("Not enough arguments were passed. Must at least include: 1. a file name, 2. a max amount of pages that will be indexed, and 3. A random int to create the seed.\n");
    return -1;
  }

  fp = fopen(argv[1], "r");
  MAX_PAGES = atoi(argv[2]);

  if(argc >= 3) {
    seed = atol(argv[3]);
    srand(seed);
    seed = rand();
  }

  pListStart = malloc(sizeof(struct listNode));
  if(pListStart == NULL) {
    fprintf(stderr, "ERROR: unable to allocate memory.\n");
    return -2;
  }
  else {
    pListStart->next = NULL;
    pListStart->root = NULL;
  }

  printf("Indexing...\n");
  while((fscanf(fp, "%s %d", url, &maxHops) != EOF) && (n < MAX_PAGES)) {

    strncpy(startAddr, url, MAX_ADDR_LENGTH);
    hopNum = 0;

    int continueLoop = 1;
    while(continueLoop) {

      if(contains(pListStart->next, startAddr) == 0) {
        printf("%s\n", startAddr);

        int totalTerms = 0;
        pListStart->root = urlWords(url, &totalTerms);

        insertBack(pListStart, startAddr);
        printTrieContents(pListStart->root, displayArr, 0);

        n++;
      }

      hopNum++;

      if((hopNum <=maxHops) && (n<MAX_PAGES)) {
        int found = getLink(startAddr, destArr, MAX_ADDR_LENGTH);

        if(found == 0) {
          break;
        }

        strncpy(startAddr, destArr, MAX_ADDR_LENGTH);
      }
      else {
        break;
      }
    }
  }
  fclose(fp);
  destroyList(pListStart);

}