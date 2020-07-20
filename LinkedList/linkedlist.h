//
// Created by Haim Adrian
//

#ifndef UTILS_LINKEDLIST_H
#define UTILS_LINKEDLIST_H

typedef char listNodeType;
//typedef int listNodeType;

typedef struct Node {
	struct Node *next;
	listNodeType value;
} listNode;

typedef struct {
	listNode *first;
	listNode *last;
	unsigned int size;
} linkedList;

int createLinkedList(linkedList *);

int addFirst(linkedList *, listNodeType);
int addLast(linkedList *, listNodeType);
int insert(linkedList *, listNode *after, listNodeType);
int insertBefore(linkedList *, listNode *before, listNodeType);

int removeFirst(linkedList *);
int removeLast(linkedList *);
int removeFromList(linkedList *, listNode *);

int peekFirst(linkedList *, listNodeType *);
int peekLast(linkedList *, listNodeType *);

int isLinkedListEmpty(linkedList *);

unsigned int linkedListSize(linkedList *);

listNode *linkedListIterator(linkedList *);
int hasNext(listNode *);
listNode *next(listNode *);
listNode *find(linkedList *, listNodeType item);

void printLinkedList(linkedList *);

int readLinkedList(linkedList *list);

void freeLinkedList(linkedList *list);

#endif //UTILS_LINKEDLIST_H
