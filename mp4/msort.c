/** @file msort.c */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int allowPrinting = 0;

typedef struct _sort_params {
  int* arrayToSort;
  int sortLength;
  int startIndex;
} sort_params;

typedef struct _merge_params {
  int* arr;
  int segmentIndex1;
  int segmentIndex2;
  int segmentLength1;
  int segmentLength2;
} merge_params;

/**
 * Returns the difference of a and b
 */
int compare(const void* a, const void* b) { return (*(int*)a - *(int*)b); }

/**
 * Sorts a segment with given `sort_params`
 */
void* sortSegment(void* ptr) {
  sort_params p = *((sort_params*)ptr);
  // Sort the array with given index + length
  qsort(p.arrayToSort + p.startIndex, p.sortLength, sizeof(int), compare);
  fprintf(stderr, "Sorted %d elements.\n", p.sortLength);
  return NULL;
}

/**
 * Merges two segments with given `merge_params`
 * Assumes segment 1 is the "left" segment, i.e. lower index
 */
void* mergeSegments(void* ptr) {
  merge_params p = *((merge_params*)ptr);

  if (allowPrinting) {
    printf("Merging segment indices %d and %d with lengths %d and %d\n",
          p.segmentIndex1, p.segmentIndex2, p.segmentLength1, p.segmentLength2);
  }

  // Create temporary lists of each array
  // int segment1[p.segmentLength1];
  // int segment2[p.segmentLength2];
  int* segment1 = malloc(sizeof(int) * p.segmentLength1);
  int* segment2 = malloc(sizeof(int) * p.segmentLength2);
  for (int i = 0; i < p.segmentLength1; i++) {
    if (allowPrinting) {
      printf("arr[%d]: %d\n", p.segmentIndex1 + i, p.arr[p.segmentIndex1 + i]);
    }
    segment1[i] = p.arr[p.segmentIndex1 + i];
  }
  for (int i = 0; i < p.segmentLength2; i++) {
    if (allowPrinting) {
      printf("arr[%d]: %d\n", p.segmentIndex2 + i, p.arr[p.segmentIndex2 + i]);
    }
    segment2[i] = p.arr[p.segmentIndex2 + i];
  }

  // Merge two segments in the array
  int totalLength = p.segmentLength1 + p.segmentLength2;
  int numDuplicates = 0;
  int seg1Index = 0;
  int seg2Index = 0;
  // Starting from the original segment and into the adjacent segment
  for (int i = p.segmentIndex1; i < p.segmentIndex1 + totalLength; i++) {
    int a = segment1[seg1Index];
    int b = segment2[seg2Index];
    // True if we've finished everything in segment 1
    int endOfSegment1 = seg1Index == p.segmentLength1 ? 1 : 0;
    // True if we've finished everything in segment 2
    int endOfSegment2 = seg2Index == p.segmentLength2 ? 1 : 0;

    if (allowPrinting) {
      printf("a: %d, b: %d, s1i: %d, s2i: %d\n", a, b, seg1Index, seg2Index);
    }

    // Compare the numbers
    if (endOfSegment1) {
      p.arr[i] = b;
      seg2Index++;
    } else if (endOfSegment2) {
      p.arr[i] = a;
      seg1Index++;
    } else if (a < b) {
      p.arr[i] = a;
      seg1Index++;
    } else if (a > b) {
      p.arr[i] = b;
      seg2Index++;
    } else {
      // TODO: count duplicates properly
      numDuplicates++;
      // Advance one of the lists
      if (endOfSegment1) {
        p.arr[i] = b;
        seg2Index++;
      } else if (endOfSegment2) {
        p.arr[i] = a;
        seg1Index++;
      } else {
        // Default to segment 1
        p.arr[i] = a;
        seg1Index++;
      }
    }
    
    if (allowPrinting) {
      printf("in merge, arr[%d]: %d\n", i, p.arr[i]);
    }
  }

  fprintf(stderr, "Merged %d and %d elements with %d duplicates.\n",
          p.segmentLength1, p.segmentLength2, numDuplicates);
  free(segment1);
  free(segment2);
  return NULL;
}

/**
 * CS 240 MP4
 */
