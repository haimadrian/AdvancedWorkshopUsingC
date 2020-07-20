//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stringutil.h"

#define TRUE 1
#define FALSE 0

int reverseString(string str);
void countChars(string str, int *lowerCaseLettersCount, int *upperCaseLettersCount, int *digitsCount, int *restCount);
void trim(string *str);

int main() {
    string str;
    int lowerCaseLettersCount, upperCaseLettersCount, digitsCount, restCount;

    printf("Please enter a string to reverse:\n");
    str = readString();

    if (reverseString(str)) {
        printf("No change has been made on the input string: %s\n", str);
    } else {
        printf("Reversed string is: %s\n", str);
    }

    countChars(str, &lowerCaseLettersCount, &upperCaseLettersCount, &digitsCount, &restCount);
    printf("\nThere are:\n%d lower case characters\n%d upper case characters\n%d digit characters\n%d other characters\n", lowerCaseLettersCount, upperCaseLettersCount, digitsCount, restCount);

    trim(&str);
    printf("\nTrimmed string is: %s\n", str);

    free(str);

    return 0;
}

int reverseString(string str) {
    int noChange = TRUE, strLength = (int)strlen(str), left = 0, right = strLength - 1;
    char temp;

    while (left < right) {
        if (*(str + left) != *(str + right)) {
            noChange = FALSE;

            // Swap
            temp = *(str + left);
            *(str + left) = *(str + right);
            *(str + right) = temp;
        }

        left++;
        right--;
    }

    return noChange;
}

void countChars(string str, int *lowerCaseLettersCount, int *upperCaseLettersCount, int *digitsCount, int *restCount) {
    int i, strLength = (int)strlen(str);
    char currChar;

    *lowerCaseLettersCount = *upperCaseLettersCount = *digitsCount = *restCount = 0;

    for (i = 0; i < strLength; i++) {
        currChar = *(str + i);
        if (islower(currChar)) {
            (*lowerCaseLettersCount)++;
        } else if (isupper(currChar)) {
            (*upperCaseLettersCount)++;
        } else if (isdigit(currChar)) {
            (*digitsCount)++;
        } else {
            (*restCount)++;
        }
    }
}

void trim(string *str) {
    int i, actual, strLength = (int)strlen(*str);
    int firstChar = 0;

    while ((firstChar < strLength) && ((*str)[firstChar] == ' ')) {
        firstChar++;
    }

    for (i = firstChar, actual = 0; i < strLength; i++) {
        if ((*str)[i] == ' ') {
            if ((*str)[actual - 1] != ' ') {
                (*str)[actual++] = ' ';
            }

            continue;
        }

        (*str)[actual++] = (*str)[i];
    }

    // If there was no change, there is no reason to realloc.
    if (actual != strLength) {
        // Remove last space.
        (*str)[((*str)[actual - 1] == ' ' ? actual - 1 : actual++)] = '\0';
        *str = realloc(*str, actual * sizeof(char));
    }
}