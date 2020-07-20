//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

#define TRUE 1
#define FALSE 0

listNode *createLinkedList(listNodeType item) {
	listNode *list;
    list = (listNode *)malloc(sizeof(listNode));
	if (list) {
        list->value = item;
        list->next = NULL;
	} else {
        printf("Failed to allocate memory for node item: %d\n", item);
    }

	return list;
}

void freeLinkedList(listNode *list) {
    while (!isLinkedListEmpty(list)) {
        // Remove also free the node.
        removeFirst(&list);
    }
}

int addFirst(listNode ** list, listNodeType item) {
	int success = FALSE;
	listNode *node;

	node = createLinkedList(item);

	if (node) {
		success = TRUE;

        node->next = (*list);
        (*list) = node;
	}

	return success;
}

int addLast(listNode ** list, listNodeType item) {
	int success = FALSE;
	listNode *node, *last;

	node = createLinkedList(item);

	if (node) {
		success = TRUE;

		if (!(*list)) {
            (*list) = node;
		} else {
		    tail(*list, &last);
		    last->next = node;
		}
	}

	return success;
}

int insert(listNode **after, listNodeType item) {
	int success = FALSE;
	listNode *node;

	if (isLinkedListEmpty(*after)) {
	    addFirst(after, item);
	} else {
        node = createLinkedList(item);

        if (node) {
            success = TRUE;
            node->next = (*after)->next;
            (*after)->next = node;
        }
    }

	return success;
}

listNode *findPrev(listNode *list, listNode *item) {
	listNode *node;

	if (item == list) {
	    return NULL;
	} else {
        node = list;
        while (hasNext(node) && (node->next != item)) {
            node = node->next;
        }

        return hasNext(node) ? node : NULL;
    }
}

int insertBefore(listNode ** list, listNode *before, listNodeType item) {
	int success = FALSE;
	listNode *node;
	listNode *prev;

	if (before) {
        node = createLinkedList(item);

        if (node) {
            prev = findPrev(*list, before);
            if (prev) {
                success = TRUE;
                node->next = before;
                prev->next = node;
            } else if (*list == before) {
                success = TRUE;
                node->next = before;
                *list = node;
            }
        }
    } else {
        success = addLast(list, item);
	}

	return success;
}

int removeFirst(listNode ** list) {
	int success = FALSE;
	listNode *node;

	if (!isLinkedListEmpty(*list)) {
		success = TRUE;
		node = *list;
		*list = node->next;

		free(node);
	}

	return success;
}

int removeLast(listNode ** list) {
	int success = FALSE;
	listNode *node, *last;

	if (!isLinkedListEmpty(*list)) {
		success = TRUE;

		if (!(*list)->next) {
            free(*list);
            (*list) = NULL;
		} else {
            tail(*list, &last);
			node = findPrev(*list, last);
			node->next = NULL;
            free(last);
		}
	}

	return success;
}

int removeFromList(listNode ** list, listNode *node) {
	int success = FALSE;
	listNode *prevNode;

	if (!isLinkedListEmpty(*list)) {
		if ((*list) == node) {
            success = removeFirst(list);
		} else {
			prevNode = findPrev(*list, node);
			if (!prevNode) {
				printf("Specified node - %d is not in list.\n", node->value);
			} else {
				success = TRUE;
				prevNode->next = node->next;
                free(node);
			}
		}
	}

	return success;
}

listNodeType value(listNode *node) {
    return node->value;
}

int head(listNode * list, listNode **item) {
	int success = FALSE;

	if (!isLinkedListEmpty(list)) {
        success = TRUE;
		*item = list;
	}

	return success;
}

int tail(listNode * list, listNode **item) {
    int success = FALSE;

    *item = list;
    while ((*item)->next) {
        *item = (*item)->next;
    }

    return success;
}

int isLinkedListEmpty(listNode * list) {
    return (list == NULL);
}

int hasNext(listNode *node) {
	return (node != NULL) && (node->next != NULL);
}

listNode *next(listNode *node) {
	return (node != NULL ? node->next : NULL);
}

listNode *find(listNode *list, listNodeType item) {
	listNode *result = NULL;
	listNode *node;

	for (node = list; node != NULL; node = node->next) {
		if (node->value == item) {
			result = node;
			break;
		}
	}

	return result;
}

void printLinkedList(listNode *list) {
	listNode *node;

    if (!list) {
        printf("NULL\n");
    } else {
        node = list;
        printf("%d", node->value);

        while ((node = node->next) != NULL) {
            printf("  ->  %d", node->value);
        }
        printf("  ->  %c\n", 186); // End of list symbol
    }
}