/**
 * Assigned By
 * -----------
 * Name: Haim Adrian
 * Group: 61108-61
 *
 * Name: Shlomki Amir
 * Group: 61108-61
 */

#include "three.h"

// Set it so we will define the custom list type here, and not the default one in customtype_linkedlist.h
#define CUSTOM_LIST_NODE_TYPE
typedef three * listNodeType_custom;

#include <stdio.h>
#include <stdlib.h>
#include "safe.h"
#include "matrix.h"
#include "customtype_linkedlist.h"
#include "linkedlist.h"

#define TRUE 1
#define FALSE 0
#define FIRST_MAT_ROWS 3
#define FIRST_MAT_COLS 3
#define SECOND_MAT_ROWS 3
#define SECOND_MAT_COLS 4

typedef unsigned int uint;
typedef three * arrType_three;

/// Ex Functions ///
int showMainMenu();
void ex1();
void ex2();
void ex3();
void ex4();
void ex5();


/// Logic Functions ///
uint *powerArray(uint n); // ex1
matrix matrixMultiplication(matrix a, int aRows, int aCols, matrix b, int bRows, int bCols); // ex2
uint createArrayAndList(matrix a, int rows, int cols, arrType_three **arr, linkedList_custom **list); // ex3
listNode *createList(listNode **list); // ex4
listNode *createList2(listNode **list); // ex5


/// Utils ///
void printUIntArray(const char *title, uint *arr, uint length);


/// Main ///

int main() {
    int action;

    do {
        action = showMainMenu();
        switch (action) {
            case 1: ex1(); break;
            case 2: ex2(); break;
            case 3: ex3(); break;
            case 4: ex4(); break;
            case 5: ex5(); break;
            case 0:  break;
            default: printf("Invalid action: %d. Please try again.\n", action);
        }
        printf("\n");
    } while (action != 0);

    printf("Good Bye. Press any key to continue...\n");
    getchar();

    return 0;
}

//////////////////// EX1 ////////////////////
uint *powerArray(uint n) {
    uint *arr = NULL, power = 1;
    uint i;

    if (n > 0) {
        arr = (uint *)malloc(n * sizeof(uint));
        if (!arr) {
            printf("Failed to allocate memory for power array. Insufficient memory. (requested length was %u)\n", n);
        } else {
            for (i = 0; i < n; i++) {
                // Start multiplying from 2^0 when i divides by 32 with no remainder.
                if ((i % 32) == 0) {
                    power = 1; // i = 0, 32, 64, 96, ...
                } else {
                    power = (power << ((uint)1)); // Efficient way for multiplying by 2: (power *= 2) == (power = (power << 1));
                }

                arr[i] = power;
            }
        }
    }

    return arr;
}

void ex1() {
    uint *arr;
    uint length;

    printf("Enter amount of elements in power array:\n");
    length = scanUIntSafe();

    arr = powerArray(length);

    printUIntArray("\nPower array is:", arr, length);

    if (arr) {
        free(arr);
    }
}


//////////////////// EX2 ////////////////////
matrix matrixMultiplication(matrix a, int aRows, int aCols, matrix b, int bRows, int bCols) {
    matrix mat = NULL;
    int mulRow, mulCol, i, sum, mul;

    // Do something only in case we can multiply matrices (amount of columns in A equals to the amount of rows in B)
    if (aCols == bRows) {
        // Multiplication matrix will have same amount of rows as A and same amount of columns as B.
        // createMatrix uses calloc so we can count.
        mat = createMatrix(aRows, bCols);

        for (mulRow = 0; mulRow < aRows; mulRow++) {
            for (mulCol = 0; mulCol < bCols; mulCol++) {
                sum = 0;
                for (i = 0; i < aCols; i++) {
                    // This if is a safe implementation for sum += (a[mulRow][i] * b[i][mulCol]), to make sure we do not exceed integer range.
                    if ((!mulIntSafe(a[mulRow][i], b[i][mulCol], &mul)) || (!(addIntSafe(sum, mul, &sum)))) {
                        sum = 0; // Overflow. set sum = 0 and stop current cell calculation.
                        break;
                    }
                }

                mat[mulRow][mulCol] = sum;
            }
        }
    } else {
        printf("Cannot multiply matrices. Amount of columns in matrix A differs from the amount of rows in matrix B. (A[%d][%d], B[%d][%d])\n", aRows, aCols, bRows, bCols);
    }

    return mat;
}

