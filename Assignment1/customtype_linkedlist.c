//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include "customtype_linkedlist.h"

#define TRUE 1
#define FALSE 0

int initLinkedList_custom(linkedList_custom * list) {
	list->head = list->tail = NULL;
	list->size = 0;

	return TRUE;
}

void freeLinkedList_custom(linkedList_custom *list) {
    while (!isLinkedListEmpty_custom(list)) {
        // Remove also free the node.
        removeFirst_custom(list);
    }
}

listNode_custom *allocNode_three(listNodeType_custom value) {
	listNode_custom *node;
	node = (listNode_custom *)malloc(sizeof(listNode_custom));
	if (node) {
		node->value = value;
		node->next = NULL;
	} else {
        printf("Failed to allocate memory for node item.\n");
    }

	return node;
}

int addFirst_custom(linkedList_custom * list, listNodeType_custom item) {
	int success = FALSE;
	listNode_custom *node;

	node = allocNode_three(item);

	if (node) {
		success = TRUE;

		if (isLinkedListEmpty_custom(list)) {
			list->tail = node;
		}

		node->next = list->head;
		list->head = node;
		list->size++;
	}

	return success;
}

int addLast_custom(linkedList_custom * list, listNodeType_custom item) {
	int success = FALSE;
	listNode_custom *node;

	node = allocNode_three(item);

	if (node) {
		success = TRUE;

		if (isLinkedListEmpty_custom(list)) {
			list->head = node;
		} else {
			list->tail->next = node;
		}

		list->tail = node;
		list->size++;
	}

	return success;
}

int insert_custom(linkedList_custom * list, listNode_custom *after, listNodeType_custom item) {
	int success = FALSE;
	listNode_custom *node;

	if (after) {
        node = allocNode_three(item);

        if (node) {
            success = TRUE;

            node->next = after->next;
            after->next = node;

            if (!node->next) {
                list->tail = node;
            }

            list->size++;
        }
    } else {
        printf("Cannot insert to list after NULL element.\n");
    }

	return success;
}

listNode_custom *findPrev_three(linkedList_custom *list, listNode_custom *item) {
	listNode_custom *node;

    if (item == list->head) {
        return NULL;
    } else {
        node = linkedListIterator_custom(list);
        while (hasNext_custom(node) && (node->next != item)) {
            node = node->next;
        }

        return hasNext_custom(node) ? node : NULL;
    }
}

int insertBefore_custom(linkedList_custom * list, listNode_custom *before, listNodeType_custom item) {
	int success = FALSE;
	listNode_custom *node;
	listNode_custom *prev;

	if (before) {
        node = allocNode_three(item);

        if (node) {
            success = TRUE;

            node->next = before;

            prev = findPrev_three(list, before);
            if (prev) {
                prev->next = node;
            } else {
                list->head = node;
            }

            list->size++;
        }
    } else {
        success = addLast_custom(list, item);
	}

	return success;
}

int removeFirst_custom(linkedList_custom * list) {
	int success = FALSE;
	listNode_custom *node;

	if (!isLinkedListEmpty_custom(list)) {
		success = TRUE;
		node = list->head;
		list->head = node->next;

		// In case of last element.
		if (!list->head) {
			list->tail = NULL;
		}

		list->size--;
		free(node);
	}

	return success;
}

int removeLast_custom(linkedList_custom * list) {
	int success = FALSE;
	listNode_custom *node;

	if (!isLinkedListEmpty_custom(list)) {
		success = TRUE;
		node = list->tail;

		if (list->head == list->tail) {
            list->head = list->tail = NULL;
		} else {
			list->tail = findPrev_three(list, node);
			if (list->tail) {
				list->tail->next = NULL;
			}
		}

		list->size--;
		free(node);
	}

	return success;
}

int removeFromList_custom(linkedList_custom * list, listNode_custom *node) {
	int success = FALSE;
	listNode_custom *prevNode;

	if (!isLinkedListEmpty_custom(list)) {
		if ((list->head == list->tail) && (list->head == node)) {
			success = TRUE;
            list->head = list->tail = NULL;
            list->size--;
            free(node);
		} else if (list->head == node) {
            success = removeFirst_custom(list);
        } else if (list->tail == node) {
            success = removeLast_custom(list);
        } else {
			prevNode = findPrev_three(list, node);
			if (!prevNode) {
				printf("Specified node is not in list.\n");
			} else {
				success = TRUE;
				prevNode->next = node->next;
                list->size--;
                free(node);
			}
		}
	}

	return success;
}

listNodeType_custom value_custom(listNode_custom *node) {
    return node->value;
}

int head_custom(linkedList_custom * list, listNode_custom **item) {
	int success = FALSE;

	if (!isLinkedListEmpty_custom(list)) {
        success = TRUE;
		*item = list->head;
	} else {
	    *item = NULL;
	}

	return success;
}

int tail_custom(linkedList_custom * list, listNode_custom **item) {
	int success = FALSE;

	if (!isLinkedListEmpty_custom(list)) {
        success = TRUE;
		*item = list->tail;
    } else {
        *item = NULL;
    }

	return success;
}

int isLinkedListEmpty_custom(linkedList_custom * list) {
	return (list == NULL) || (list->head == NULL);
}

unsigned int linkedListSize_custom(linkedList_custom * list) {
    return (list == NULL ? 0 : list->size);
}

listNode_custom *linkedListIterator_custom(linkedList_custom * list) {
    return (list == NULL ? NULL : list->head);
}

int hasNext_custom(listNode_custom *list) {
	return (list != NULL) && (list->next != NULL);
}

listNode_custom *next_custom(listNode_custom *list) {
	return (list != NULL ? list->next : NULL);
}

listNode_custom *find_custom(linkedList_custom *list, listNodeType_custom item) {
	listNode_custom *result = NULL;
	listNode_custom *node;

	for (node = linkedListIterator_custom(list); node != NULL; node = node->next) {
		if (node->value == item) {
			result = node;
			break;
		}
	}

	return result;
}

void printLinkedList_custom(linkedList_custom *list, void (*printItemFunc)(listNodeType_custom)) {
	listNode_custom *node;

	if (!list) {
	    printf("NULL\n");
	} else {
        node = list->head;
        if (node) {
            printItemFunc(node->value);

            while ((node = node->next) != NULL) {
                printf("  ->  ");
                printItemFunc(node->value);
            }

            printf("  ->  %c", 186); // End of list symbol
        } else {
            printf("EMPTY");
        }

        printf("  (size=%d)\n", list->size);
    }
}