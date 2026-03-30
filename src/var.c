// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "../include/var.h"
#include "../include/core.h"
#include <stdlib.h>
#include <string.h>

lo3_varList *list = NULL;

void var_init(void) {

	list = malloc(sizeof(lo3_varList));
	list->index = 0;
}

int var_find(const char *name) {

	for (int i = 0; i < list->index; i++) {
		if (strcmp(list->array[i]->name, name) == 0) {
			return i;
		}
	}
	return -1;
}

void var_create(const char *name, int type) {

	if (name == NULL || strcmp(name, "") == 0) {
		lo3_error("Name is invalid, Please check your code!", name);
		return;
	}

	if (list->index >= 256) {
		lo3_error("Max. variables reached! Please free() any var before define any "
		          "again!",
		          name);
		return;
	}

	// create new var
	lo3_var *var = malloc(sizeof(lo3_var));
	strncpy(var->name, name, 64);
	var->type = type;

	// save it
	list->array[list->index] = var;
	list->index++;
}

void var_set(const char *name, lo3_var value) {

	int index = var_find(name);

	if (index == -1) {
		lo3_error("Could not find var, did you write it correctly?", name);
		return;
	}

	// if there is a assignation (again), then it will work...
	if (list->array[index]->type == 3 && list->array[index]->value.string != NULL) {
		free(list->array[index]->value.string);
		list->array[index]->value.string = NULL;
	}

	if (!value.type) {

		list->array[index]->value.num = value.value.num;
		list->array[index]->type = 0;
	} else {
		// var is available outside of the stack; -> heap; // don't forget to free() mem!
		list->array[index]->value.string = strdup(value.value.string);
		list->array[index]->type = 3;
	}
}

lo3_var *var_get(const char *name) {

	int index = var_find(name);
	if (index == -1) {

		lo3_error("Invalid <var> name, is this the right name?", name);
		return NULL;
	}

	return list->array[index];
}

/*
 * algo
 * designed to:
 * 1. delete on index {returnVal} (/free)
 * 2. put val index-1 in index {returnVal}
 * index-- so index can be used again.
 *
 * Why?:
 * This is designed as it is, because so it is a fast and short algo.
 * O(1) complexity !!!
 */
void var_free(const char *name) {

	int returnVal = var_find(name);

	if (returnVal != -1) {
		free(list->array[returnVal]);
	} else {
		lo3_error(
		    "Could not free <var>. Is there any change that the var has an other name?",
		    name);
		return;
	}

	if (list->array[returnVal]->type == 3) {
		free(list->array[returnVal]->value.string);
	}
	free(list->array[returnVal]);

	list->array[returnVal] = list->array[list->index - 1];
	list->index--;
}

void var_freeAll(void) {

	for (int i = 0; i < list->index; i++) {
		if (list->array[i]->type == 3)
			free(list->array[i]->value.string);
		free(list->array[i]);
	}

	free(list);
	list = NULL;
}
