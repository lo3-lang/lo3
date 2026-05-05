// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int pars_isFileValid(char *name, FILE **file);
int pars_file(FILE *file);

// colorcodes
#define CLR_RED "\033[31m"
#define CLR_YELLOW "\033[33m"
#define CLR_RESET "\033[0m"
#define CLR_BOLD "\033[1m"

#if defined(_MSC_VER)
  typedef long ssize_t;

#endif

#ifdef _WIN32
ssize_t lo3_getLine(char **lineptr, size_t *n, FILE *stream);

#define GETLINE(line, len, file) \
	(lo3_getLine((line), (len), (file)) != -1)

#elif __linux__
#define GETLINE(line, len, file) \
	(getline((line), (len), (file)) != -1)

#endif

void lo3_warn(const char *msg, const char *context);
void lo3_error(const char *msg, const char *context);
void lo3_printf(const char *fmt, ...);

///// from parsing.c /////
extern int currentLine;

///// from var.c /////
void var_init(void); // just exist here cuz: main.c is in need of this func.
void var_freeAll(void);
