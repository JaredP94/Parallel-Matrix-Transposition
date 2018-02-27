#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
 #include "omp.h"

int* generateSquareMatrix(int _dimension)
{
    static int * _created_squareMatrix; // Creates a pointer to a block of memory on the heap
    _created_squareMatrix =(int*)malloc((_dimension*_dimension) * sizeof(int));

    // If the matrix cannot be created, exit the program
    if (_created_squareMatrix == NULL)
    {
        printf("Could not allocate required memory\n");
        exit(1);
    }
#pragma omp for schedule(dynamic, 256) // make the creation of the matrix parallel
    for (int i=0; i < (_dimension*_dimension); i++)
    {
        _created_squareMatrix[i]=i+1;
      //  printf("%d ",i);
    }

    return _created_squareMatrix;
}

// Function to swap two values
void swap(int* i, int* j) 
{
    int temp;
    temp= *i;
    *i= *j;
    *j= temp;
}

//Function to calculate whether the index to be swapped has already been
// swapped or not
bool isValueInArray(int array[], int value, int array_size) 
{
    for (int i = 0; i < array_size - 1; i++)
    {
        if (array[i] == value){
            return true;
        }
    }
    return false;
}

// Function to transpose a square matrix
int* transpose(int* squareMatrix, int dimension)
{
    int size=dimension*dimension;
    int* swappedIndices = (int*)malloc((size-2) * sizeof(int));
    
    for (int index=1; index< size-1; index++)
    {
        int newPosition = (index*dimension)%(size-1);
        swappedIndices[index-1]=newPosition;
        if (!isValueInArray(swappedIndices, index, size) && newPosition>index)
        {
            swap(&squareMatrix[index], &squareMatrix[newPosition]);
        }
    }
}

// Function to transpose a square matrix
int* transposeParallel(int* squareMatrix, int dimension)
{
    int size=dimension*dimension;
    int* swappedIndices = (int*)malloc((size-2) * sizeof(int));
    #pragma omp parallel num_threads(64)
    {    
        #pragma omp for ordered schedule(static, 256) //try chunk size as size/256
        for (int index=1; index< size-1; index++)
        {
            int newPosition = (index*dimension)%(size-1);
            swappedIndices[index-1]=newPosition;

            #pragma omp ordered 
            if (!isValueInArray(swappedIndices, index, size) && newPosition>index)
            {
                swap(&squareMatrix[index], &squareMatrix[newPosition]);
            }
        }
    }
}

// Function to print the matrix
void printMatrix(int* squareMatrix, int dimension)
{
    // Calculates the number of digits in the largest number in the matrix
    int count = log10(squareMatrix[dimension*dimension-1]) + 2;

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

    int dimension = 256;

    int* squareMatrix= generateSquareMatrix(dimension);

//    printMatrix(squareMatrix, dimension);

//    printf("\n\n");

// Time the serial transposition
    clock_t begin = clock();
    transpose(squareMatrix, dimension);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent serial %f", time_spent);
    printf("\n");
    free(squareMatrix);

// Time the parallel transposition
    int* squareMatrix2= generateSquareMatrix(dimension);
    clock_t begin2 = clock();
    transposeParallel(squareMatrix2, dimension);
    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("Time spent parallel %f", time_spent2);
    printf("\n");
    free(squareMatrix2);

    // printf("\n\n");
    // printf("Final matrix: \n");

    // printMatrix(squareMatrix2, dimension);
    // printf("\n");
    // free(squareMatrix2);

    return 0;
}