// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "./internal/core.h"

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

void lo3_printf(const char *fmt, ...) {

	if (fmt == NULL || fmt[0] == '\0') {
		lo3_error("lo3_printf called with empty format!", "");
		return;
	}

	va_list args;
	va_start(args, fmt);
	int ret = vfprintf(stdout, fmt, args);
	va_end(args);

	if (ret < 0) {
		lo3_error("Failed to write output!", "");
	}
}
