#include "./internal/core.h"
#ifdef _WIN32

ssize_t lo3_getLine(char **lineptr, size_t *n, FILE *stream) {

	if (lineptr == NULL || n == NULL || stream == NULL) {
		return -1;
	}

	if (*lineptr == NULL || *n == 0) {

		*n = 128;
		*lineptr = malloc(*n);
		if (*lineptr == NULL) {
			return -1;
		}
	}

	int pos = 0;
	int c;
	while ((c = fgetc(stream)) != EOF) {
		
		if (pos + 1 >= *n) {

			size_t new_size = *n * 2;
			char *tmp = realloc(*lineptr, new_size);
			if (tmp == NULL) {
				return -1;
			}
			
			*lineptr = tmp;
			*n = new_size;
		}

		(*lineptr)[pos++] = (char)c;
		if (c == '\n') {
			break;
		}
	}

	if (pos == 0) {
		return -1;
	}

	(*lineptr)[pos] = '\0';
	return (ssize_t)pos;
}
#endif
