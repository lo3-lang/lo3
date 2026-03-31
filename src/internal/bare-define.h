#pragma once

typedef enum {

	TYPE_num = '$',
	TYPE_string = '_',
	TYPE_array = '*',
	TYPE_var = '%'
} lo3_types;

typedef union {
	int num;
	char *string;
} lo3_value;

typedef struct {

	lo3_types type;
	int chooseType;

	lo3_value value;

} lo3_val;
