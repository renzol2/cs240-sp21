#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/png.h"

// 3 args: exe, PNG file to read, GIF file to write into
// Extract `uiuc` chunk
// Put `uiuc` data into new GIF file
int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF File>\n", argv[0]);
    return 1;
  }

  // Open the file specified in argv[1] for reading:
  FILE *pngFile = fopen(argv[1], "r");

  // Read the PNG header, exiting if it is not a valid PNG:
  int result;
  result = PNGChunk_readHeader(pngFile);
  if (result != 0) {
    fclose(pngFile);
    return result;
  }
  printf("PNG Header: OK\n");

  PNGChunk *uiuc = NULL;
  char uiucName[5] = "uiuc";

  // Read chunks until reaching "IEND" or in invalid chunk:
  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    PNGChunk *chunk = PNGChunk_readChunk_alloc(pngFile);
    if (chunk == NULL) {
      fclose(pngFile);
      return 1;
    }

    // Report data about the chunk to the command line:
    printf("Chunk: %s (%d bytes of data)\n", chunk->type, chunk->len);

    // Check for the "IEND" chunk to exit:
    if (strcmp(chunk->type, "IEND") == 0) {
      PNGChunk_free(chunk);
      break;
    }

    // Check if it's the uiuc chunk
    if (strcmp(uiucName, chunk->type) == 0) {
      uiuc = chunk;
    } else {
      // Free the memory associated with the chunk we just read
      // if it's not the uiuc chunk (we need the uiuc chunk later)
      PNGChunk_free(chunk);
    }
  }

  fclose(pngFile);

  if (uiuc == NULL) {
    printf("No uiuc chunk found\n");
    return 1;
  }

  printf("uiuc chunk: %s (%d bytes of data)\n", uiuc->type, uiuc->len);

  // Create file to put GIF in
  FILE* gifFile = fopen(argv[2], "w");
  fwrite(uiuc->data, sizeof(uiuc->data), uiuc->len, gifFile);
  fclose(gifFile);

  PNGChunk_free(uiuc);

  return 0;
}
