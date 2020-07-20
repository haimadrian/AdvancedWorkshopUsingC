//
// Created by Haim Adrian
//

#ifndef UTILS_CUSTOMLINKEDLIST_H
#define UTILS_CUSTOMLINKEDLIST_H

#include "three.h"

#ifndef CUSTOM_LIST_NODE_TYPE
#define CUSTOM_LIST_NODE_TYPE
typedef three * listNodeType_custom;
#endif

typedef struct listNode_custom {
	struct listNode_custom *next;
	listNodeType_custom value;
} listNode_custom;

typedef struct linkedList_custom {
	listNode_custom *head;
	listNode_custom *tail;
	unsigned int size;
} linkedList_custom;

int initLinkedList_custom(linkedList_custom *list);
void freeLinkedList_custom(linkedList_custom *list);

int addFirst_custom(linkedList_custom *list, listNodeType_custom item);
int addLast_custom(linkedList_custom *list, listNodeType_custom item);
int insert_custom(linkedList_custom *list, listNode_custom *after, listNodeType_custom item);
int insertBefore_custom(linkedList_custom *list, listNode_custom *before, listNodeType_custom item);

int removeFirst_custom(linkedList_custom *list);
int removeLast_custom(linkedList_custom *list);
int removeFromList_custom(linkedList_custom *list, listNode_custom *node);

listNodeType_custom value_custom(listNode_custom *node);
int head_custom(linkedList_custom *list, listNode_custom **item);
int tail_custom(linkedList_custom *list, listNode_custom **item);

int isLinkedListEmpty_custom(linkedList_custom *list);

unsigned int linkedListSize_custom(linkedList_custom *list);

listNode_custom *linkedListIterator_custom(linkedList_custom *list);
int hasNext_custom(listNode_custom *list);
listNode_custom *next_custom(listNode_custom *list);
listNode_custom *find_custom(linkedList_custom *list, listNodeType_custom item);

void printLinkedList_custom(linkedList_custom *list, void (*printItemFunc)(listNodeType_custom));

#endif //UTILS_CUSTOMLINKEDLIST_H
