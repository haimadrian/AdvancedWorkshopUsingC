//
// Created by Haim Adrian
//

#ifndef UTILS_SAFE_H
#define UTILS_SAFE_H

typedef unsigned int uint;

int scanIntSafe(int *eofEncountered);
float scanFloatSafe(int *eofEncountered);

int addFloatSafe(float a, float b, float *result);

#endif //UTILS_SAFE_H
