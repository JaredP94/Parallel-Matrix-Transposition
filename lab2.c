#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

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

    for (int i=0; i < (_dimension*_dimension); i++)
    {
        _created_squareMatrix[i]=i+1;
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

    int dimension = 16;

    int* squareMatrix= generateSquareMatrix(dimension);

    printMatrix(squareMatrix, dimension);

    printf("\n\n");

    transpose(squareMatrix, dimension);

    printf("\n\n");
    printf("Final matrix: \n");

    printMatrix(squareMatrix, dimension);
    printf("\n");
    free(squareMatrix);

    return 0;
}