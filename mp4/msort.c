/** @file msort.c */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./msort [NUM SEGMENTS] <[FILE]\n");
    return 1;
  }

  // Get number of segments
  int numSegments = argv[1];

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

  for (int i = 0; i < numElements; i++) {
    printf("arr[i]: %d\n", arr[i]);
  }

  printf("total # of numbers: %d\n", numElements);

	return 0;
}