int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: ./msort [NUM SEGMENTS] <[FILE]\n");
    return 1;
  }

  // Get number of segments
  int numSegments = atoi(argv[1]);
  if (allowPrinting) printf("Number of segments: %d\n", numSegments);

  // Initialize array
  const int INITIAL_LENGTH = 100;
  const int REALLOC_MULTIPLIER = 2;
  int currentLength = INITIAL_LENGTH;
  int* arr = malloc(sizeof(int) * INITIAL_LENGTH);

  int scanResult = 1;
  int num;
  int index = 0;

  // While before end of file
  while (scanResult != -1) {
    // Check if line # in file is greater than allocated space
    if (index >= currentLength - 1) {
      // Reallocate more memory to array
      arr = realloc(arr, sizeof(int) * currentLength * REALLOC_MULTIPLIER);
      currentLength *= REALLOC_MULTIPLIER;
      if (allowPrinting) printf("current size: %d\n", currentLength);
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

  if (allowPrinting) printf("total # of numbers: %d\n", numElements);

  // Calculate # of values per segment
  int valuesPerSegment;
  if (numElements % numSegments == 0) {
    valuesPerSegment = numElements / numSegments;
  } else {
    valuesPerSegment = (numElements / numSegments) + 1;
  }

  if (allowPrinting) printf("Values per segment: %d\n", valuesPerSegment);

  // If # of values don't divide evenly, find leftover # of values
  int leftover = numElements - (numSegments - 1) * valuesPerSegment;
  int hasLeftover = leftover == valuesPerSegment ? 0 : 1;
  
  // If leftover is 0, it's easier to treat it as if we have
  // one less segment (pitfall of required calculation of value per segment)
  if (leftover == 0) {
    numSegments -= 1;
    hasLeftover = 0;
  }

  if (allowPrinting) printf("Leftover: %d\n", leftover);

  // Launch threads to sort
  pthread_t sortThreads[numSegments];
  sort_params sortParams[numSegments];
  for (int i = 0; i < numSegments; i++) {
    if (i == numSegments - 1 && hasLeftover) {
      sortParams[i].sortLength = leftover;
    } else {
      sortParams[i].sortLength = valuesPerSegment;
    }
    sortParams[i].startIndex = i * valuesPerSegment;
    sortParams[i].arrayToSort = arr;
    pthread_create(&sortThreads[i], NULL, sortSegment, (void*)&(sortParams[i]));
  }

  // Join threads
  for (int i = 0; i < numSegments; i++) {
    pthread_join(sortThreads[i], NULL);
  }

  // Print array
  if (allowPrinting) {
    printf("\nAfter sorting:\n");
    for (int i = 0; i < numElements; i++) {
      printf("arr[%d]: %d\n", i, arr[i]);
    }
  }

  // Begin merging segments
  int currentNumSegments = numSegments;
  int currentSegmentLength = valuesPerSegment;
  int irregularSegmentLength = leftover;
  while (currentNumSegments > 1) {
    int numMerges = currentNumSegments / 2;
    int mergeLeftover = numElements - (currentNumSegments - 1) * currentSegmentLength;
    if (allowPrinting) printf("merge leftover: %d\n", mergeLeftover);
    if (mergeLeftover != currentSegmentLength) {
      irregularSegmentLength = mergeLeftover;
      hasLeftover = 1;
    } else {
      hasLeftover = 0;
    }

    pthread_t mergeThreads[numMerges];
    merge_params mergeParams[numMerges];

    // Create threads to merge segments
    for (int i = 0; i < numMerges; i++) {
      // Create params to sort
      mergeParams[i].arr = arr;

      int segment1Index = currentSegmentLength * (2 * i);
      int segment2Index = currentSegmentLength * (2 * i + 1);
      mergeParams[i].segmentIndex1 = segment1Index;
      mergeParams[i].segmentIndex2 = segment2Index;
      mergeParams[i].segmentLength1 = currentSegmentLength;

      // If "rightmost" merge and we have irregular segment
      if (segment2Index >= numElements - currentSegmentLength && hasLeftover) {
         mergeParams[i].segmentLength2 = irregularSegmentLength;
         irregularSegmentLength += currentSegmentLength;
      } else {
        // If not, segment length is standard
        mergeParams[i].segmentLength2 =
            currentSegmentLength;
      }

      if (allowPrinting) {
        printf(
            "Merge params\n s1 index: %d\n s1 length: %d\n s2 index: %d\n s2 "
            "length: %d\n",
            mergeParams[i].segmentIndex1, mergeParams[i].segmentLength1,
            mergeParams[i].segmentIndex2, mergeParams[i].segmentLength2);
      }

      // Launch thread
      pthread_create(&mergeThreads[i], NULL, mergeSegments,
                     (void*)&mergeParams[i]);
    }

    // Join merge threads
    for (int i = 0; i < numMerges; i++) {
      pthread_join(mergeThreads[i], NULL);
      // Every merge decreases the # of segments by 1
      currentNumSegments--;
    }

    // Update segment length
    currentSegmentLength *= 2;
  }

  // Print array (required)
  for (int i = 0; i < numElements; i++) {
    printf("%d\n", arr[i]);
  }

  free(arr);
  return 0;
}
