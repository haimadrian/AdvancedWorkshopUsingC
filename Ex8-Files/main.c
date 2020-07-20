//
// Created by Haim Adrian
//

#include <stdio.h>
#include <ctype.h>

#define CHARS_AMOUNT 26

int countLines(char * fileName);
void letterFrequency(char * fileName, int counts[]);
void removeSpaces(char * infile, char * outfile);

void printArray(int * arr, int length);

int main() {
    char * fileName = "tempFile.txt";
    FILE *f;
    int err, letterFreq[CHARS_AMOUNT];

    if ((err = fopen_s(&f, fileName, "w"))) {
        printf("Failed to create file for program. Error: %d\n", err);
    } else {
        fprintf(f, "First line.\n");
        fprintf(f, "Second line.\n");
        fprintf(f, "Third line.\n");
        fprintf(f, "Fourth line.\n");

        fclose(f);

        printf("Lines count: %d\n", countLines(fileName));

        letterFrequency(fileName, letterFreq);
        printf("Letter frequency:\n");
        printArray(letterFreq, CHARS_AMOUNT);

        removeSpaces(fileName, "tempFileWithoutSpaces.txt");
    }

    return 0;
}

void resetArray(char arr[], int length) {
    int i;
    for (i = 0; i < length; ++i) {
        arr[i] = 0;
    }
}

void resetIntArray(int arr[], int length) {
    int i;
    for (i = 0; i < length; ++i) {
        arr[i] = 0;
    }
}

void printArray(int * arr, int length) {
    int i;

    if (!arr) {
        printf("NULL\n");
    } else if (length == 0) {
        printf("[]\n");
    } else {
        printf("[%d", arr[0]);
        for (i = 1; i < length; ++i) {
            printf(", %d", arr[i]);
        }
        printf("]\n");
    }
}

int countLines(char * fileName) {
    FILE *f;
    int err, lines = 0;
    char line[1024];

    resetArray(line, 1024);

    if ((err = fopen_s(&f, fileName, "r"))) {
        printf("Failed to open file. Error: %d\n", err);
    } else {
        while (fgets(line, 1024, f) != NULL) {
            printf("Line: %s", line);
            lines++;
        }

        fclose(f);
    }

    return lines;
}

void letterFrequency(char * fileName, int counts[CHARS_AMOUNT]) {
    FILE *f;
    int err, ch;

    resetIntArray(counts, CHARS_AMOUNT);

    if ((err = fopen_s(&f, fileName, "r"))) {
        printf("Failed to open file. Error: %d\n", err);
    } else {
        while ((ch = fgetc(f)) != EOF) {
            if (isalpha(ch)) {
                counts[toupper(ch) - 'A']++;
            }
        }

        fclose(f);
    }
}

void removeSpaces(char * infile, char * outfile) {
    FILE *inf, *outf;
    int err, ch;

    if ((err = fopen_s(&inf, infile, "r"))) {
        printf("Failed to open input file. Error:%d\n", err);
    } else if((err = fopen_s(&outf, outfile, "w"))) {
        printf("Failed to create output file. Error: %d\n", err);
        fclose(inf);
    } else {
        while ((ch = fgetc(inf)) != EOF) {
            if (ch != ' ') {
                fputc(ch, outf);
            }
        }

        fclose(inf);
        fclose(outf);
    }
}