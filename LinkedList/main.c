//
// Created by Haim Adrian
//

#include <stdio.h>
#include "linkedlist.h"

#define TRUE 1
#define FALSE 0

int insertToSortedList(linkedList *list, listNodeType item);
int readSortedLinkedList(linkedList *list);
void reverseList(linkedList *list);

int main() {
    linkedList list;

    readSortedLinkedList(&list);

    printf("Sorted list:\n");
    printLinkedList(&list);

    reverseList(&list);

    printf("Reversed:\n");
    printLinkedList(&list);

    freeLinkedList(&list);

    printf("\nPress any key to continue...\n");
    getchar();

    return 0;
}

int insertToSortedList(linkedList *list, listNodeType item) {
    int success;
    listNode *iterator = linkedListIterator(list);
    listNode *prev = NULL;

    if (isLinkedListEmpty(list) || (item <= iterator->value)) {
        success = addFirst(list, item);
    } else {
        // O(n)
        prev = iterator;
        while (hasNext(iterator) && ((iterator = next(iterator))->value < item)) {
            prev = iterator;
        }

        // O(1)
        success = insert(list, prev, item);
    }

    return success;
}

int readSortedLinkedList(linkedList *list) {
    unsigned int n;
    int success;
    listNodeType item;

    success = createLinkedList(list);

    if (success) {
        printf("Enter amount of characters to scan into linked list:\n");
        scanf_s("%d%*c", &n);

        printf("Enter %d items:\n", n);
        while (n-- > 0) {
            scanf_s("%c%*c", &item, sizeof(listNodeType));
            insertToSortedList(list, item);
        }
    }

    return success;
}

void reverseList(linkedList *list) {
    listNode *iterator = linkedListIterator(list);
    listNode *temp;
    listNode *prev;

    list->first = list->last;
    list->last = iterator;

    if (linkedListSize(list) > 1) {
        prev = iterator;
        iterator = next(iterator);
        prev->next = NULL;

        while (iterator != NULL) {
            temp = iterator;
            iterator = next(iterator);
            temp->next = prev;
            prev = temp;
        }
    }
}