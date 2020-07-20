//
// Created by Haim Adrian on 21-Aug-19.
//

#ifndef UTILS_SAFESCANF_H
#define UTILS_SAFESCANF_H

typedef unsigned int uint;

int scanIntSafe();
uint scanUIntSafe();

int mulIntSafe(int a, int b, int *result);
int addIntSafe(int a, int b, int *result);

#endif //UTILS_SAFESCANF_H
