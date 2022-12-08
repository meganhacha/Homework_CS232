#include "crawler.h"
#include "indexPage.h"

int contains(const struct listNode *pNode, const char *addr){

  if(pNode != NULL) {

    if(strcmp(pNode->addr, addr) == 0) {
      return 1;
    }
    else {
      return contains(pNode->next, addr);
    }
  }
  return 0;
}

void insertBack(struct listNode *pNode, const char *addr, int* totalTerms){

  if(pNode->next == NULL) {
    struct listNode* tempNode = (struct listNode*)malloc(sizeof(struct listNode));

    strncpy(tempNode->addr, addr, strlen(addr));
    tempNode->next = NULL;
    //Insert the Trie of the indexed page as the root of each newly created linked list node.
    tempNode->root = indexPage(addr, totalTerms);
    pNode->next = tempNode;
  }
  else {
    insertBack(pNode->next, addr, totalTerms);
  }

}

void printAddresses(const struct listNode *pNode){

  if(pNode != NULL) {
    printf("%s\n", pNode->addr);
    printAddresses(pNode->next);
  }
}

void destroyList(struct listNode *pNode){
  if(pNode->next == NULL) {
    freeTrieMemory(pNode->root);
    free(pNode);
    return;
  }

  else {
    destroyList(pNode->next);
    freeTrieMemory(pNode->root);
    free(pNode);
    return;
  }
}
  


int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		    break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    
    /* get rid of the newline */
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		    *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}
