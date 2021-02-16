#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/png.h"

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 4) {
    printf("Usage: %s <PNG File> <GIF File> <New PNG File>\n", argv[0]);
    return 1;
  }

  // Open the files:
  // 1 is the PNG file with data we hide the GIF in
  // 2 is the GIF file we want to hide
  // 3 is the new PNG file we create
  FILE* originalPng = fopen(argv[1], "r");
  FILE* gifFile = fopen(argv[2], "r");
  FILE* newPng = fopen(argv[3], "w");
  size_t bytesWritten;

  // Read the PNG header, exiting if it is not a valid PNG:
  int result = PNGChunk_readHeader(originalPng);
  if (result != 0) {
    fclose(originalPng);
    return result;
  }

  bytesWritten = PNGChunk_writeHeader(newPng);
  printf("PNG Header written (%lu bytes)\n", bytesWritten);

  // "bool" to check if we've passed the IHDR chunk
  int isAfterIHDR = 0;

  // Read chunks until reaching "IEND" or in invalid chunk:
  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    PNGChunk* chunk = PNGChunk_readChunk_alloc(originalPng);
    if (chunk == NULL) {
      fclose(originalPng);
      return 1;
    }

    if (isAfterIHDR) {
      // TODO: put the gif inside
      // Create new chunk
      PNGChunk* gifChunk = malloc(sizeof(PNGChunk));
      
      // Get the file size
      fseek(gifFile, 0, SEEK_END);
      long gifSize = ftell(gifFile);
      rewind(gifFile);

      // Create copy of data
      char* buffer = malloc(sizeof(char) * gifSize);
      fread(buffer, 1, gifSize, gifFile);

      // Put gif data inside chunk
      gifChunk->data = buffer;
      gifChunk->len = gifSize;
      strcpy(gifChunk->type, "uiuc");

      // Write new chunk into png
      size_t gifChunkBytesWritten = PNGChunk_writeChunk(newPng, gifChunk);
      printf("PNG chunk %s written (%lu bytes)\n", gifChunk->type, gifChunkBytesWritten);
      PNGChunk_free(gifChunk);

      // Set to 'false'
      isAfterIHDR = 0;
    }

    // Report data about the chunk to the command line:
    bytesWritten = PNGChunk_writeChunk(newPng, chunk);
    printf("PNG chunk %s written (%lu bytes)\n", chunk->type, bytesWritten);

    // Check for "IHDR" chunk to mark ready to write "kurapika" chunk
    if (strcmp(chunk->type, "IHDR") == 0) {
      isAfterIHDR = 1;
    }

    // Check for the "IEND" chunk to exit:
    if (strcmp(chunk->type, "IEND") == 0) {
      PNGChunk_free(chunk);
      break;
    }

    // Free the memory associated with the chunk we just read:
    PNGChunk_free(chunk);
  }

  fclose(originalPng);
  fclose(gifFile);
  fclose(newPng);
  return 0;
}
