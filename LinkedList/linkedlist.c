//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

#define TRUE 1
#define FALSE 0

int createLinkedList(linkedList * list) {
	list->first = NULL;
	list->last = NULL;
	list->size = 0;

	return TRUE;
}

listNode *allocNode(listNodeType item) {
    listNode *node;
    node = malloc(sizeof(listNode));
    if (node) {
        node->value = item;
        node->next = NULL;
    }

    return node;
}

int addFirst(linkedList * list, listNodeType item) {
	int success = FALSE;
	listNode *node;

	node = allocNode(item);

	if (node) {
		success = TRUE;

		if (isLinkedListEmpty(list)) {
			list->last = node;
		}

		node->next = list->first;
		list->first = node;
		list->size++;
	}

	return success;
}

int addLast(linkedList * list, listNodeType item) {
	int success = FALSE;
	listNode *node;

    node = allocNode(item);

	if (node) {
		success = TRUE;

		if (isLinkedListEmpty(list)) {
			list->first = node;
		} else {
			list->last->next = node;
		}

		list->last = node;
		list->size++;
	}

	return success;
}

int insert(linkedList * list, listNode *after, listNodeType item) {
	int success = FALSE;
	listNode *node;

    node = allocNode(item);

	if (node) {
		success = TRUE;

		node->next = next(after);
		after->next = node;

		if (!node->next) {
		    list->last = node;
		}

        list->size++;
	}

	return success;
}

listNode *findPrev(linkedList *list, listNode *item) {
    listNode *node;

    node = linkedListIterator(list);
    while (hasNext(node) && next(node) != item) {
        node = next(node);
    }

    return hasNext(node) ? node : NULL;
}

int insertBefore(linkedList * list, listNode *before, listNodeType item) {
    int success = FALSE;
    listNode *node;
    listNode *prev;

    node = allocNode(item);

    if (node) {
        success = TRUE;

        node->next = before;

        prev = findPrev(list, before);
        if (prev) {
            prev->next = node;
        } else {
            list->first = node;
        }

        list->size++;
    }

    return success;
}

int removeFirst(linkedList * list) {
	int success = FALSE;
	listNode *node;

	if (!isLinkedListEmpty(list)) {
		success = TRUE;
		node = list->first;
		list->first = next(node);

		// In case of last element.
		if (!list->first) {
			list->last = NULL;
		}

		list->size--;
		free(node);
	}

	return success;
}

int removeLast(linkedList * list) {
	int success = FALSE;
	listNode *node;

	if (!isLinkedListEmpty(list)) {
		success = TRUE;
		node = list->last;

		if (list->first == list->last) {
			list->first = list->last = NULL;
		} else {
			list->last = findPrev(list, node);
            if (list->last) {
                list->last->next = NULL;
            }
		}

		list->size--;
		free(node);
	}

	return success;
}

int removeFromList(linkedList * list, listNode *node) {
	int success = FALSE, isCharList;
	listNode *prevNode;

	if (!isLinkedListEmpty(list)) {
		if ((list->first == list->last) && (list->first == node)) {
			success = TRUE;
			list->first = list->last = NULL;
		} else {
			prevNode = findPrev(list, node);
			if (!prevNode) {
				isCharList = (sizeof(listNodeType) == sizeof(char));
				if (isCharList) {
					printf("Specified node - %c is not in list.\n", node->value);
				} else {
					printf("Specified node - %d is not in list.\n", node->value);
				}
			} else {
				success = TRUE;
				prevNode->next = next(node);
			}
		}

		if (success) {
			list->size--;
			free(node);
		}
	}

	return success;
}

int peekFirst(linkedList * list, listNodeType *item) {
	int success = FALSE;

	if (!isLinkedListEmpty(list)) {
		*item = list->first->value;
	}

	return success;
}

int peekLast(linkedList * list, listNodeType *item) {
	int success = FALSE;

	if (!isLinkedListEmpty(list)) {
		*item = list->last->value;
	}

	return success;
}

int isLinkedListEmpty(linkedList * list) {
	return list->size == 0;
}

unsigned int linkedListSize(linkedList * list) {
	return list->size;
}

listNode *linkedListIterator(linkedList * list) {
	return list->first;
}

int hasNext(listNode *node) {
	return (node != NULL) && (next(node) != NULL);
}

listNode *next(listNode *node) {
	return (node != NULL ? node->next : NULL);
}

listNode *find(linkedList *list, listNodeType item) {
	listNode *result = NULL;
	listNode *node;

    for (node = linkedListIterator(list); node != NULL; node = next(node)) {
		if (node->value == item) {
			result = node;
			break;
		}
	}

	return result;
}

void printLinkedList(linkedList *list) {
	int isCharList;
	listNode *node;

	isCharList = (sizeof(listNodeType) == sizeof(char));

	node = linkedListIterator(list);
	if (node) {
		if (isCharList) {
			printf("%c", node->value);
		} else {
			printf("%d", node->value);
		}
	}

	while (hasNext(node)) {
		node = next(node);

		if (isCharList) {
			printf("  ->  %c", node->value);
		} else {
			printf("  ->  %d", node->value);
		}
	}
    printf("  ->  %c", 186);

	printf("  (size=%d)\n", linkedListSize(list));
}

int readLinkedList(linkedList *list) {
	unsigned int n;
	int success, isCharQueue;
	listNodeType item;

	isCharQueue = (sizeof(listNodeType) == sizeof(char));
	success = createLinkedList(list);

	if (success) {
		printf("Enter amount of %s to scan into linked list:\n", isCharQueue ? "characters" : "numbers");
		scanf_s("%d%*c", &n);

		printf("Enter %d items:\n", n);
		while (n-- > 0) {
			if (isCharQueue) {
				scanf_s("%c%*c", &item, sizeof(listNodeType));
			} else {
				scanf_s("%d%*c", &item);
			}
			addLast(list, item);
		}
	}

	return success;
}

void freeLinkedList(linkedList *list) {
	while (!isLinkedListEmpty(list)) {
		// Remove also free the node.
		removeFirst(list);
	}
}