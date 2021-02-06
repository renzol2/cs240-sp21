#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int CASE_DISTANCE = 32;


int isLowercase(char c) {
  return c >= 'a' && c <= 'z';
}

void makeUppercase(char* c) {
  *c -= CASE_DISTANCE;
}

// Modify the string `s` to capitalize the first letter of the string if the
// first letter of `s` is lower case.  Otherwise, do nothing.
void capitalize(char *s) {
  char first = s[0];
  // if lowercase
  if (isLowercase(first)) {
    makeUppercase(&first);
  }
}

// Modify the string `s` to capitalize ALL of the lower case letters in `s`.  If
// the letter is not lower case, do nothing.  (You can assume you will be given
// only ASCII characters.)
void capitalizeAll(char *s) {
  for (int i = 0; i < strlen(s); i++) {
    if (isLowercase(s[i])) {
      makeUppercase(&s[i]);
    }
  }
}

// Return a new string (allocated in heap memory that you allocate) that
// transforms `s` in the same way as `capitalizeAll`.  (In this function, `s`
// cannot be modified directly and must be done in the new memory you will
// allocate.)
char *capitalizeAll_alloc(const char *s) {
  char *copy = malloc(strlen(s));
  strcpy(copy, s);
  capitalizeAll(copy);
  return copy;
}
