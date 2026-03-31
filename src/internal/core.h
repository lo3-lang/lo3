// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#pragma once

#include <stdio.h>
#include <stdlib.h>

int pars_isFileValid(char *name, FILE **file);
int pars_file(FILE *file);

// colorcodes
#define CLR_RED "\033[31m"
#define CLR_YELLOW "\033[33m"
#define CLR_RESET "\033[0m"
#define CLR_BOLD "\033[1m"

void lo3_warn(const char *msg, const char *context);
void lo3_error(const char *msg, const char *context);

///// from parsing.c /////
extern int currentLine;

///// from var.c /////
void var_init(void); // just exist here cuz: main.c is in need of this func.
void var_freeAll(void);
