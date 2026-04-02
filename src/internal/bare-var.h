// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#pragma once

#include "specific-language.h"

typedef struct lo3_var lo3_var;

typedef struct lo3_varList lo3_varList;

///// from var.c /////
void var_create(const char *name, int type);
void var_free(const char *name);
int var_find(const char *name);

lo3_var *var_get(const char *name);
int var_getType(const lo3_var *var);
lo3_value var_getValue(const lo3_var *var);
int var_getNum(const lo3_var *var);
const char *var_getString(const lo3_var *var);

void var_setNum(const char *name, int num);
void var_setString(const char *name, char *string);
