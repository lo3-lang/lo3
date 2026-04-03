// Copyright (c) 2026 seesee010.
// see License file for more informations

/*
 * For all other plugins!
 */

// important:

/*
 * Use this as your header for any plugin you create.
 * this header file already uses everything YOU need to have.
 */

#pragma once

///// bare-define.h
typedef enum {

	TYPE_num = '$',
	TYPE_string = '_',
	TYPE_array = '*',
	TYPE_var = '%',
	TYPE_double = '/'
} lo3_types;

typedef union {
	int num;
	char *string;
	double numD;
} lo3_value;

typedef struct {

	lo3_types type;
	int chooseType; // 0(=num), 2(=double), 3(=string)

	lo3_value value;

} lo3_val;

///// additional-naming.h /////

// just some values for the exec,
// to not use some random values.
#define ATYPE_num 0b00
#define ATYPE_var 0b01
#define ATYPE_array 0b10
#define ATYPE_string 0b11

// sleep(ms) macro
#ifdef _WIN32
#inlcude < windows.h>
#define ASLEEP(ms) Sleep(ms);

#else // linux
#include <unistd.h>
#define ASLEEP(ms) slee((ms) * 1000) // mic sec -> ms

#endif

///// bare-var.h /////
#pragma once

// #include "specific-language.h"

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

///// global.h /////

// #include "bare-define.h"

#define G_SIZE 100
typedef struct lo3_g lo3_g;

// getter and setter
lo3_val g_get(int index);
void g_set(int index, lo3_val value);
int g_isSet(int index);
