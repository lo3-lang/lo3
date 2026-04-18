#pragma once

// true and false so it is more fancy!
#define TRUE 1
#define FALSE 0
#define NOFREE -1

unsigned char rush = FALSE;
unsigned char isWarped = FALSE;

typedef struct {
	char *names[254] = {};

	// -1 tracks the next pos
	int pos[254];        // could be fixed by importing size_t
	int nextFreePos = 0; // if NOFREE it will lo3_error() that it is full!
} lo3_label;
