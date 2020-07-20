/**
 * Assigned By
 * -----------
 * Name: Haim Adrian
 * Group: 61108-61
 *
 * Name: Shlomki Amir
 * Group: 61108-61
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "safe.h"
#include "stringutil.h"

#define EMP_NAME_SIZE 15
#define SIZE 8

typedef struct employee {
    int code;
    char name[EMP_NAME_SIZE];
    float salary;
} Employee;

typedef unsigned char uchar;

typedef char board[SIZE][SIZE];

void ex1();

void ex2();

void ex3();

int main() {
    uint ex;
    void (*exs[3])() = {ex1, ex2, ex3};

    // Use current time as seed for random generator
    srand((unsigned int)time(NULL));

    do {
        printf("Choose an exercise or 0 to exit:\n");
        printf("1. Employees\n");
        printf("2. Coding to short string\n");
        printf("3. Binary board\n");
        printf("0. Exit\n");
        ex = scanIntSafe((int *) NULL);

        // 0 = Exit
        if (ex != 0) {
            if (ex < 1 || ex > 3) {
                printf("Unknown action: %d. Please try again.\n", ex);
            } else {
                // Execute the requested exercise
                (*exs[ex - 1])();
            }
        }

        printf("\n\n");
    } while (ex != 0);

    return 0;
}

/////////////////////////////////// EX1 ///////////////////////////////////
Employee *readEmployeeFromConsole(int *eofEncountered) {
    Employee *emp = (Employee *) calloc(1, sizeof(Employee));
    string name = NULL;

    emp->code = -1;
    emp->salary = -1;

    printf("Enter employee details: Code (int), Name (14 characters), Salary (float)\n");
    printf("Code:");
    emp->code = scanIntSafe(eofEncountered);

    if (!*eofEncountered) {
        printf("Name:");
        name = readStringWithCustomLength(EMP_NAME_SIZE - 1, eofEncountered);
        if (name != NULL) {
            strcpy_s(emp->name, EMP_NAME_SIZE, name);
            free(name);
        }
    }

    if (!*eofEncountered) {
        printf("Salary:");
        emp->salary = scanFloatSafe(eofEncountered);
    }

    // If there was EOF in the middle of the input
    if (*eofEncountered && (emp->salary == -1)) {
        free(emp);
        emp = NULL;
    }

    return emp;
}

long createEmployeesFile(string fileName) {
    FILE *f;
    Employee *emp;
    long empCount = 0;
    int eofEncountered = 0;

    if (fopen_s(&f, fileName, "wb")) {
        printf("Failed to create file: %s.\n", fileName);
    } else {
        printf("Enter employees, one by another, or EOF to stop. (EOF = Ctrl+Z on Win and Ctrl+D on Unix)\n");
        while ((emp = readEmployeeFromConsole(&eofEncountered)) != NULL) {
            if (!fwrite(emp, sizeof(Employee), 1, f)) {
                printf("Failed to write to file. Employee: %d \"%s\" %.2f\n", emp->code, emp->name, emp->salary);
            } else {
                empCount++;
                free(emp);
            }

            // In order to support entering Ctrl+Z after a valid salary input,
            // at the same line, we break after writing the record to the file.
            if (eofEncountered) {
                break;
            }
        }

        fclose(f);
        printf("Input has ended. Output file: %s\n", fileName);
    }

    return empCount;
}

void updateSalary(string fileName, float threshold) {
    FILE *f, *tempFile;
    Employee emp;
    string tempFileName = "tempFile.dat";

    if (fopen_s(&f, fileName, "rb")) {
        printf("Cannot open file: %s\n", fileName);
    } else if (fopen_s(&tempFile, tempFileName, "wb")) {
        printf("Cannot create temp file for update: tempFile.dat\n");
        fclose(f);
    } else {
        fread(&emp, sizeof(Employee), 1, f);
        while (!feof(f)) {
            printf("Enter value to raise salary for: %d \"%s\" %.2f\n", emp.code, emp.name, emp.salary);
            while (!addFloatSafe(emp.salary, scanFloatSafe((int *)NULL), &(emp.salary)));

            if (emp.salary <= threshold) {
                fwrite(&emp, sizeof(Employee), 1, tempFile);
            } else {
                printf("Salary of %s exceeds the threshold (%.3f) by %.3f. Deleting record.\n", emp.name, threshold, emp.salary - threshold);
			}

            // next
            fread(&emp, sizeof(Employee), 1, f);
        }

        // Close files so we can rename
        fclose(f);
        fclose(tempFile);

        if (remove(fileName)) {
            printf("Failed to delete old file: %s\n", fileName);
        } else if (rename(tempFileName, fileName)) {
            printf("Failed to rename file: %s to: %s", tempFileName, fileName);
        } else {
            printf("Update completed successfully.\n");
        }
    }
}

void printEmployeesFile(string fileName) {
    FILE *f;
    Employee emp;

    if (fopen_s(&f, fileName, "rb")) {
        printf("Failed to open file: %s\n", fileName);
    } else {
        fread(&emp, sizeof(Employee), 1, f);
        while (!feof(f)) {
            printf("%d \"%s\" %.2f\n", emp.code, emp.name, emp.salary);
            fread(&emp, sizeof(Employee), 1, f);
        }

        fclose(f);
    }
}

void ex1() {
    string empFile = "Employees.dat";
    float threshold;
    long empCount;

    empCount = createEmployeesFile(empFile);
    if (empCount > 0) {
        printf("\nEmployees file content before update is:\n");
        printEmployeesFile(empFile);

        printf("\nEnter salary threshold for update:\n");
        threshold = scanFloatSafe((int *)NULL);

        updateSalary(empFile, threshold);

        printf("\nEmployees file content after update is:\n");
        printEmployeesFile(empFile);
    } else {
        printf("No employee has been entered.\n");
    }
}


/////////////////////////////////// EX2 ///////////////////////////////////
uchar * codingToShortString(string digStr) {
    uchar * shortDigStr = NULL;
    int digStrLen = (int)strlen(digStr), i, j, shortDigStrLen;

    if (digStrLen > 0) {
        shortDigStrLen = (digStrLen / 2) + (digStrLen % 2);
        shortDigStr = (uchar *)calloc(shortDigStrLen + 1, sizeof(uchar)); // 1 for \0

        for (i = digStrLen - 1, j = shortDigStrLen - 1; i >= 0; i -= 2, j--) {
            // Right 4 bits of a byte.
            shortDigStr[j] = (uchar)(digStr[i] - '0');

            // If number got odd amount of digits, then i might be 0 here.
            if (i > 0) {
                // Left 4 bits of a byte.
                shortDigStr[j] += (((uchar)(digStr[i-1] - '0')) << (uchar)4);
            }
        }
    }

    return shortDigStr;
}

void displayShortStringAlgorithm(uchar * shortDigStr, void (*printFunc)(uchar)) {
    int i, shortDigStrLen = (int)strlen((string)shortDigStr);
    uchar leftBits, rightBits;

    // Zero is \0, so we take it into account.
    if (shortDigStrLen == 0) {
        (*printFunc)(0);
    } else {
        for (i = 0; i < shortDigStrLen; i++) {
            leftBits = shortDigStr[i] >> (uchar) 4;
            rightBits = shortDigStr[i] & (uchar) 0b00001111;

            if (!leftBits && (i == 0)) { // 0X -> X
                (*printFunc)(rightBits);
            } else {
                (*printFunc)(leftBits);
                (*printFunc)(rightBits);
            }
        }
    }
}

void print4Bits(uchar num) {
    int i;

    printf(" ");
    for (i = 3; i >= 0; i--) {
        printf("%u", (uchar)(num >> (uchar)i) & (uchar)1);
    }
}

void printDecimal(uchar num) {
    printf("%u", num);
}

void displayShortString(uchar * shortDigStr) {
    displayShortStringAlgorithm(shortDigStr, printDecimal);

    printf(" =");

    displayShortStringAlgorithm(shortDigStr, print4Bits);
}

void ex2() {
    string digStr;
    uchar * shortDigStr;

    printf("Enter an unsigned number:\n");
    digStr = readString((int*)NULL);
    if (digStr) {
        if ((strlen(digStr) > 1) && ((digStr[0] == '0') || (digStr[0] == '-'))) {
            printf("\nInput number is illegal. A number cannot begin with 0 except 0 itself, and it must be unsigned. Input was: %s\n", digStr);
        } else {
            shortDigStr = codingToShortString(digStr);
            if (shortDigStr) {
                printf("\nDisplay short string as decimal and binary presentations:\n");
                displayShortString(shortDigStr);
                free(shortDigStr);
            }
        }

        free(digStr);
    } else {
        printf("There is no input string.\n");
    }
}


/////////////////////////////////// EX3 ///////////////////////////////////
void fillBoard(board binBoard) {
    int i, j;

    printf("\nBinary board:\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            binBoard[i][j] = (char)('0' + (rand() % 2));
            printf("%2c", binBoard[i][j]);
        }
        printf("\n");
    }
}

unsigned long long boardToBits(board binBoard) {
    unsigned long long result = 0, temp = 0;
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            temp = (uchar)(binBoard[i][j] - '0') & (uchar)1;
            result |= (temp << (uchar)((SIZE - i)*SIZE - 1 - j));
        }
    }

    return result;
}

void displayBinaryBoard(unsigned long long encodedBinBoard) {
    int i, j;

    printf("\nEncoded binary board:\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf("%2c", '0' + (uchar)((encodedBinBoard >> (uchar)((SIZE - i)*SIZE - 1 - j)) & (uchar)1));
        }
        printf("\n");
    }
}

void print64Bits(unsigned long long num) {
    int i;

    printf("%llu(10) = ", num);
    for (i = 63; i >= 0; i--) {
        printf("%u", (uchar)(num >> (uchar)i) & (uchar)1);
    }
    printf("(2)");
}

void ex3() {
    board binBoard;
    unsigned long long encodedBoard;

    fillBoard(binBoard);

    encodedBoard = boardToBits(binBoard);
    printf("\nEncoded board bits: ");
    print64Bits(encodedBoard);
    printf("\n");

    displayBinaryBoard(encodedBoard);
}