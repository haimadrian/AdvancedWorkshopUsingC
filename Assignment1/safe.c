//
// Created by Haim Adrian on 21-Aug-19.
//

#include <stdio.h>
#include "safe.h"

#define TRUE 1
#define FALSE 0

// Don't know if limits.h library will be available at tester env, so I copied the definitions here.
#define INT_MIN (-2147483647 - 1) // minimum (signed) int value
#define INT_MAX 2147483647    // maximum (signed) int value
#define UINT_MAX 0xffffffff    // maximum unsigned int value

typedef unsigned int uint;
typedef int bool;

int scanIntSafe() {
    int scanfResult, result = 0;
    bool tryAgain = TRUE;
    double temp;

    do {
        scanfResult = scanf_s("%lf%*c", &temp);
        if (scanfResult == EOF) {
            printf("Error has encountered while reading a number from stdin. Please try again.\n");
        } else if (scanfResult == 0) {
            printf("Illegal input has entered. Valid input is: Integer. Please try again.\n");
            while (fgetc(stdin) != '\n'); // Read until a newline is found to clear the illegal input
        } else if ((temp < INT_MIN) || (temp > INT_MAX)) {
            printf("Illegal input has entered (Out of Range). Integer range is: [%d, %d]. Please try again.\n", INT_MIN, INT_MAX);
        } else {
            result = (int)temp;
            tryAgain = FALSE;
        }
    } while (tryAgain);

    return result;
}

uint scanUIntSafe() {
    int scanfResult;
    uint result = 0;
    bool tryAgain = TRUE;
    double temp;

    do {
        scanfResult = scanf_s("%lf%*c", &temp);
        if (scanfResult == EOF) {
            printf("Error has encountered while reading a number from stdin. Please try again.\n");
        } else if (scanfResult == 0) {
            printf("Illegal input has entered. Valid input is: Natural number. Please try again.\n");
            while (fgetc(stdin) != '\n'); // Read until a newline is found to clear the illegal input
        } else if ((temp < 0) || (temp > UINT_MAX)) {
            printf("Illegal input has entered (Out of Range). Natural number range is: [0, %u]. Please try again.\n", UINT_MAX);
        } else {
            result = (uint)temp;
            tryAgain = FALSE;
        }
    } while (tryAgain);

    return result;
}

bool tryScanUIntSafe(uint *out) {
    int scanfResult;
    bool success = FALSE;
    double temp;

    scanfResult = scanf_s("%lf%*c", &temp);
    if (scanfResult == EOF) {
        printf("Error has encountered while reading a number from stdin. Please try again.\n");
    } else if (scanfResult == 0) {
        printf("Illegal input has entered. Valid input is: Natural number. Please try again.\n");
        while (fgetc(stdin) != '\n'); // Read until a newline is found to clear the illegal input
    } else if (temp > UINT_MAX) {
        printf("Illegal input has entered (Out of Range). Natural number max value is: [%u]. Please try again.\n", UINT_MAX);
    } else if (temp >= 0) {
        *out = (uint)temp;
        success = TRUE;
    }

    return success;
}

int mulIntSafe(int a, int b, int *result) {
    double mul = (double)a * (double)b;

    if ((mul > INT_MAX) || (mul < INT_MIN)) {
        printf("Multiplication failed: Result of %d*%d exceeds integer buffer.\n", a, b);
        return FALSE;
    }

    *result = (int)mul;

    return TRUE;
}

int addIntSafe(int a, int b, int *result) {
    double add = (double)a + (double)b;

    if ((add > INT_MAX) || (add < INT_MIN)) {
        printf("Addition failed: Result of %d+%d exceeds integer buffer.\n", a, b);
        return FALSE;
    }

    *result = (int)add;

    return TRUE;
}

int subIntSafe(int a, int b, int *result) {
	double sub = (double)a - (double)b;

	if ((sub > INT_MAX) || (sub < INT_MIN)) {
		printf("Subtraction failed: Result of %d-%d exceeds integer buffer.\n", a, b);
		return FALSE;
	}

	*result = (int)sub;

	return TRUE;
}

