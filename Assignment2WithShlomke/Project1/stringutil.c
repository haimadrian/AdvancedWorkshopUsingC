//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringutil.h"

// Default starting length for a dynamic string.
#define DEFAULT_STR_LEN 64

string readString() 
{
	string input;
	int length = 0, capacity = DEFAULT_STR_LEN;
	unsigned char c;

	input = (string)malloc(capacity * (sizeof(char)));
	if (!input) 
	{
		printf("Failed to read string. Insufficient memory.\n");
		return NULL;
	}

	while ((c = getchar()) != '\n') 
	{
		// Ignore \r in case we are running on linux
		if (c != '\r') 
		{
			// In case we have reached end of string. saving last index for \0, enlarge string.
			if (length >= capacity - 1) 
			{
				capacity = (int)(capacity * 1.5);
				input = (string)realloc(input, capacity * sizeof(char));
			}

			input[length++] = c;
		}
	}

	input[length++] = '\0';

	if (length != capacity) 
	{
		input = (string)realloc(input, length * sizeof(char));
	}

	return input;
}

// trims spaces before the word
void trim(string* str) 
{
	int i = 0, count = 0;
	if ((*str)[i] != ' ') 
	{
		return;
	}

	while ((*str)[i] == ' ') 
	{
		count++;
		i++;
	}

	delete(str, count);
}

// to check the begining of declaration
int startWith(string a, string declaration) 
{
	int size, i;
	size = (int)min(strlen(a), strlen(declaration));
	for (i = 0; i < size; i++) 
	{
		if (a[i] != declaration[i]) 
		{
			return 0;
		}
	}
	
	return 1;
}

void delete(string* str, int count)
{
	int i, size;
	size = (int)strlen(*str);

	for (i = count; i < size; i++) 
	{
		(*str)[i - count] = (*str)[i];
	}

	(*str)[i - count] = '\0';

	*str = (string)realloc(*str, sizeof(char)*(size - count + 1));
}

