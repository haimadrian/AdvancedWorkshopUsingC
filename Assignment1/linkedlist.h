//
// Created by Haim Adrian
//

#ifndef UTILS_LINKEDLIST_H
#define UTILS_LINKEDLIST_H

typedef unsigned int listNodeType;

typedef struct listNode {
	struct listNode *next;
	listNodeType value;
} listNode;

listNode * createLinkedList(listNodeType item);
void freeLinkedList(listNode *list);

int addFirst(listNode **list, listNodeType item);
int addLast(listNode **list, listNodeType item);
int insert(listNode **after, listNodeType item);
int insertBefore(listNode **list, listNode *before, listNodeType item);

int removeFirst(listNode **list);
int removeLast(listNode **list);
int removeFromList(listNode **list, listNode *node);

listNodeType value(listNode *node);
int head(listNode *list, listNode **node);
int tail(listNode *list, listNode **node);

int isLinkedListEmpty(listNode *list);

int hasNext(listNode *node);
listNode *next(listNode *node);
listNode *find(listNode *list, listNodeType item);

void printLinkedList(listNode *list);

#endif //UTILS_LINKEDLIST_H
