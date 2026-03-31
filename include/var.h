// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#pragma once

#include "specific-language.h"

typedef struct {

	char name[64];
	int type; // 0 = num, 3 = string

	lo3_value value;
} lo3_var;

typedef struct {

	int index;
	lo3_var *array[256];
} lo3_varList;

lo3_var *var_get(const char *name);

///// from var.c /////
void var_create(const char *name, int type);
void var_set(const char *name, lo3_var value);
void var_free(const char *name);
int var_find(const char *name);
