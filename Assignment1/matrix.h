//
// Created by Haim Adrian
//

#ifndef UTILS_MATRIX_H
#define UTILS_MATRIX_H

typedef int matrixType;
typedef matrixType** matrix;

/// Allocate memory for dynamic matrix
matrix createMatrix(int rows, int cols);

/// Free a dynamic matrix
void freeMatrix(matrix *mat, int rows);

/// Dynamically reads a matrix
matrix readMatrix(int rows, int cols);

/// Dynamically reads a matrix with rows and columns that the user enters
matrix readCustomSizeMatrix(int *rows, int *cols);

/// Prints a matrix with a fixed padding (10)
void printMatrix(matrix mat, int rows, int cols);
void printMatricesExpression(matrix mat1, int rows1, int cols1, matrix mat2, int rows2, int cols2, matrix mat3, int rows3, int cols3, char operator);

#endif //UTILS_MATRIX_H
