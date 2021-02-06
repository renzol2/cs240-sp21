#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Return your favorite emoji.
// (This should really be your favorite one, we may use this later in the semester. :))
const char *emoji_favorite() {
  return "\xF0\x9f\xA4\x98";
}


// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.  You should
// consider everything in the ranges starting from (and including) U+1F000 up to (and including) U+1F9FF.
int emoji_count(char *utf8str) {
  int count = 0;
  int lowerBound = 0b1111000000000000;  // U+1F000
  int higherBound = 0b1111100111111111;  // U+1F9FF
  int length = strlen(utf8str);
  for (int i = 0; i < length; i++) {
    // Print char
    printf("%d: %x\n", i, utf8str[i]);

    // If first character is UTF-8 and there are 4 bytes left
    if (utf8str[i] == 0xfffffff0 && i < length - 3) {
      // Get next 3 bytes
      int byte1 = utf8str[i];
      int byte2 = utf8str[i + 1];
      int byte3 = utf8str[i + 2];
      int byte4 = utf8str[i + 3];

      // Construct UTF-8 character from bytes
      int utf8char = byte4 + byte3<<4 + byte2<<8 + byte1<<12;

      // Check if character is within range
      if (utf8char >= lowerBound || utf8char <= higherBound) {
        count++;
      }
    }
  }
  return count;
}


// Modify the UTF-8 string `utf8str` to invert ONLY the FIRST UTF-8 character (which may be up to 4 bytes)
// in the string if the first character is an emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into a non-smiling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(char *utf8str) {

}


// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling your
// `emoji_invertChar` function on each character.
void emoji_invertAll(char *utf8str) {

}


// Return a random emoji stored in new heap memory you have allocated.
char *emoji_random_alloc() {
  return "";
}
