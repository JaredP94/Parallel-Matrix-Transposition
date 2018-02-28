#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
 #include "omp.h"

int* generateSquareMatrix(int _dimension, int noOfThreads)
{
    printf("entered function");
    int size = _dimension*_dimension;
    static int * _created_squareMatrix; // Creates a pointer to a block of memory on the heap
    _created_squareMatrix =(int*)malloc(size * sizeof(int));
     printf("allocated memory");
    // If the matrix cannot be created, exit the program
    if (_created_squareMatrix == NULL)
    {
        printf("Could not allocate required memory\n");
        exit(1);
    }
    #pragma omp parallel num_threads(noOfThreads)
    {
        #pragma omp for schedule(static, size/256) // make the creation of the matrix parallel
        for (int i=0; i < (size); i++)
        {
            _created_squareMatrix[i] = i + 1;
            // printf("%d ",i);
        }
    }

    printf("Created array to be transposed");

    return _created_squareMatrix;
}

// Function to swap two values
void swap(int* i, int* j) 
{
    int temp;
    temp = *i;
    *i = *j;
    *j = temp;
}

//Function to calculate whether the index to be swapped has already been
// swapped or not
bool isValueInArray(int array[], int value, int array_size) 
{
    for (int i = 0; i < array_size; i++)
    {
        if (array[i] == value){
            return true;
        }
    }
    return false;
}
// Function to transpose a square matrix
int* transpose(int* squareMatrix, int dimension, int noOfThreads)
{
    int size = dimension*dimension;
    int* swappedIndices = (int*)malloc((size) * sizeof(int));

    #pragma omp parallel num_threads(noOfThreads)
    {
        #pragma omp for schedule(static, size/256) // make the creation of the matrix parallel
        for (int i=0; i < (size); i++)
        {
            swappedIndices[i]=0;
            // printf("%d ",i);
        }
  
        #pragma omp for ordered schedule(static, size/256) //try chunk size as size/256
        for (int index=1; index< size-1; index++)
        {
            int newPosition = (index*dimension)%(size-1);
            swappedIndices[index-1]=newPosition;

            #pragma omp ordered 
            if (!isValueInArray(swappedIndices, index, index-1) && newPosition>index)
            {
                swap(&squareMatrix[index], &squareMatrix[newPosition]);
            }
        }
    }
    free(swappedIndices);
}

// Function to print the matrix
void printMatrix(int* squareMatrix, int dimension)
{
    // Calculates the number of digits in the largest number in the matrix
    int count = log10(squareMatrix[dimension*dimension-1]) + 2;

    for (int i = 0; i < dimension*dimension; i++)
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

    int dimension = 128;

// Time the parallel transposition
    int* squareMatrix2= generateSquareMatrix(dimension, 4);
    clock_t begin2 = clock();
    transpose(squareMatrix2, dimension, 4);
    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("Time spent parallel %f", time_spent2);
    printf("\n");
    free(squareMatrix2);

    return 0;
}