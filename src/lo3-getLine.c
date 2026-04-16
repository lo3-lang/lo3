#include "./internal/core.h"

lo3_getLine(&(line), &(len), (file)) {
	int *buf = malloc(sizeof(int[256]));

	if (buf == NULL) {
		return -2;
	}
	free(buf);
}
