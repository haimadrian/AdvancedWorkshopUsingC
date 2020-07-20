//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include "stringutil.h"

// Default starting length for a dynamic string.
#define DEFAULT_STR_LEN 64

string readString() {
	string input;
	int length = 0, capacity = DEFAULT_STR_LEN;
	unsigned char c;

	input = malloc(capacity * (sizeof(char)));
	if (!input) {
		printf("Failed to read string. Insufficient memory.\n");
		return NULL;
	}

	while ((c = getchar()) != '\n') {
		// Ignore \r in case we are running on linux
		if (c != '\r') {
			// In case we have reached end of string. saving last index for \0, enlarge string.
			if (length >= capacity - 1) {
				capacity = (int) (capacity * 1.5);
				input = realloc(input, capacity * sizeof(char));
			}

			input[length++] = c;
		}
	}

	input[length++] = '\0';

	if (length != capacity) {
		input = realloc(input, length * sizeof(char));
	}

	return input;
}