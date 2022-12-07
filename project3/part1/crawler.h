#ifndef CRAWLER_H_
#define CRAWLER_H_

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ADDR_LENGTH 1000

struct listNode{
  char addr[MAX_ADDR_LENGTH];
  struct trieNode* root;

  struct listNode *next;
};


int contains(const struct listNode *pNode, const char *addr);
void insertBack(struct listNode *pNode, const char *addr);
void printAddresses(const struct listNode *pNode);
void destroyList(struct listNode *pNode);
int getLink(const char* srcAddr, char* link, const int maxLinkLength);

#endif 