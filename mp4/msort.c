/** @file msort.c */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _sort_params {
  int sortLength;
  int startIndex;
  int* arrayToSort;
} sort_params;

int compare(const void* a, const void* b) { return (*(int*)a - *(int*)b); }

void* sortSegment(void* ptr) {
  sort_params parameters = *((sort_params*)ptr);
  printf("Sorting at index %d with length %d\n", parameters.startIndex,
         parameters.sortLength);
  // Sort the array with given index + length
  qsort(parameters.arrayToSort + parameters.startIndex, parameters.sortLength,
        sizeof(int), compare);
  return NULL;
}

int main(int argc, char** argv) {
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
  sort_params parameters[numSegments];
  for (int i = 0; i < numSegments; i++) {
    if (i == numSegments - 1 && hasLeftover) {
      parameters[i].sortLength = leftover;
    } else {
      parameters[i].sortLength = valuesPerSegment;
    }
    parameters[i].startIndex = i * valuesPerSegment;
    parameters[i].arrayToSort = arr;
    pthread_create(&threads[i], NULL, sortSegment, (void*)&(parameters[i]));
  }

  // Join threads
  for (int i = 0; i < numSegments; i++) {
    pthread_join(threads[i], NULL);
  }

  // Begin merging segments
  

  // Print array
  for (int i = 0; i < numElements; i++) {
    printf("arr[%d]: %d\n", i, arr[i]);
  }

  free(arr);
  return 0;
}