void ex2() {
    matrixType a[FIRST_MAT_ROWS][FIRST_MAT_COLS] = {{1, 2, 3},
                                                    {4, 5, 6},
                                                    {7, 8, 9}};
    matrixType b[SECOND_MAT_ROWS][SECOND_MAT_COLS] = {{1, 2, 3, 4},
                                                      {5, 6, 7, 8},
                                                      {9, 10, 11, 12}};
    matrix customA, customB, mulMatrix;
    int aRows, aCols, bRows, bCols;
    char action;

    printf("Would you like to enter custom matrices for multiplication? (Press Y to enter custom matrices or N to use static matrices)\n");
    action = (char)getchar();

    // Dynamic matrices
    if ((action == 'Y') || (action == 'y')) {
        customA = readCustomSizeMatrix(&aRows, &aCols);
        if (customA) {
            customB = readCustomSizeMatrix(&bRows, &bCols);
            if (customB) {
                mulMatrix = matrixMultiplication(customA, aRows, aCols, customB, bRows, bCols);
                if (mulMatrix) {
                    printf("\nResult of multiplication:\n");
                    printMatricesExpression(customA, aRows, aCols, customB, bRows, bCols, mulMatrix, aRows, bCols, '*');
                    freeMatrix(&mulMatrix, aRows);
                }

                freeMatrix(&customB, bRows);
            }

            freeMatrix(&customA, aRows);
        }
    }
    // Static matrices
    else {
        customA = (matrix)malloc(FIRST_MAT_ROWS * sizeof(matrixType *));
        if (customA) {
            for (aRows = 0; aRows < FIRST_MAT_ROWS; aRows++) {
                customA[aRows] = &(a[aRows][0]);
            }

            customB = (matrix)malloc(SECOND_MAT_ROWS * sizeof(matrixType *));
            if (customB) {
                for (bRows = 0; bRows < SECOND_MAT_ROWS; bRows++) {
                    customB[bRows] = &(b[bRows][0]);
                }

                mulMatrix = matrixMultiplication(customA, FIRST_MAT_ROWS, FIRST_MAT_COLS, customB, SECOND_MAT_ROWS, SECOND_MAT_COLS);
                if (mulMatrix) {
                    printf("\nResult of multiplication:\n");
                    printMatricesExpression(customA, FIRST_MAT_ROWS, FIRST_MAT_COLS, customB, SECOND_MAT_ROWS, SECOND_MAT_COLS, mulMatrix,
                                            FIRST_MAT_ROWS, SECOND_MAT_COLS, '*');
                    freeMatrix(&mulMatrix, FIRST_MAT_ROWS);
                }

                free(customB);
            }

            free(customA);
        }
    }
}


//////////////////// EX3 ////////////////////
uint createThreeList(matrix a, int rows, int cols, linkedList_custom **list) {
    listNodeType_custom currThree;
    uint length = 0;
    int row, col, value;

    *list = NULL;

    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            value = row + col;

            // Make sure value meets requirements and we also succeed to create a new Three.
            if ((a[row][col] == value) && (currThree = createThree(value, row, col))) {
                length++;

                // Lazy initialization
                if (!(*list)) {
                    *list = (linkedList_custom *)malloc(sizeof(linkedList_custom));
                    initLinkedList_custom(*list);
                }

                addLast_custom(*list, currThree);
            }
        }
    }

    return length;
}

uint createThreeArr(matrix a, int rows, int cols, arrType_three **arr) {
    three *currThree;
    uint length = 0, capacity = rows * cols;
    int row, col, value;

    *arr = NULL;

    // As a result of scanning a matrix, we prefer to scan it once and use a dynamic array, rather than scan it twice.
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            value = row + col;

            // Make sure value meets requirements and we also succeed to create a new Three.
            if ((a[row][col] == value) && (currThree = createThree(value, row, col))) {
                // Lazy initialization
                if (!(*arr)) {
                    *arr = (arrType_three *) malloc(capacity * sizeof(arrType_three));
                }

                (*arr)[length++] = currThree;
            }
        }
    }

    // Cut the rest of the elements
    if ((length > 0) && (length < capacity)) {
        *arr = (arrType_three *) realloc(*arr, length * sizeof(arrType_three));
    }

    return length;
}

uint createArrayAndList(matrix a, int rows, int cols, arrType_three **arr, linkedList_custom **list) {
    uint length;

    length = createThreeArr(a, rows, cols, arr);
    createThreeList(a, rows, cols, list);

    return length;
}

void printArray_three(arrType_three *arr, uint length) {
    uint i;

    if (!arr) {
        printf("NULL\n");
    } else if (length == 0) {
        printf("[] (length=0)\n");
    } else {
        printf("[");
        printThree(arr[0]);
        for (i = 1; i < length; i++) {
            printf(", ");
            printThree(arr[i]);
        }
        printf("] (length=%d)\n", length);
    }
}

