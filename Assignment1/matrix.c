//
// Created by Haim Adrian
//

#include "matrix.h"
#include "safe.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))

matrix createMatrix(int rows, int cols) {
    int row;
    matrix mat = NULL;

    if ((rows <= 0) || (cols <= 0)) {
        printf("Cannot create a matrix with illegal amount of rows or columns. count must be bigger than zero. [rows=%d, cols=%d]\n", rows, cols);
    } else {
        // Allocate first column in a matrix, which is a column of pointers, where
        // each element points to an array that holds the columns of that row.
        mat = (matrix)calloc(rows, sizeof(matrixType *));
        if (!mat) {
            printf("Failed to create matrix. Insufficient memory.\n");
            return NULL;
        }

        for (row = 0; row < rows; row++) {
            // Allocate the columns (array) for current row.
            *(mat + row) = (matrixType *)calloc(cols, sizeof(matrixType)); // matrix[row] = malloc
            if (!(mat[row])) {
                printf("Failed to create matrix. Insufficient memory.\n");

                // Free previously allocated rows.
                freeMatrix(&mat, rows);

                return NULL;
            }
        }
    }

    return mat;
}

void freeMatrix(matrix *mat, int rows) {
    int row;

    // Free a matrix in case it refers to something other than NULL only.
    if (*mat) {
        for (row = 0; row < rows; row++) {
            if ((*mat)[row]) {
                free((*mat)[row]);
            }
        }

        free(*mat);
        *mat = NULL;
    }
}

matrix readMatrix(int rows, int cols) {
	int row, col;
	matrix mat;

	// Allocate first column in a matrix, which is a column of pointers, where
	// each element points to an array that holds the columns of that row.
    mat = createMatrix(rows, cols);

    if (mat) {
        printf("Enter matrix[%d][%d], row by row:\n", rows, cols);
        for (row = 0; row < rows; row++) {
            for (col = 0; col < cols; col++) {
                // Scan an integer into current cell.
                // Note that: *(*(matrix + row) + col) is the same as (matrix[row][col])
                *(*(mat + row) + col) = scanIntSafe();
            }
        }
    }

	return mat;
}

matrix readCustomSizeMatrix(int *rows, int *cols) {
	printf("Enter amount of rows and columns, one by one:\n");
	*rows = scanIntSafe();
    *cols = scanIntSafe();
	return readMatrix(*rows, *cols);
}

// Utility function to find the longest number in a matrix. We use this for indentation when printing a matrix.
int findMaxNumberOfDigits(matrix mat, int rows, int cols) {
    int row, col, num, nDigits = 1;
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            num = *(*(mat + row) + col); // matrix[row][col]
            nDigits = max(nDigits, (int)floor(log10(abs(max(1, num)))) + 1);
        }
    }

    return nDigits;
}

void printMatrix(matrix mat, int rows, int cols) {
    int row, col, longestNumberDigits = findMaxNumberOfDigits(mat, rows, cols) + 1;

    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            printf("%-*d", longestNumberDigits, *(*(mat + row) + col)); // matrix[row][col]
        }

        printf("\n");
    }
}

void printRow(matrix mat, int row, int rows, int cols, int cellWidth) {
    int col;

    if (row < rows) {
        for (col = 0; col < cols; col++) {
            printf("%-*d", cellWidth, *(*(mat + row) + col)); // mat[row][col]
        }
    } else {
		printf("%*c", cellWidth * cols, ' ');
    }
}

void printMatricesExpression(matrix mat1, int rows1, int cols1, matrix mat2, int rows2, int cols2, matrix mat3, int rows3, int cols3, char operator) {
    int row, maxRows = max(max(rows1, rows2), rows3), midRow = maxRows / 2, longestNumberDigits1, longestNumberDigits2, longestNumberDigits3;

    longestNumberDigits1 = findMaxNumberOfDigits(mat1, rows1, cols1) + 1;
    longestNumberDigits2 = findMaxNumberOfDigits(mat2, rows2, cols2) + 1;
    longestNumberDigits3 = findMaxNumberOfDigits(mat3, rows3, cols3) + 1;

    for (row = 0; row < maxRows; row++) {
        printRow(mat1, row, rows1, cols1, longestNumberDigits1);

        printf("  %c   ", ((row == midRow) ? operator : ' '));

        printRow(mat2, row, rows2, cols2, longestNumberDigits2);

        printf("  %c   ", ((row == midRow) ? '=' : ' '));

        printRow(mat3, row, rows3, cols3, longestNumberDigits3);

        printf("\n");
    }
}