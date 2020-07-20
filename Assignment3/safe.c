//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "safe.h"
#include "stringutil.h"

#define TRUE 1
#define FALSE 0

// Don't know if limits.h library will be available at tester env, so I copied the definitions here.
#define INT_MIN (-2147483647 - 1)  // minimum (signed) int value
#define INT_MAX 2147483647         // maximum (signed) int value
#define FLT_MIN 1.175494351e-38F   // min normalized positive value
#define FLT_MAX 3.402823466e+38F   // max normalized positive value

typedef unsigned int uint;
typedef int bool;

int scanIntSafe(int *eofEncountered) {
    long double temp = -1;
    int scanfResult, result = -1;
    string inputStr, endOfNum = NULL;
    bool tryAgain = TRUE;
    if (eofEncountered != NULL) *eofEncountered = 0;

    do {
        inputStr = readString(eofEncountered);
        if (inputStr != NULL) {
            temp = strtold(inputStr, &endOfNum);
            if (endOfNum && strlen(endOfNum) > 0) {
                // If we could not parse anything, set 0 in scanfResult.
                if (inputStr == endOfNum) {
                    scanfResult = 0;
                } else {
                    // How many bytes we have succeeded to parse.
                    scanfResult = (int)(strlen(inputStr) - strlen(endOfNum));
                }
            } else {
                scanfResult = (int)strlen(inputStr);
            }

            free(inputStr);
        } else {
            scanfResult = 0;
        }

        if (eofEncountered && *eofEncountered) {
            scanfResult = EOF;
        }

        if (scanfResult != 0) {
            if ((temp < INT_MIN) || (temp > INT_MAX)) {
                printf("Illegal input has entered (Out of Range). Integer range is: [%d, %d]. Please try again.\n", INT_MIN, INT_MAX);
            } else {
                result = (int) temp;
                tryAgain = FALSE;
            }
        } else {
            printf("Illegal input has entered. Valid input is: Integer. Please try again.\n");
        }
    } while (tryAgain);

    return result;
}

float scanFloatSafe(int *eofEncountered) {
    long double temp = -1;
    int scanfResult;
    float result = -1;
    string inputStr, endOfNum = NULL;
    bool tryAgain = TRUE;
    if (eofEncountered != NULL) *eofEncountered = 0;

    do {
        inputStr = readString(eofEncountered);
        if (inputStr != NULL) {
            temp = strtold(inputStr, &endOfNum);
            if (endOfNum && strlen(endOfNum) > 0) {
                // If we could not parse anything, set 0 in scanfResult.
                if (inputStr == endOfNum) {
                    scanfResult = 0;
                } else {
                    // How many bytes we have succeeded to parse.
                    scanfResult = (int)(strlen(inputStr) - strlen(endOfNum));
                }
            } else {
                scanfResult = (int)strlen(inputStr);
            }

            free(inputStr);
        } else {
            scanfResult = 0;
        }

        if (eofEncountered && *eofEncountered) {
            scanfResult = EOF;
        }

        if (scanfResult != 0) {
            if ((temp < FLT_MIN) || (temp > FLT_MAX)) {
                printf("Illegal input has entered (Out of Range). Float range is: [%f, %f]. Please try again.\n", FLT_MIN, FLT_MAX);
            } else {
                result = (float) temp;
                tryAgain = FALSE;
            }
        } else {
            printf("Illegal input has entered. Valid input is: Float. Please try again.\n");
        }
    } while (tryAgain);

    return result;
}

int addFloatSafe(float a, float b, float *result) {
    double add = (double) a + (double) b;

    if ((add > FLT_MAX) || (add < FLT_MIN)) {
        printf("Addition failed: Result of %f+%f exceeds float buffer.\n", a, b);
        return FALSE;
    }

    *result = (float) add;

    return TRUE;
}
