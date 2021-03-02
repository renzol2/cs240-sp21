/**
 * Malloc
 * CS 240 - Fall 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _metadata_t {
  /** Size of the memory block */
  unsigned int size;
  /** 0 if block is free, 1 if block is used. */
  unsigned char isUsed;
  /** Pointer to next free block */
  struct _metadata_t* next;
  /** Pointer to prev free block */
  struct _metadata_t* prev;
  /** Pointer to previous block in heap (free or not) */
  struct _metadata_t* heapPrev;
} metadata_t;

/** Global variable to keep track of start of heap */
void* startOfHeap = NULL;
/** 1 to print, 0 to not print */
int allowPrinting = 0;
/** Head of free list */
metadata_t* head = NULL;
/** Tail of free list */
metadata_t* tail = NULL;

/**
 * Prints the heap
 */
void printHeap() {
  // If we have not recorded the start of the heap, record it
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);  // returns end of heap without increasing
  }

  metadata_t* currentMeta = startOfHeap;
  void* endOfHeap = sbrk(0);
  if (allowPrinting) printf("-- Start of Heap (%p) --\n", startOfHeap);

  while ((void*)currentMeta < endOfHeap) {
    if (allowPrinting) {
      printf(" metadata for memory %p: (%p, size=%d, isUsed=%d)\n",
             (void*)currentMeta + sizeof(metadata_t), currentMeta,
             currentMeta->size, currentMeta->isUsed);
    }
    currentMeta = (void*)currentMeta + currentMeta->size + sizeof(metadata_t);
  }
  if (allowPrinting) printf("-- End of Heap (%p) --\n\n", endOfHeap);
}

void printFreeList() {
  metadata_t* currentMeta = head;
  printf("-- Start of free list --\n");
  while (currentMeta != NULL) {
    printf(" metadata for memory %p: (%p, size=%d)\n",
           (void*)currentMeta + sizeof(metadata_t), currentMeta,
           currentMeta->size);
    currentMeta = currentMeta->next;
  }
  printf("-- End of free list --\n\n");
}

/**
 * Coalesces (combines) adjacent blocks of memory
 */
void coalesceMemory() {
  if (allowPrinting) printf("Inside: coalesceMemory\n");
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);  // returns end of heap without increasing
  }

  metadata_t* currentMeta = startOfHeap;
  metadata_t* prevMeta = startOfHeap;
  void* endOfHeap = sbrk(0);

  while ((void*)currentMeta < endOfHeap) {
    // Get current meta to next memory chunk
    currentMeta = (void*)currentMeta + currentMeta->size + sizeof(metadata_t);

    // Check if both prev and current are free
    if (currentMeta->isUsed == 0 && prevMeta->isUsed == 0) {
      // Join together
      prevMeta->size = prevMeta->size + sizeof(metadata_t) + currentMeta->size;
    }

    prevMeta = currentMeta;
  }
}

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void* calloc(size_t num, size_t size) {
  // implement calloc:
  void* ptr = malloc(num * size);
  memset(ptr, 0, num * size);
  return ptr;
}

void* mallocExplicit(size_t size) {
  // Traverse through free list
  metadata_t* current = head;
  metadata_t* previous = current;
  while (current != NULL) {
    // Check if current has enough space for requested size
    if (current->size >= size) {
      // Split block if necessary
      int sizeDiff = current->size - size;
      // If I'm able to split
      if (sizeDiff > sizeof(metadata_t)) {
        metadata_t* split = (void*)current + sizeof(metadata_t) + size;
        split->isUsed = 0;

        // Transfer pointers
        split->next = current->next;
        split->prev = current->prev;
        split->size = sizeDiff - sizeof(metadata_t);
        current->size = size;
      }

      // Set current block to in use
      current->isUsed = 1;
      if (current->prev != NULL) {
        current->prev->next = current->next;
      }
      if (current->next != NULL) {
        current->next->prev = current->prev;
      }
      
      // Return the location of the current block
      return (void*)current + sizeof(metadata_t);
    } else {
      // If block isn't big enough, go to next free block
      previous = current;
      current = current->next;
    }
  }

  metadata_t* meta = sbrk(sizeof(metadata_t));
  meta->size = size;
  meta->isUsed = 1;
  meta->prev = NULL;
  meta->next = NULL;
  meta->heapPrev = previous;  // can be free or not

  if (allowPrinting) printFreeList();

  void* ptr = sbrk(size);
  return ptr;
}

/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */
void* malloc(size_t size) {
  if (allowPrinting) {
    printf("Inside: malloc(%lu):\n", size);
    printHeap();
  }
  return mallocExplicit(size);
}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void* ptr) {
  if (ptr == NULL) return;
  if (allowPrinting) {
    printf("Inside: free(%p)\n", ptr);
  } 
    
  // Find the metadata located immediately before `ptr`:
  metadata_t* meta = ptr - sizeof(metadata_t);

  // Mark the allocation as free
  meta->isUsed = 0;

  // Add to free list
  if (tail != NULL) {
    tail->next = meta;
  }
  meta->prev = tail;
  meta->next = NULL;
  tail = meta;

  if (head == NULL) {
    meta->prev = NULL; 
    head = meta;
  }

  // Coalesce memory
  metadata_t* current = head;
  while (current != NULL) {
    // Get the previous and next metadata
    metadata_t* previous = current->heapPrev;
    metadata_t* next = (void*)current + sizeof(metadata_t) + current->size;

    if (next != NULL && next->isUsed == 0) {
      current->size = current->size + sizeof(metadata_t) + next->size; 
      next->size = 0;
      next->prev = NULL;
      next->next = NULL;
      next->heapPrev = NULL;
    }

    if (previous != NULL && previous->isUsed == 0) {
      current->size = current->size + sizeof(metadata_t) + previous->size;
      previous->size = 0;
      previous->prev = NULL;
      previous->next = NULL;
      previous->heapPrev = NULL;
    }

    current = current->next;
  }

  if (allowPrinting) {
    printFreeList();
  }

}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void* realloc(void* ptr, size_t size) {
  // FIXME: unsure if correct
  // implement realloc:
  if (ptr == NULL) {
    return malloc(size);
  } else if (size == 0 && ptr != NULL) {
    free(ptr);
    return NULL;
  }

  void* contents;
  memcpy(contents, ptr, size);

  free(ptr);
  void* location = malloc(size);
  memcpy(location, contents, size);
  return location;
}