void ex3() {
    matrix mat;
    arrType_three *arr;
    linkedList_custom *list;
    listNode_custom *listIter;
    int rows, cols;
    uint length;

    mat = readCustomSizeMatrix(&rows, &cols);
    if (mat) {
        length = createArrayAndList(mat, rows, cols, &arr, &list);

        printf("\nInput matrix is:\n");
        printMatrix(mat, rows, cols);

        if (length == 0) {
            printf("\nNo three matches could be found in the specified matrix.\n");
        } else {
            printf("\nA three format is: (VALUE, ROW, COL)\n");
            printf("Output array of threes is:\n");
            printArray_three(arr, length);

            printf("Output linked list of threes is:\n");
            printLinkedList_custom(list, printThree);

            // Free list including its values.
            listIter = linkedListIterator_custom(list);
            while (listIter) {
                freeThree(value_custom(listIter));
                listIter = next_custom(listIter);
            }
            freeLinkedList_custom(list);
            free(list);

            // Free array and its values.
            while (length-- > 0) {
                freeThree(arr[length]);
            }
            free(arr);
        }

        freeMatrix(&mat, rows);
    }
}


//////////////////// EX4 ////////////////////
void delete(listNode **list, listNode *node) {
    // Just call removeFromList, which fixes links.
    // Please note that we would implement it without calling insert & delete, because deletion of an element is inefficient. We could
    // update links while iterating the linked list and free the element by ourselves, just like we do with prevNode reference in ex5.
    // But the exercise demands to use two separate procedures, one for insert and another for delete. So please do not decrease points on that.
    removeFromList(list, node);
}

listNode *createList(listNode **list) {
    listNode *oddList = NULL;
    listNode *listIter, *temp = NULL;
    listNodeType nodeValue;

    listIter = *list;
    while (listIter) {
        nodeValue = value(listIter);

        if (!(nodeValue % 2)) {
            // If it is an even number, continue to next iteration.
            listIter = listIter->next;
        }
        // If current value is an odd number
        else {
            // When odd list is empty, tail will refer to NULL, so the insert will add the node to the head of the list in O(1)
            addLast(&oddList, nodeValue);

            temp = listIter;
            listIter = listIter->next;

            delete(list, temp);
        }
    }

    return oddList;
}

void ex4() {
    listNode *list = NULL, *oddList;
    listNodeType item;

    printf("Enter integer numbers or negative number to stop:\n");
    while (tryScanUIntSafe(&item)) {
        addLast(&list, item);
    }

    printf("\nInput list is:\n");
    printLinkedList(list);

    oddList = createList(&list);

    printf("Even list (modified input list):\n");
    printLinkedList(list);
    printf("Odd list:\n");
    printLinkedList(oddList);

    freeLinkedList(list);
    freeLinkedList(oddList);
}


//////////////////// EX5 ////////////////////
listNode *createList2(listNode **list) {
    listNode *oddList = NULL;
    listNode *listIter, *oddListIter = NULL, *prevNode = NULL;
    listNodeType nodeValue;

    listIter = *list;
    while (listIter) {
        nodeValue = value(listIter);

        if (!(nodeValue % 2)) {
            // If it is an even number, continue to next iteration.
            prevNode = listIter;
            listIter = listIter->next; // next
        }
        // If current value is an odd number
        else {
            if (isLinkedListEmpty(oddList)) {
                // In case current node should be the first (and only) node in oddList, just set it.
                oddListIter = oddList = listIter;
            } else {
                // Otherwise, update link and continue with oddListIter.
                oddListIter->next = listIter;
                oddListIter = oddListIter->next;
            }

            listIter = listIter->next;
            oddListIter->next = NULL; // Now update tail of oddList (we use addLast strategy)

            // If there is a previous node (meaning it's not the head of the even list)
            if (prevNode) {
                prevNode->next = listIter;
            } else {
                // Handle head item
                *list = listIter;
            }
        }
    }

    return oddList;
}

void ex5() {
    listNode *list = NULL, *oddList;
    listNodeType item;

    printf("Enter integer numbers or negative number to stop:\n");
    while (tryScanUIntSafe(&item)) {
        addLast(&list, item);
    }

    printf("\nInput list is:\n");
    printLinkedList(list);

    oddList = createList2(&list);

    printf("Even list (modified input list):\n");
    printLinkedList(list);
    printf("Odd list:\n");
    printLinkedList(oddList);

    freeLinkedList(list);
    freeLinkedList(oddList);
}

int showMainMenu() {
    int action;

    printf("Choose an exercise or 0 to exit.\n");
    printf("1. Powers of 2\n");
    printf("2. Matrix multiplication\n");
    printf("3. Array and list of Threes\n");
    printf("4. Filter odd numbers using memory allocations (delete & insert)\n");
    printf("5. Filter odd numbers with no memory allocations\n");
    printf("0. Exit\n");
    action = scanIntSafe();

    return action;
}

/// Utils ///

void printUIntArray(const char *title, uint *arr, uint length) {
    uint i;

    if (title) {
        printf("%s\n", title);
    }

    if (!arr) {
        printf("NULL\n");
    } else if (length == 0) {
        printf("[] (length=0)\n");
    } else {
        printf("[%u", arr[0]);
        for (i = 1; i < length; i++) {
            printf(", %u", arr[i]);
        }
        printf("] (length=%d)\n", length);
    }
}