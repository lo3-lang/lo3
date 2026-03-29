#include "../include/var.h"
#include "../include/core.h"
#include <stdlib.h>

lo3_var var;
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

	if (strcmp(name, "") != 0 || name == NULL) {
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
	if (!value.type) {

		list->array[index]->value.num = value.value.num;
		list->array[index]->type = 0;
	} else {
		list->array[index]->value.string = value.value.string;
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
