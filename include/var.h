#pragma once

#include "specific-language.h"

typedef struct {

	char name[64];
	int type; // 0 = num, 3 = string

	union {
		int num;
		char *string;
	} value;
} lo3_var;

typedef struct {

	int index;
	lo3_var *array[256];
} lo3_varList;

lo3_var *var_get(const char *name);
