/** @file msort.c */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
  if (argc != 3) {
    printf("No\n");
  }

  int numSegments = argv[1];
  FILE* file = fopen(argv[2], "r");
  
  // Check that file exists
  if (file == NULL) {
    printf("no file :(\n");
    return 1;
  }

  // Initialize array
  const int INITIAL_LENGTH = 100;
  int currentLength = INITIAL_LENGTH;
  int* arr = malloc(sizeof(int) * INITIAL_LENGTH);

  // Loop through file
  const int REALLOC_MULTIPLIER = 2;
  const int LINE_LENGTH = 100;
  char line[LINE_LENGTH];
  int i = 0;
  while (fgets(line, LINE_LENGTH, file) != NULL) {
    // Check if line # in file is greater than allocated space
    if (i >= currentLength - 1) {
      // Reallocate more memory to array
      arr = realloc(arr, currentLength * REALLOC_MULTIPLIER);
      currentLength *= REALLOC_MULTIPLIER;
      printf("current size: %d\n", currentLength);
    }

    int num = atoi(line);

    // Add number to array
    arr[i] = num;
    i++;
  }

  int numElements = i;
  printf("total # of numbers: %d\n", numElements);
	return 0;
}
