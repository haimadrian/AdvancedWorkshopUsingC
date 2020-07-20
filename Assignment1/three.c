//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include "three.h"

three *createThree(int value, int row, int col) {
    three *result = (three *)malloc(sizeof(three));

    if (result) {
        result->value = value;
        result->row = row;
        result->col = col;
    } else {
        printf("Failed to allocate memory for three: [%d, %d, %d]\n", value, row, col);
    }

    return result;
}

void freeThree(three *three1) {
    if (three1) {
        free(three1);
    }
}

void printThree(three *three1) {
    if (three1) {
        printf("(%d, %d, %d)", three1->value, three1->row, three1->col);
    } else {
        printf("(NULL)");
    }
}