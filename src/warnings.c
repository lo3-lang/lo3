// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "./internal/core.h"
#include <stdarg.h>

void lo3_warn(const char *msg, const char *context) {

	if (msg == NULL || msg[0] == '\0') {
		lo3_error("Could not warn the shit out it!", context);
		return;
	}

	if (context != NULL) {
		(void)fprintf(
		    stderr, CLR_BOLD CLR_YELLOW "[WARNING]: %s " CLR_RESET "(?: '%s', line: %d)\n",
		    msg, context, currentLine);
	} else {
		(void)fprintf(stderr, CLR_BOLD CLR_YELLOW "[WARNING]: %s \n" CLR_RESET, msg);
	}
}

void lo3_error(const char *msg, const char *context) {

	if (msg == NULL || msg[0] == '\0') {
		lo3_warn("Could not error, because there was no error message for the error", NULL);
		return;
	}

	if (context != NULL) {
		(void)fprintf(stderr,
		              CLR_BOLD CLR_RED "[ERROR]: %s " CLR_RESET "(?: '%s', line: %d)\n",
		              msg, context, currentLine);
	} else {
		(void)fprintf(stderr, CLR_BOLD CLR_RED "[ERROR]: %s \n" CLR_RESET, msg);
	}
}

void lo3_printD(const char *fmt, ...) {
	if (fmt == NULL) {
		return;
	}
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
