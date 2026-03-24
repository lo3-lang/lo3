#pragma once

#include <string.h>

typedef enum {

	TYPE_num = '$',
	TYPE_string = '_',
	TYPE_array = '*',
	TYPE_var = '%'
} lo3_types;

typedef struct {

	lo3_types type;
	int num;
	char *string;
} lo3_val;

typedef enum {

	// assign
	BSC_asn = '=',

	// alu
	ALU_add = '+',
	ALU_sub = '-',
	ALU_div = '/',
	ALU_mul = '*',

	// control
	CNT_jmp = 'd',
	CNT_call = 'c',
	CNT_callS = 'C',
	CNT_label = 'l',

	// stream
	STM_out = '>',
	STM_in = '<',
} lo3_cmds;

typedef struct {

	lo3_types type;
	char name[64];
} lo3_var;

lo3_val pars_resv(char type[64]);
void pars_dispatch(lo3_cmds cmd, lo3_val a1, lo3_val a2);

// execute func
void exec_asn(lo3_val a1, lo3_val a2);
void exec_add(lo3_val a1, lo3_val a2);
void exec_sub(lo3_val a1, lo3_val a2);
void exec_mul(lo3_val a1, lo3_val a2);
void exec_div(lo3_val a1, lo3_val a2);
void exec_jmp(lo3_val a1, lo3_val a2);
void exec_call(lo3_val a1, lo3_val a2);
void exec_callS(lo3_val a1, lo3_val a2);
void exec_label(lo3_val a1, lo3_val a2);
void exec_out(lo3_val a1, lo3_val a2);
void exec_in(lo3_val a1, lo3_val a2);
