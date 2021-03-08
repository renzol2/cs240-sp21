/** @file msort.c */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The number of elements to sort in thread function
int sortLength;
int* arrayToSort;

int compare(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void* sortSegment(void* ptr) {
  printf("Start sortSegment\n");
  int startIndex = *((int*)ptr);
  printf("Thread started with value: %d\n", startIndex);
  qsort(arrayToSort + startIndex, sortLength, sizeof(int), compare);
  return NULL;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./msort [NUM SEGMENTS] <[FILE]\n");
    return 1;
  }

  // Get number of segments
  int numSegments = atoi(argv[1]);
  printf("Number of segments: %d\n", numSegments);

  // Initialize array
  const int INITIAL_LENGTH = 100;
  const int REALLOC_MULTIPLIER = 2;
  int currentLength = INITIAL_LENGTH;
  int* arr = malloc(sizeof(int) * INITIAL_LENGTH);

  int scanResult = 1;
  int num;
  int index;

  // While before end of file
  while (scanResult != -1) {
    // Check if line # in file is greater than allocated space
    if (index >= currentLength - 1) {
      // Reallocate more memory to array
      arr = realloc(arr, sizeof(int) * currentLength * REALLOC_MULTIPLIER);
      currentLength *= REALLOC_MULTIPLIER;
      printf("current size: %d\n", currentLength);
    }

    // Load the number into `num` and put it in array
    scanResult = scanf("%d", &num);
    arr[index] = num;
    index++;
  }

  // Generated sample.txt files usually have an extra line
  // FIXME: maybe not the best solution
  int numElements = index - 1;

  // Reallocate array so it only uses necessary size
  arr = realloc(arr, sizeof(int) * numElements);

  printf("total # of numbers: %d\n", numElements);

  // Calculate # of values per segment
  int valuesPerSegment;
  if (numElements % numSegments == 0) {
    valuesPerSegment = numElements / numSegments;
  } else {
    valuesPerSegment = (numElements / numSegments) + 1;
  }

  printf("Values per segment: %d\n", valuesPerSegment);

  // If # of values don't divide evenly, find leftover # of values
  int leftover = numElements - (numSegments - 1) * valuesPerSegment;
  int hasLeftover = leftover == valuesPerSegment ? 0 : 1;

  // Launch threads to sort
  pthread_t threads[numSegments];
  int startIndices[numSegments];
  for (int i = 0; i < numSegments; i++) {
    // TODO: change sort size for last segment if uneven
    sortLength = valuesPerSegment;
    arrayToSort = arr;
    startIndices[i] = i * valuesPerSegment;
    pthread_create(&threads[i], NULL, sortSegment, (void*)&(startIndices[i]));
  }

  // Join threads
  for (int i = 0; i < numSegments; i++) {
    pthread_join(threads[i], NULL);
  }

  // Print array
  for (int i = 0; i < numElements; i++) {
    printf("arr[%d]: %d\n", i, arr[i]);
  }
  
  free(arr);
	return 0;
}
