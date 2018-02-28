#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int* generateSquareMatrix(int _dimension)
{
    printf("entered function");

    int size = _dimension * _dimension;

    static int * _created_squareMatrix; // Creates a pointer to a block of memory on the heap
    
    _created_squareMatrix = (int*) malloc((size) * sizeof(int));

    printf("allocated memory");

    // If the matrix cannot be created, exit the program
    if (_created_squareMatrix == NULL)
    {
        printf("Could not allocate required memory\n");
        exit(1);
    }

    for (int i = 0; i < (size); i++)
    {
        _created_squareMatrix[i] = i + 1;
    }

    printf("Created array to be transposed");

    return _created_squareMatrix;
}

// Function to swap two values
void swap(int* i, int* j) 
{
    long int temp;
    temp = *i;
    *i = *j;
    *j = temp;
}

//Function to transpose a square matrix
int* transpose(int* squareMatrix, int dimension)
{
    int size = dimension * dimension;

    printf("Created 0 array");
    
    for (long int index = 1; index < dimension; index++)
    {
        for (long int j = 0; j < index; j++)
        {
            long int currentIndex = index * dimension + j;
            long int newPosition = (currentIndex * dimension) % (size - 1);

             swap(&squareMatrix[currentIndex], &squareMatrix[newPosition]);
        }
    }
}

// Function to print the matrix
void printMatrix(int* squareMatrix, int dimension)
{
    // Calculates the number of digits in the largest number in the matrix
    int count = log10(squareMatrix[dimension * dimension - 1]) + 2;

    for (int i=0; i< dimension*dimension; i++)
    {
        if (i % dimension==0)
        {
            printf("\n");
        }
        // Format the output matrix so that each column has the width
        // of the largest number + 1
        printf("%*d", count, squareMatrix[i]); 
    }
}

int main()
{

    int dimension = 8192;

    int* squareMatrix= generateSquareMatrix(dimension);

// Time the serial transposition
    clock_t begin = clock();

    transpose(squareMatrix, dimension);

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time spent serial %f", time_spent);
    
    printf("\n");

    free(squareMatrix);

    return 0;
}