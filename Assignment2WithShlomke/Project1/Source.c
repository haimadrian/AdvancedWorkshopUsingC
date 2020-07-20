/*
* Assigned By
* -----------
* Name: Haim Adrian
* Group: 61108
*
* Name: Shlomki Amir
* Group: 61108
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "safe.h"
#include "stringutil.h"

void ex1();
void ex2();
void ex3();
void ex4();
void ex5();

// 1
string* split(string str, char letter, int *size);

// 2
string clearDigitsAndSpaces(string str);

// 3
void createFile(string fileName);
char commonestLetter(string fileName);

// 4
void decode(string str);

// 5
void memoryReport(string declaration, string fileName);
void printFromFile(string fileName);

void main()
{
	int num;

	do 
	{
		printf("please choose an exercise\n");
		printf("1. Split\n");
		printf("2. Remove Space and Digits\n");
		printf("3. Commonest Letter\n");
		printf("4. Decode\n");
		printf("5. Memory Report\n");
		printf("0. Exit\n");
		num = scanIntSafe();
		switch (num)
		{
			case 1: ex1(); break;
			case 2: ex2(); break;
			case 3: ex3(); break;
			case 4: ex4(); break;
			case 5: ex5(); break;
			case 0: break; // Exit
			default: printf("An invalid input was entered\n"); break;
		}
	} while (num != 0);
}

void ex1() 
{
	string str, *result;
	char c;
	int size, i = 0;

	printf("please enter a string\n");
	str = readString();
	printf("please enter a letter\n");
	scanf_s("%c%*c", &c, 1);
	result = split(str, c, &size);
	for (; i < size; i++) 
	{
		printf("%s\n", result[i]);
		free(result[i]);
	}

	free(str);
	free(result);
}

void ex2()
{
	string str, result;

	printf("please enter a string with spaces and digits\n");
	str = readString();
	result = clearDigitsAndSpaces(str);
	printf("The string without spaces and digits:\n");
	printf("%s\n", result);
	free(str);
	free(result);
}

void ex3() 
{
	string fileName = "input.txt";
	char mostCommonChar;

	createFile(fileName);

	mostCommonChar = commonestLetter(fileName);

	if (mostCommonChar != 0) 
	{
		printf("Most common character is: %c\n", mostCommonChar);
	}
	else 
	{
		printf("No character could be found.\n");
	}
}

void ex4()
{
	int size;
	string str = "Btwlzx Dqqes Eq|pj2 Tjhvqujs Iqoqjy bpg Eqfxtx Xcwwtt";
	string newStr;

	size = (int)strlen(str);
	newStr = (string)malloc(sizeof(char)*(size + 1));
	strcpy(newStr, str);
	decode(newStr);
	printf("%s\n", newStr);
	free(newStr);
}

void ex5()
{
	string str, filename = "memoryReport.txt";
	printf("please enter a valid declaration line\n");
	str = readString();
	memoryReport(str, filename);
	printFromFile(filename);
	free(str);
}

// copy requested words into result string
// Words are separated by one or more spaces. We collect
// words starting with the specified char 'letter'.
string* split(string str, char letter, int *size) 
{
	int i = 0;
	string word, *result;
	*size = (int)strlen(str) / 2;
	result = (string*)malloc(*size * sizeof(string));
	
	// Make sure input char is lowercased
	letter = tolower(letter);
	
	// Split by space character
	word = strtok(str, " ");
	while (word)
	{
		if (tolower(word[0]) == letter)
		{
			result[i] = (string)malloc(sizeof(char)*(strlen(word) + 1));
			strcpy(result[i++], word);

		}
		word = strtok(NULL, " ");
	}

	result = (string*)realloc(result, i * sizeof(string));
	*size = i;
	return result;
}

// creates new string without spaces and digits
string clearDigitsAndSpaces(string str) 
{
	int i = 0, j = 0, size;
	string result;
	size = (int)strlen(str);
	result = (string)malloc(size * sizeof(char));
	while (i < size) 
	{
		if ((!isspace(str[i])) && (!isdigit(str[i]))) 
		{
			result[j++] = str[i];
		}
		i++;
	}

	// avoids redundant use of realloc
	if (j + 1 != size) 
	{ 
		result = (string)realloc(result, sizeof(char)*(j + 1));
	}

	result[j] = '\0';
	return result;
}

// Creates a new file and fill it with user input from console
void createFile(string fileName) 
{
	FILE *f;
	char c;

	f = fopen(fileName, "w");
	if (f) 
	{
		printf("Enter sequence of characters or ';' to stop:\n");
		while ((c = getchar()) != ';') 
		{
			fputc(c, f);
		}

		fclose(f);
	}
	else 
	{
		printf("Cannot create file: %s\n", fileName);
	}
}

// counter's array which keeps the mostCommonChar while running through the file
char commonestLetter(string fileName) 
{ 
	FILE *f;
	int charCounts[26] = { 0 }, i, maxCharCount = 0;
	char mostCommonChar = 0, c;

	f = fopen(fileName, "r");
	if (f) 
	{
		while ((c = fgetc(f)) != EOF) 
		{
			if (isalpha(c)) 
			{
				charCounts[toupper(c) - 'A']++;
			}
		}

		maxCharCount = charCounts[25];
		mostCommonChar = 'Z';
		for (i = 24; i >= 0; i--) 
		{
			if (charCounts[i] > maxCharCount) 
			{
				maxCharCount = charCounts[i];
				mostCommonChar = i + 'A';
			}
		}

		fclose(f);
	}

	return (maxCharCount == 0) ? 0 : mostCommonChar;
}

// zeroes the counter every space, then decoding by subtracting the counter in every character by method.
void decode(string str) 
{
	int count = 1, i, strsize;
	strsize = (int)strlen(str);
	for (i = 0; i < strsize; i++) 
	{
		if (str[i] == ' ')
		{
			count = 0;
		}

		str[i] = str[i] - count;
		count++;
	}
}

// Replaces any whitespace characters sequence (2+) with a single space char.
// for cases like "long     long" -> will be replace to be common "long long"
void spaceReplacer(string *declaration) 
{
	int i = 0, j = 0, size;
	size = (int)strlen(*declaration);
	for (; i < size; i++) 
	{
		while (isspace((*declaration)[i]) && isspace((*declaration)[i + 1])) 
		{
			i++;

			// Make sure it is a space and not tab or new line.
			(*declaration)[i] = ' ';
		}

		(*declaration)[j++] = (*declaration)[i];
	}

	(*declaration)[j++] = '\0';
	if (j != size + 1) 
	{
		*declaration = (string)realloc(*declaration, sizeof(char)*j);
	}
}

void memoryReport(string declaration, string fileName)
{
	int sizeOfType, pSize, bytesSize, i = 0, j = 0, copyWordLen;
	string word, copyWord, variableName;
	string numOfArray;
	FILE* f;

	f = fopen(fileName, "w");
	if (!f) 
	{
		printf("Unable to create file");
		return;
	}

	// Replace any 2+ whitespace chars in a row, with a single space,
	// for successfully comparison to "long long"
	spaceReplacer(&declaration);
	
	// begin of types comparison , skip the type value characters per each type, saves size of type itself and its pointer size.
	if (startWith("long long", declaration)) 
	{
		sizeOfType = sizeof(long long);
		delete(&declaration, 10);
		pSize = sizeof(long long*);
	}
	else if (startWith("long", declaration)) 
	{
		sizeOfType = sizeof(long);
		delete(&declaration, 5);
		pSize = sizeof(long*);
	}
	else if (startWith("double", declaration)) 
	{
		sizeOfType = sizeof(double);
		delete(&declaration, 7);
		pSize = sizeof(double*);
	}
	else if (startWith("float", declaration)) 
	{
		sizeOfType = sizeof(float);
		delete(&declaration, 6);
		pSize = sizeof(float*);
	}
	else if (startWith("short", declaration)) 
	{
		sizeOfType = sizeof(short);
		delete(&declaration, 6);
		pSize = sizeof(short*);
	}
	else if (startWith("int", declaration)) 
	{
		sizeOfType = sizeof(int);
		delete(&declaration, 4);
		pSize = sizeof(int*);
	}
	else if (startWith("char", declaration)) 
	{
		sizeOfType = sizeof(char);
		delete(&declaration, 5);
		pSize = sizeof(char*);
	}
	else 
	{
		printf("invalid declaration input\n");
		fclose(f);
		return;
	}

	// Use strtok to split variables by comma character.
	word = strtok(declaration, ",");
	while (word) {
		// Work on a copy, for modifications
		copyWord = (string)malloc(sizeof(char)*(strlen(word) + 1));
		strcpy(copyWord, word);

		// Remove opening whitespace characters
		trim(&copyWord);

		// for pointer variables
		if (copyWord[0] == '*') 
		{
			// Skip pointer sign, so we can get the variable name alone.
			delete(&copyWord, 1);
			trim(&copyWord);
			bytesSize = pSize;
		}
		else 
		{
			bytesSize = sizeOfType;
		}

		// Get up to date length. (we might have been removed spaces or pointer sign)
		copyWordLen = (int)strlen(copyWord);
		variableName = (string)malloc(sizeof(char)*(copyWordLen + 1));
		
		// Get the name of the variable. (without array / end of input characters.)
		for (i = 0; i < copyWordLen; i++)
		{
			if ((copyWord[i] == '[') || (copyWord[i] == ';') || (isspace(copyWord[i]))) 
			{
				break;
			}
			else 
			{
				variableName[i] = copyWord[i];
			}
		}

		variableName[i] = '\0';

		if (copyWordLen != i) 
		{
			variableName = (string)realloc(variableName, sizeof(char) * (i + 1));
		}

		// In case current variable is an array, get its size
		if (copyWord[i] == '[') 
		{
			numOfArray = (string)malloc(sizeof(char)*(copyWordLen + 1));
			j = 0;
			
			// Run until we reach end of array length
			while (copyWord[++i] != ']') 
			{
				if (isdigit(copyWord[i])) 
				{
					numOfArray[j++] = copyWord[i];
				}
			}
			numOfArray[j++] = '\0';

			if (j != (copyWordLen + 1)) 
			{
				numOfArray = (string)realloc(numOfArray, sizeof(char) * j);
			}

			bytesSize = sizeOfType*strtol(numOfArray, (char**)NULL, 10);
			free(numOfArray);
		}

		fprintf(f, "%s requires %d byte%s\n", variableName, bytesSize, (bytesSize > 1 ? "s" : ""));
		free(copyWord);
		free(variableName);

		word = strtok(NULL, ",");
	}

	fclose(f);
}
void printFromFile(string fileName)
{
	FILE* f;
	char line[1024];
	f = fopen(fileName, "r");

	if (!f) 
	{
		printf("Cannot open file: %s\n", fileName);
	}
	else 
	{
		while (fgets(line, 1024, f)) 
		{
			printf("%s", line);
		}

		fclose(f);
	}
}

