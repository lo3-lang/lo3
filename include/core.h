#pragma once

#include <stdio.h>
int pars_readFile(char *name, FILE **file);

// colorcodes
#define CLR_RED "\033[31m"
#define CLR_YELLOW "\033[33m"
#define CLR_RESET "\033[0m"
#define CLR_BOLD "\033[1m"

void lo3_warn(const char *msg, const char *context);
void lo3_error(const char *msg, const char *context);
