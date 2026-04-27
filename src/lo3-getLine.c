#include "./internal/core.h"

#ifdef _WIN32

int lo3_getLine(char **lineptr, size_t *n, FILE *stream) {
	if (*lineptr == NULL || *n == 0) {
		*n = 128;
		*lineptr = malloc(*n);
		if (*lineptr == NULL) return -1;
	}

	size_t pos = 0;
	int c;

	while ((c = fgetc(stream)) != EOF) {
		if (pos + 2 > *n) {
			*n *= 2;
			char *tmp = realloc(*lineptr, *n);
			if (tmp == NULL) return -1;
			*lineptr = tmp;
		}
		(*lineptr)[pos++] = (char)c;
		if (c == '\n') break;
	}

	if (pos == 0) return -1;

	(*lineptr)[pos] = '\0';
	return (int)pos;
}

#endif
