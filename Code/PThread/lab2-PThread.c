#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

struct ThreadData {
    int start, stop;
    int* array;
};

struct ThreadTransposeData {
    int start, stop, dimension, size;
    int* array;
};

void* generateValuesForMatrix(void *ThreadData)
{
    struct ThreadData* data = (struct ThreadData*) ThreadData;
    int start = data->start;
    int stop = data->stop;
    int* array = data->array;

    for (int i = start; i < stop; i++)
    {
        array[i] = i + 1;
    }

    return NULL;
}

int* generateSquareMatrix(int _dimension, int noOfThreads)
{
    int size = _dimension * _dimension;
    static int * _created_squareMatrix; // Creates a pointer to a block of memory on the heap
    _created_squareMatrix = (int*) malloc((size) * sizeof(int));

    pthread_t thread[noOfThreads];
    struct ThreadData data[noOfThreads];
    int tasksPerThread = (size + noOfThreads - 1) / noOfThreads;

    // If the matrix cannot be created, exit the program
    if (_created_squareMatrix == NULL)
    {
        printf("Could not allocate required memory\n");
        exit(1);
    }

    for (int i = 0; i < noOfThreads; i++) {
        data[i].start = i * tasksPerThread;
        data[i].stop = (i+1) * tasksPerThread;
        data[i].array = _created_squareMatrix;
    }

    data[noOfThreads - 1].stop = size;

    /* Launch Threads */
    for (int i = 0; i < noOfThreads; i++) {
        pthread_create(&thread[i], NULL, generateValuesForMatrix, &data[i]);
    }

    /* Wait for Threads to Finish */
    for (int i = 0; i < noOfThreads; i++) {
        pthread_join(thread[i], NULL);
    }

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

void* threadTranspose(void *ThreadTransposeData)
{
    struct ThreadTransposeData* data = (struct ThreadTransposeData*) ThreadTransposeData;
    int start = data->start;
    int stop = data->stop;
    int dimension = data->dimension;
    int size = data->size;
    int* array = data->array;

    for (long int index = start; index < stop; index++)
    {
        for (long int j = 0; j < index; j++)
        {
            long int currentIndex = index * dimension + j;
            long int newPosition = (currentIndex * dimension) % (size - 1);

            swap(&array[currentIndex], &array[newPosition]);
        }
    }

    return NULL;
}

//Function to transpose a square matrix
int* transpose(int* squareMatrix, int dimension, int noOfThreads)
{
    int size = dimension * dimension;
        
    pthread_t thread[noOfThreads];
    struct ThreadTransposeData data[noOfThreads];
    int tasksPerThread = (dimension + noOfThreads - 1) / noOfThreads;

    for (int i = 0; i < noOfThreads; i++) {
        data[i].start = i * tasksPerThread;
        data[i].stop = (i+1) * tasksPerThread;
        data[i].dimension = dimension;
        data[i].size = size;
        data[i].array = squareMatrix;
    }

    data[noOfThreads - 1].stop = dimension;

    /* Launch Threads */
    for (int i = 0; i < noOfThreads; i++) {
        pthread_create(&thread[i], NULL, threadTranspose, &data[i]);
    }

    /* Wait for Threads to Finish */
    for (int i = 0; i < noOfThreads; i++) {
        pthread_join(thread[i], NULL);
    }

    return squareMatrix;
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

void callFunctions(int dimension, int noOfThreads)
{
    int* squareMatrix= generateSquareMatrix(dimension, noOfThreads);

    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);

    transpose(squareMatrix, dimension, noOfThreads);

    gettimeofday(&tv2, NULL);

    printf("Dimension: %d Number of threads: %d Time to transpose: ",dimension ,noOfThreads);
    printf ("%f seconds\n", (double) (tv2.tv_usec - tv1.tv_usec) / CLOCKS_PER_SEC + (double) (tv2.tv_sec - tv1.tv_sec));

    printf("\n");

    free(squareMatrix);

}

int main()
{

    int dimension[3] = {128, 1024, 8192};
    int threads[5]= {4,8,16,64,128};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            callFunctions(dimension[i], threads[j]);
        }
    }
    
    return 0;
}