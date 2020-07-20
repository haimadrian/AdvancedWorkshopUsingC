//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include "stringutil.h"

// Default starting length for a dynamic string.
#define DEFAULT_STR_LEN 64

string readString(int *eofEncountered) {
	return readStringWithCustomLength(0, eofEncountered);
}

string readStringWithCustomLength(int length, int *eofEncountered) {
    string input;
    int len = 0, capacity = length <= 0 ? DEFAULT_STR_LEN : length + 1;
    int c;
    if (eofEncountered != NULL) *eofEncountered = 0;

    input = (string)calloc(capacity, sizeof(char));
    if (!input) {
        printf("Failed to read string. Insufficient memory.\n");
        return NULL;
    }

	// 26 = SUB = Ctrl+Z
    while (((c = fgetc(stdin)) != '\n') && (c != EOF) && (c != 26)) {
        // Ignore \r in case we are running on linux
        if (c != '\r') {
            // In case we have reached end of string. saving last index for \0, enlarge string.
            if (len >= capacity - 1) {
                // In case we have been asked for custom length, and we reach it, stop.
                if (length > 0) {
                    while (((c = getchar()) != '\n') && (c != EOF) && (c != 26));
                    break;
                }

                capacity = (int) (capacity * 1.5);
                input = (string)realloc(input, capacity * sizeof(char));
            }

            input[len++] = (char)c;
        }
    }

    if (eofEncountered != NULL) *eofEncountered = ((c == EOF) || (c == 26));

    if (len == 0) {
        free(input);
        input = NULL;
    } else {
        input[len++] = '\0';

        if (len != capacity) {
            input = (string) realloc(input, len * sizeof(char));
        }
    }

    return input;
}