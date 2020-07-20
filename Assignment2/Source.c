//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Default starting length for a dynamic string.
#define DEFAULT_STR_LEN 64
#define LETTERS 26
#define MAX_LINE_LEN 1024

typedef char* string;

void ex1();
void ex2();
void ex3();
void ex4();
void ex5();

void main() 
{
	int ex;
	void (*exs[5])() = { ex1, ex2, ex3, ex4, ex5 };

	do
	{
		printf("Choose an exercise or 0 to exit:\n");
		printf("1. Split\n");
		printf("2. Clear digits and spaces\n");
		printf("3. Most common letter in file\n");
		printf("4. Decode\n");
		printf("5. Memory Report\n");
		printf("0. Exit\n");
		scanf_s("%d", &ex);

		// Clear input buffer to avoid of infinite loop when user enters illegal input.
		while (getchar() != '\n'); 

		// 0 = Exit
		if (ex != 0)
		{
			if (ex < 1 || ex > 5) 
			{
				printf("Unknown action: %d. Please try again.\n", ex);
			}
			else 
			{
				// Execute the requested exercise
				(*exs[ex - 1])();
			}
		}

		printf("\n");
	} while (ex != 0);
}

//////////////////////////// Utility ////////////////////////////
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

//////////////////////////// EX1 ////////////////////////////
string * split(string str, char ch, int *resultLen)
{
	int inputLen = strlen(str), currLen;
	string * result = NULL, tempStr, currWord, nextTok, delimiter = " ";
	*resultLen = 0;
	
	if (inputLen > 0)
	{
		// Make sure input char is a lowercased letter
		ch = tolower(ch);

		// Worst case: every second char is a space
		result = (string*)calloc((inputLen / 2), sizeof(string));
		
		// Copy source string because strtok modifies the string
		tempStr = (string)calloc((inputLen + 1), sizeof(char));
		strcpy_s(tempStr, inputLen + 1, str);

		// Now loop over all words (splitted by delimiter (" "))
		currWord = strtok_s(tempStr, delimiter, &nextTok);
		while (currWord)
		{
			currLen = strlen(currWord);
			if ((currLen > 0) && (tolower(currWord[0]) == ch)) 
			{
				// strtok already put a \0 on currWord, so we don't need to repeat that.
				result[*resultLen] = (string)calloc((currLen + 1), sizeof(char));
				strcpy_s(result[*resultLen], currLen + 1, currWord);
				(*resultLen)++;
			}

			// Next word
			currWord = strtok_s(NULL, delimiter, &nextTok);
		}

		result = (string*)realloc(result, *resultLen * sizeof(string));
	}

	return result;
}

void ex1()
{
	int resultLen, i;
	string input, *result;
	char splitBy;

	printf("Enter a string to split:\n");
	input = readString();

	printf("Enter a character to collect words starts with that letter out of input string:\n");
	splitBy = getchar();

	result = split(input, splitBy, &resultLen);

	if (resultLen == 0) 
	{
		printf("No words starting with '%c' could be found.\n", splitBy);
	}
	else
	{
		printf("Collected words are:\n");
		for (i = 0; i < resultLen; i++)
		{
			printf("%s\n", result[i]);
			free(result[i]);
		}

		free(result);
	}

	free(input);
}

//////////////////////////// EX2 ////////////////////////////
string clearDigitsAndSpaces(string str)
{
	int i, k = 0, strLen = strlen(str);
	string out = (string)calloc(strLen, sizeof(char));

	// Run including strlen to copy \0 as well
	for (i = 0; i <= strLen; i++)
	{
		if (!(isdigit(str[i]) || (str[i] == ' ')))
		{
			out[k++] = str[i];
		}
	}

	out = (string)realloc(out, k * sizeof(char));

	return out;
}

void ex2()
{
	string input, output;

	printf("Enter a string to clear digits and spaces from:\n");
	input = readString();

	output = clearDigitsAndSpaces(input);
	printf("Output string is: %s\n", output);

	free(input);
	free(output);
}

//////////////////////////// EX3 ////////////////////////////
void createFile(string fileName)
{
	FILE *f;
	char wildcard = ';', ch;

	if (fopen_s(&f, fileName, "w"))
	{
		printf("Failed to create file: %s\n", fileName);
	}
	else
	{
		printf("Enter characters (including new lines) and '%c' followed by ENTER to stop:\n", wildcard);
		while ((ch = getchar()) != wildcard)
		{
			fputc(ch, f);
		}

		// Clear stdin
		while (getchar() != '\n');

		fclose(f);
	}
}

