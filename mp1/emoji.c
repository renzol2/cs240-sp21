#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Checks if `c` signifies a 4-byte UTF-8 encoded character
 * Returns 1 if true, 0 if false
 */
int isUtf8Prefix(char c) {
  if (c == 0xfffffff0) {
    return 1;
  } else {
    return 0;
  }
}

int isBlushEmoji(char *s, int i) {
  if (s[i + 1] == 0xffffff9f && s[i + 2] == 0xffffff98 &&
      s[i + 3] == 0xffffff8a) {
    return 1;
  } else {
    return 0;
  }
}

int isSparklingHeart(char *s, int i) {
  if (s[i + 1] == 0xffffff9f && s[i + 2] == 0xffffff92 &&
      s[i + 3] == 0xffffff96) {
    return 1;
  } else {
    return 0;
  }
}

int isPersonalComputer(char *s, int i) {
  if (s[i + 1] == 0xffffff9f && s[i + 2] == 0xffffff92 &&
      s[i + 3] == 0xffffffBB) {
    return 1;
  } else {
    return 0;
  }
}

int isPopper(char *s, int i) {
  if (s[i + 1] == 0xffffff9f && s[i + 2] == 0xffffff8e &&
      s[i + 3] == 0xffffff89) {
    return 1;
  } else {
    return 0;
  }
}

int isSunFace(char *s, int i) {
  if (s[i + 1] == 0xffffff9f && s[i + 2] == 0xffffff8c &&
      s[i + 3] == 0xffffff9e) {
    return 1;
  } else {
    return 0;
  }
}

int isDogFace(char *s, int i) {
  if (s[i + 1] == 0xffffff9f && s[i + 2] == 0xffffff90 &&
      s[i + 3] == 0xffffffb6) {
    return 1;
  } else {
    return 0;
  }
}

// Return your favorite emoji.
// (This should really be your favorite one, we may use this later in the
// semester. :))
const char *emoji_favorite() { return "\xF0\x9f\xA4\x98"; }

// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.
// You should consider everything in the ranges starting from (and including)
// U+1F000 up to (and including) U+1F9FF.
int emoji_count(char *utf8str) {
  char *s = utf8str;
  int count = 0;
  int lowerBound = 0b1111000000000000;   // U+1F000
  int higherBound = 0b1111100111111111;  // U+1F9FF
  int length = strlen(s);
  for (int i = 0; i < length; i++) {
    // If first character is UTF-8 4 bytes and there are 4 bytes left
    if (isUtf8Prefix(s[i]) && i < length - 3) {
      // Get next 3 bytes
      int byte1 = s[i];
      int byte2 = s[i + 1];
      int byte3 = s[i + 2];
      int byte4 = s[i + 3];

      // Construct UTF-8 character from bytes
      int utf8char = byte4 + byte3 << 4 + byte2 << 8 + byte1 << 12;

      // Check if character is within range
      if (utf8char >= lowerBound || utf8char <= higherBound) {
        count++;
      }
    }
  }
  return count;
}

// Modify the UTF-8 string `utf8str` to invert ONLY the FIRST UTF-8 character
// (which may be up to 4 bytes) in the string if the first character is an
// emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into a non-smiling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(char *utf8str) {
  char *s = utf8str;
  int length = strlen(s);
  for (int i = 0; i < length; i++) {
    if (isUtf8Prefix(s[i]) && i < length - 3) {
      if (isBlushEmoji(s, i)) {
        s[i + 3] = 0xad;
      } else if (isSparklingHeart(s, i)) {
        s[i + 3] = 0x94;
      } else if (isPersonalComputer(s, i)) {
        s[i + 2] = 0x93;
        s[i + 3] = 0x93;
      } else if (isPopper(s, i)) {
        s[i + 2] = 0x94;
        s[i + 3] = 0xaa;
      } else if (isSunFace(s, i)) {
        s[i + 2] = 0x8c;
        s[i + 3] = 0x9d;
      } else if (isDogFace(s, i)) {
        s[i + 2] = 0x90;
        s[i + 3] = 0xb1;
      }

      // Only invert first in string
      break;
    }
  }
}

// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling
// your `emoji_invertChar` function on each character.
void emoji_invertAll(char *utf8str) {
  for (int i = 0; i < strlen(utf8str); i++) {
    emoji_invertChar(utf8str + i);
  }
}

// Return a random emoji stored in new heap memory you have allocated.
char *emoji_random_alloc() {
  srand(time(0));
  printf("%d\n", rand());
  char* c = malloc(sizeof(char));
  return c;
}
