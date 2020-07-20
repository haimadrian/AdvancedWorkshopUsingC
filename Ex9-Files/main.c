//
// Created by Haim Adrian
//

#include <stdio.h>
#include <string.h>

#define ID_LENGTH 9
#define NAME_LENGTH 30
#define SUM_LENGTH 8

const int ID_LENGTH_STR = ID_LENGTH + 1; // 9 + 1 for null terminator char
const int LINE_LENGTH = ID_LENGTH + NAME_LENGTH + SUM_LENGTH + 2; // 1 for \n and another 1 for \0
const int LINE_LENGTH_BIN = ID_LENGTH + NAME_LENGTH + sizeof(double) + 1; // only 1 for \n, no \0 in binary

void printFile(char * fileName);
void printFileBin(char * fileName);

// Lookup for line number of a record with the specified id
long findRecord(char * id, char * fileName);
int updatePurchaseTxt(char * id, double sum, char * fileName);
long findRecordBin(char * id, char * fileName);
int updatePurchaseBin(char * id, double sum, char * fileName);

void ex1();
void ex2();

int main() {
    ex1();
    ex2();

    return 0;
}

void ex1() {
    FILE *f;
    int err;
    char * fileName = "customers.txt";

    printf("ex1: Text file\n");

    if ((err = fopen_s(&f, fileName, "w"))) {
        printf("Failed to create program DB. Error: %d", err);
    } else {
        fprintf(f, "%*s%*s%*.2lf\n", ID_LENGTH, "123456789", NAME_LENGTH, "Eyal", SUM_LENGTH, 0.0);
        fprintf(f, "%*s%*s%*.2lf\n", ID_LENGTH, "123456788", NAME_LENGTH, "Shlomke", SUM_LENGTH, 1120.21);
        fprintf(f, "%*s%*s%*.2lf\n", ID_LENGTH, "012345678", NAME_LENGTH, "Yakir", SUM_LENGTH, 43.8);
        fprintf(f, "%*s%*s%*.2lf\n", ID_LENGTH, "123456787", NAME_LENGTH, "Tom", SUM_LENGTH, 600.35);
        fprintf(f, "%*s%*s%*.2lf\n", ID_LENGTH, "0623512", NAME_LENGTH, "Voldemort", SUM_LENGTH, -100.0);
        fclose(f);

        updatePurchaseTxt("123456789", 1.2, fileName);
        updatePurchaseTxt("111111111", 1.2, fileName);
        updatePurchaseTxt("123456788", 50.01, fileName);
        updatePurchaseTxt("0623512", 65, fileName);

        printf("\nFile content is:\n");
        printFile(fileName);
    }
}

void fixIdIfNeeded(char * id, char fixedId[]) {
    int i, specifiedIdLength;

    specifiedIdLength = (int)strlen(id);

    for (i = 0; i < specifiedIdLength; ++i) {
        fixedId[i + (ID_LENGTH - specifiedIdLength)] = id[i];
    }

    // Fix identities with less than 9 digits
    if (specifiedIdLength < ID_LENGTH) {
        for (i = specifiedIdLength; i < ID_LENGTH; i++) {
            fixedId[i - specifiedIdLength] = ' ';
        }
    }

    fixedId[ID_LENGTH] = '\0';
}

long findRecord(char * id, char * fileName) {
    FILE *f;
    long lineNumber = -1;
    int err, isFound = 0;
    char currId[ID_LENGTH + 1];
    char fixedId[ID_LENGTH + 1];

    fixIdIfNeeded(id, fixedId);

    if ((err = fopen_s(&f, fileName, "r"))) {
        printf("Failed to open file: %s. Error: %d\n", fileName, err);
    } else {
        while (!isFound && (fgets(currId, ID_LENGTH_STR, f) != NULL)) {
            lineNumber++;

            if (_stricmp(fixedId, currId) == 0) {
                isFound = 1; // Found
            } else {
                // Jump to next line
                fseek(f, NAME_LENGTH + SUM_LENGTH + 2, SEEK_CUR); // + 2 is for '\n'
            }
        }

        fclose(f);

        if (!isFound) {
            lineNumber = -1;
        }
    }

    return lineNumber;
}

int updatePurchaseTxt(char * id, double sum, char * fileName) {
    FILE *f;
    double currSum;
    long lineNumber;
    int result, err;

    lineNumber = findRecord(id, fileName);
    if (lineNumber < 0) {
        result = 0;
        printf("ID: %s does not exist in customers file.\n", id);
    } else if ((err = fopen_s(&f, fileName, "r+"))) {
        result = 0;
        printf("Failed to open file: %s. Error: %d\n", fileName, err);
    } else {
        printf("Record with ID %s has been found. Updating...\n", id);

        // Jump to the line we found the id at, to the SUM column.
        fseek(f, (lineNumber * LINE_LENGTH) + ID_LENGTH + NAME_LENGTH, SEEK_SET);

        fscanf_s(f, "%lf", &currSum);

        // Reset back to SUM starting point for updating it with new value.
        fseek(f, -SUM_LENGTH, SEEK_CUR);
        fprintf(f, "%*.2lf", SUM_LENGTH, sum + currSum);

        result = 1;
        fclose(f);
    }

    return result;
}

void printFile(char * fileName) {
    FILE *f;
    int err;
    char line[ID_LENGTH + NAME_LENGTH + SUM_LENGTH + 2];

    if ((err = fopen_s(&f, fileName, "r"))) {
        printf("Failed to print file. Unable to open file: %s. Error: %d\n", fileName, err);
    } else {
        while (fgets(line, LINE_LENGTH, f) != NULL) {
            printf("%s", line);
        }
    }
}