char commonestLetter(string fileName)
{
	int counters[LETTERS] = { 0 }, i, mostCommonCount = 0;
	FILE *f;
	char mostCommonLetter = '\0', currChar;

	if (fopen_s(&f, fileName, "r")) 
	{
		printf("Failed to open file: %s\n", fileName);
	}
	else
	{
		while ((currChar = fgetc(f)) != EOF) 
		{
			if (isalpha(currChar))
			{
				counters[toupper(currChar) - 'A']++;
			}
		}

		fclose(f);

		for (i = LETTERS - 1; i >= 0; i--) 
		{
			if (counters[i] > mostCommonCount)
			{
				mostCommonCount = counters[i];
				mostCommonLetter = 'A' + i;
			}
		}
	}

	return mostCommonLetter;
}

void ex3()
{
	string fileName = "input.txt";
	char mostCommonLetter = '\0';

	createFile(fileName);

	mostCommonLetter = commonestLetter(fileName);
	if (mostCommonLetter)
	{
		printf("Most common letter was: %c\n", mostCommonLetter);
	}
	else
	{
		printf("File was empty. There is no letter.\n");
	}
}

//////////////////////////// EX4 ////////////////////////////
void decode(string str)
{
	int i, currWordIndex = 0, strLen = strlen(str);

	for (i = 0; i < strLen; i++)
	{
		if (str[i] == ' ')
		{
			currWordIndex = 0;
		}
		else
		{
			str[i] = str[i] - ++currWordIndex;
		}
	}
}

void ex4()
{
	string encodedText = "Btwlzx Dqqes Eq|pj2 Tjhvqujs Iqoqjy bpg Eqfxtx Xcwwtt";
	int strLen = strlen(encodedText);
	string text = (string)calloc((strLen + 1), sizeof(char));
	strcpy_s(text, strLen + 1, encodedText);

	printf("Encoded string is: %s\n", text);

	decode(text);

	printf("Decoded text is: %s\n", text);

	free(text);
}

//////////////////////////// EX5 ////////////////////////////
void extractSizesByType(string * declarationParts, int *typeSize, int *ptrTypeSize)
{
	if (strcmp(declarationParts[0], "char") == 0)
	{
		*typeSize = sizeof(char);
		*ptrTypeSize = sizeof(char*);
	}
	else if (strcmp(declarationParts[0], "short") == 0)
	{
		*typeSize = sizeof(short);
		*ptrTypeSize = sizeof(short*);
	}
	else if (strcmp(declarationParts[0], "int") == 0)
	{
		*typeSize = sizeof(int);
		*ptrTypeSize = sizeof(int*);
	}
	else if ((strcmp(declarationParts[0], "long") == 0) && (strcmp(declarationParts[1], "long") == 0))
	{
		*typeSize = sizeof(long long);
		*ptrTypeSize = sizeof(long long*);
	}
	else if (strcmp(declarationParts[0], "long") == 0)
	{
		*typeSize = sizeof(long);
		*ptrTypeSize = sizeof(long*);
	}
	else if (strcmp(declarationParts[0], "float") == 0)
	{
		*typeSize = sizeof(float);
		*ptrTypeSize = sizeof(float*);
	}
	else if (strcmp(declarationParts[0], "double") == 0)
	{
		*typeSize = sizeof(double);
		*ptrTypeSize = sizeof(double*);
	}
	else
	{
		printf("Unknown type: %s. Assuming 0\n", declarationParts[0]);
		*typeSize = 0;
		*ptrTypeSize = 0;
	}
}

int skipWhitespaceChars(string str, int strLen, int *index)
{
	while ((*index < strLen) && isspace(str[*index]))
	{
		(*index)++;
	}

	return (*index < strLen);
}

