#pragma once

#include "bare-define.h"

#define G_SIZE 100
typedef struct {
	lo3_val g[G_SIZE];
} lo3_g;

///// from g.c /////
void g_set(int index, lo3_val value);
lo3_val g_get(int index);
void g_isSet(int index);