void printFileBin(char * fileName) {
    FILE *f;
    double sum;
    int err;
    char line[ID_LENGTH + NAME_LENGTH + 1]; // 1 for \0

    if ((err = fopen_s(&f, fileName, "rb"))) {
        printf("Failed to print file. Unable to open file: %s. Error: %d\n", fileName, err);
    } else {
        while (fgets(line, ID_LENGTH + NAME_LENGTH + 1, f) != NULL) {
            line[ID_LENGTH + NAME_LENGTH] = '\0';
            printf("%s", line);

            fread(&sum, sizeof(double), 1, f);
            printf("%*.2lf\n", SUM_LENGTH, sum);
            fseek(f, 1, SEEK_CUR); // Skip line separator.
        }
    }
}

void ex2() {
    FILE *f;
    double sum;
    int err;
    char * fileName = "customers.data";
    char line[ID_LENGTH + NAME_LENGTH + SUM_LENGTH + 2];

    printf("\n\nex2: Binary file\n");
    if ((err = fopen_s(&f, fileName, "wb"))) {
        printf("Failed to create program DB. Error: %d", err);
    } else {
        sprintf_s(line, LINE_LENGTH, "%*s%*s", ID_LENGTH, "123456789", NAME_LENGTH, "Eyal");
        fwrite(line, sizeof(char), ID_LENGTH + NAME_LENGTH, f);
        sum = 0.0;
        fwrite(&sum, sizeof(double), 1, f);
        fwrite("\n", sizeof(char), 1, f);
        sprintf_s(line, LINE_LENGTH, "%*s%*s", ID_LENGTH, "123456788", NAME_LENGTH, "Shlomke");
        fwrite(line, sizeof(char), ID_LENGTH + NAME_LENGTH, f);
        sum = 1120.21;
        fwrite(&sum, sizeof(double), 1, f);
        fwrite("\n", sizeof(char), 1, f);
        sprintf_s(line, LINE_LENGTH, "%*s%*s", ID_LENGTH, "012345678", NAME_LENGTH, "Yakir");
        fwrite(line, sizeof(char), ID_LENGTH + NAME_LENGTH, f);
        sum = 43.8;
        fwrite(&sum, sizeof(double), 1, f);
        fwrite("\n", sizeof(char), 1, f);
        sprintf_s(line, LINE_LENGTH, "%*s%*s", ID_LENGTH, "123456787", NAME_LENGTH, "Tom");
        fwrite(line, sizeof(char), ID_LENGTH + NAME_LENGTH, f);
        sum = 600.35;
        fwrite(&sum, sizeof(double), 1, f);
        fwrite("\n", sizeof(char), 1, f);
        sprintf_s(line, LINE_LENGTH, "%*s%*s", ID_LENGTH, "0623512", NAME_LENGTH, "Voldemort");
        fwrite(line, sizeof(char), ID_LENGTH + NAME_LENGTH, f);
        sum = -100.0;
        fwrite(&sum, sizeof(double), 1, f);
        fwrite("\n", sizeof(char), 1, f);
        fclose(f);

        updatePurchaseBin("123456789", 1.2, fileName);
        updatePurchaseBin("111111111", 1.2, fileName);
        updatePurchaseBin("123456788", 50.01, fileName);
        updatePurchaseBin("0623512", 65, fileName);

        printf("\nFile content is:\n");
        printFileBin(fileName);
    }
}

long findRecordBin(char * id, char * fileName) {
    FILE *f;
    long lineNumber = -1;
    int err, isFound = 0;
    char currId[ID_LENGTH + 1];
    char fixedId[ID_LENGTH + 1];

    fixIdIfNeeded(id, fixedId);

    if ((err = fopen_s(&f, fileName, "rb"))) {
        printf("Failed to open file: %s. Error: %d\n", fileName, err);
    } else {
        while (!isFound && (fread(currId, sizeof(char), ID_LENGTH, f) > 0)) {
            currId[ID_LENGTH] = '\0'; // fread does not set null terminator character, so set it manually.
            lineNumber++;

            //printf("currId: %s\n", currId);
            if (_stricmp(fixedId, currId) == 0) {
                isFound = 1; // Found
            } else {
                // Jump to next line
                fseek(f, NAME_LENGTH + sizeof(double) + 1, SEEK_CUR); // + 1 is for '\n'
            }
        }

        fclose(f);

        if (!isFound) {
            lineNumber = -1;
        }
    }

    return lineNumber;
}

int updatePurchaseBin(char * id, double sum, char * fileName) {
    FILE *f;
    double currSum;
    long lineNumber;
    int result, err;

    lineNumber = findRecordBin(id, fileName);
    if (lineNumber < 0) {
        result = 0;
        printf("ID: %s does not exist in customers file.\n", id);
    } else if ((err = fopen_s(&f, fileName, "rb+"))) {
        result = 0;
        printf("Failed to open file: %s. Error: %d\n", fileName, err);
    } else {
        printf("Record with ID %s has been found. Updating...\n", id);

        // Jump to the line we found the id at, to the SUM column.
        fseek(f, (lineNumber * LINE_LENGTH_BIN) + ID_LENGTH + NAME_LENGTH, SEEK_SET);

        fread(&currSum, sizeof(double), 1, f);

        //printf("sum=%lf, currSum=%lf\n", sum, currSum);
        // Reset back to SUM starting point for updating it with new value.
        fseek(f, -(int)sizeof(double), SEEK_CUR);
        sum += currSum;
        fwrite(&sum, sizeof(double), 1, f);

        result = 1;
        fclose(f);
    }

    return result;
}