// Splits a string by a specific char, or whitespace, or '*' / '[' / ']' / ';'
string * splitEx(string str, string by, int *size)
{
	int i, wordIndex, tempWordLen;
	string * result = (string *)malloc(strlen(str) * sizeof(string));
	string word, tempWord, specialWord = NULL, nextTok;
	*size = 0;

	tempWord = strtok_s(str, by, &nextTok);
	while (tempWord)
	{
		i = 0;
		tempWordLen = strlen(tempWord);
		while (i < tempWordLen)
		{
			if (skipWhitespaceChars(tempWord, tempWordLen, &i))
			{
				word = (string)calloc((tempWordLen + 1), sizeof(char));
				wordIndex = 0;
				while ((i < tempWordLen) && (!isspace(tempWord[i])))
				{
					// Pointer or array signs are on their own index.
					if ((tempWord[i] == '*') || (tempWord[i] == '[') || (tempWord[i] == ']') || (tempWord[i] == ';'))
					{
						specialWord = (string)calloc(2, sizeof(char));
						specialWord[0] = tempWord[i++];
						specialWord[1] = '\0';
						break;
					}

					word[wordIndex++] = tempWord[i++];
				}

				// If there is no word, there is just a special char. e.g. '*'
				if (wordIndex == 0)
				{
					free(word);
				}
				else
				{
					word[wordIndex++] = '\0';
					word = (string)realloc(word, wordIndex * sizeof(char));
					result[(*size)++] = word;
				}

				if (specialWord)
				{
					result[(*size)++] = specialWord;
					specialWord = NULL;
				}
			}
		}

		tempWord = strtok_s(NULL, by, &nextTok);
	}

	// Add a ";" as last word if there was no declaration line ending.
	if (strcmp(result[*size - 1], ";") != 0) 
	{
		result[(*size)] = (string)calloc(2, sizeof(char));
		result[(*size)][0] = ';';
		result[(*size)++][1] = '\0';
	}

	result = (string *)realloc(result, (*size) * sizeof(string));

	return result;
}

void memoryReport(string declaration, string fileName)
{
	string varName, *decParts;
	FILE *f;
	int typeSize, ptrTypeSize, varSize, currVarLen, decPartsSize, decLen = strlen(declaration), i;

	if (decLen > 0)
	{
		if (fopen_s(&f, fileName, "w"))
		{
			printf("Failed to create file: %s\n", fileName);
		}
		else
		{
			// Split declaration by comma, and by whitespace (space/tab/etc.)
			decParts = splitEx(declaration, ",", &decPartsSize);
			if (decPartsSize >= 2)
			{
				extractSizesByType(decParts, &typeSize, &ptrTypeSize);
				i = ((strcmp(decParts[0], "long") == 0) && (strcmp(decParts[1], "long") == 0)) ? 2 : 1;
				
				// Run over all parts except of last one which is a ";".
				for (; i < decPartsSize - 1; i++)
				{
					// Pointer
					if (strcmp(decParts[i], "*") == 0)
					{
						i++; // Get to the pointer name
						currVarLen = strlen(decParts[i]);
						varName = (string)calloc(currVarLen + 1, sizeof(char));
						strcpy_s(varName, currVarLen + 1, decParts[i]);
						varSize = ptrTypeSize;

						// Free the asterix
						free(decParts[i - 1]);
					}
					// Array
					else if (strcmp(decParts[i + 1], "[") == 0)
					{
						currVarLen = strlen(decParts[i]);
						varName = (string)calloc(currVarLen + 1, sizeof(char));
						strcpy_s(varName, currVarLen + 1, decParts[i]);

						// Get to the array size part
						i += 2;
						varSize = typeSize * strtol(decParts[i], (char**)NULL, 10);

						// Get to last part of array, so we will go to the next variable.
						i++;

						// Free array parts
						free(decParts[i - 1]);
						free(decParts[i - 2]);
						free(decParts[i - 3]);
					}
					// Regular variable
					else
					{
						currVarLen = strlen(decParts[i]);
						varName = (string)calloc(currVarLen + 1, sizeof(char));
						strcpy_s(varName, currVarLen + 1, decParts[i]);
						varSize = typeSize;
					}

					fprintf(f, "%s requires %d byte%s\n", varName, varSize, ((varSize > 1) ? "s" : ""));

					free(varName);
					free(decParts[i]);
				}
			}

			fclose(f);
			free(decParts);
		}
	}
}

void printFile(string fileName)
{
	FILE *f;
	char currLine[MAX_LINE_LEN];

	if (fopen_s(&f, fileName, "r"))
	{
		printf("Cannot open file: %s\n", fileName);
	}
	else
	{
		printf("Memory report is available at file: %s. Content is:\n", fileName);
		while (fgets(currLine, MAX_LINE_LEN, f))
		{
			printf("%s", currLine);
		}

		fclose(f);
	}
}

void ex5()
{
	string declaration, fileName = "MemoryReport.txt";

	printf("Enter declaration row:\n");
	declaration = readString();

	if (strlen(declaration) > 0)
	{
		memoryReport(declaration, fileName);
		printFile(fileName);
	}

	free(declaration);
}