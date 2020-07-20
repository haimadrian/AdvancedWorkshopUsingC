//
// Created by Haim Adrian
//

#ifndef ASSIGNMENT1_THREE_H
#define ASSIGNMENT1_THREE_H

typedef struct three {
    int value, row, col;
} three;

three *createThree(int value, int row, int col);
void freeThree(three *three1);
void printThree(three *three1);

#endif //ASSIGNMENT1_THREE_H
