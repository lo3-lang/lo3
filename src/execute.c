// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "./internal/core.h"
#include "./internal/specific-language.h"
#include "./internal/var.h"

void exec_new(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64];
	char *name;

	if (!a1.chooseType) {

		snprintf(buf, sizeof(buf), "%d", a1.value.num);
		lo3_warn("Are you sure you want to set the name as a number?", buf);
		name = buf;
	} else {
		name = a1.value.string;
	}

	// get the type
	int type = (!a2.chooseType) ? a2.value.num : atoi(a2.value.string);

	// duplication check
	if (var_find(name) != -1) {
		lo3_error("Variable already exists!", name);
		return;
	}

	if (type != 0 && type != 3) {

		snprintf(buf, sizeof(buf), "%d", type);
		lo3_warn("Variable type might not be num or string!", buf);
	}

	var_create(name, type);
}

void exec_free(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64];
	char *name;

	if (!a1.chooseType) {
		snprintf(buf, sizeof(buf), "%d", a1.value.num);
		lo3_warn("Are you sure you want to delete some var called:\nDoing it anyways... ",
		         buf);
		name = buf;
	} else {
		name = a1.value.string;
	}

	// ignore arg1

	var_free(name);
}

/*
 * algo:
 * if the type of a1 is the same type, else lo3_error.
 * assign a1 = a2; if everything is correct!
 *
 * what is ok?:
 * string as name, num as name
 */
void exec_asn(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64], *name;
	char numNameBuf[64];

	if (!a1.chooseType) {
		snprintf(buf, sizeof(buf), "%d", a1.value.num);
		name = buf;

	} else {
		name = a1.value.string;
	}

	// var_get() would also work here, but it is just a wrapper around var_find()
	if (var_find(name) == -1) {
		lo3_error("Could not find <var> please check if it really exists!", "");
		return;
	}

	lo3_var newVar;
	newVar.type = (a2.chooseType == 3) ? 3 : 0;

	newVar.value = a2.value;

	var_set(name, newVar);
}

///// alu /////

void exec_add(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64];
	char *name;

	if (!a1.chooseType) {
		snprintf(buf, sizeof(buf), "%d", a1.value.num);
		name = buf;

	} else {
		name = a1.value.string;
	}

	if (var_find(name) == -1) {
		lo3_error("Could not find this var, please check if you got something wrong", name);
		return;
	}

	lo3_var *oldVar = var_get(name); // it must be right now, else var_find() was wrong before!

	if (a2.chooseType) {
		lo3_error("Arg1 requires TYPE String for +=", "");
		return;
	}

	oldVar->value.num += a2.value.num;
	var_set(name, *oldVar);
}

void exec_sub(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64];
	char *name;

	if (!a1.chooseType) {
		snprintf(buf, sizeof(buf), "%d", a1.value.num);
		name = buf;

	} else {
		name = a1.value.string;
	}

	if (var_find(name) == -1) {
		lo3_error("Could not find this var, please check if you got something wrong", name);
		return;
	}

	lo3_var *oldVar = var_get(name); // it must be right now, else var_find() was wrong before!

	if (a2.chooseType) {
		lo3_error("Arg1 requires TYPE String for +=", "");
		return;
	}

	oldVar->value.num -= a2.value.num;
	var_set(name, *oldVar);
}

void exec_mul(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64];
	char *name;

	if (!a1.chooseType) {
		snprintf(buf, sizeof(buf), "%d", a1.value.num);
		name = buf;

	} else {
		name = a1.value.string;
	}

	if (var_find(name) == -1) {
		lo3_error("Could not find this var, please check if you got something wrong", name);
		return;
	}

	lo3_var *oldVar = var_get(name); // it must be right now, else var_find() was wrong before!

	if (a2.chooseType) {
		lo3_error("Arg1 requires TYPE String for +=", "");
		return;
	}

	oldVar->value.num *= a2.value.num;
	var_set(name, *oldVar);
}

void exec_div(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64];
	char *name;

	if (!a1.chooseType) {
		snprintf(buf, sizeof(buf), "%d", a1.value.num);
		name = buf;

	} else {
		name = a1.value.string;
	}

	if (var_find(name) == -1) {
		lo3_error("Could not find this var, please check if you got something wrong", name);
		return;
	}

	lo3_var *oldVar = var_get(name); // it must be right now, else var_find() was wrong before!

	if (a2.chooseType) {
		lo3_error("Arg1 requires TYPE String for +=", "");
		return;
	}

	oldVar->value.num /= a2.value.num;
	var_set(name, *oldVar);
}

void exec_jmp(lo3_val a1, lo3_val a2, char array[2]) {
}

void exec_call(lo3_val a1, lo3_val a2, char array[2]) {
}

void exec_callS(lo3_val a1, lo3_val a2, char array[2]) {
}

void exec_label(lo3_val a1, lo3_val a2, char array[2]) {
}

void exec_out(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64];
	char *name;

	if (!a1.chooseType) {
		snprintf(buf, sizeof(buf), "%d", a1.value.num);
		name = buf;
	} else {
		name = a1.value.string;
	}

	printf("%s\n", name);
}

void exec_in(lo3_val a1, lo3_val a2, char array[2]) {

	char buf[64]; // max limit of pars_resv();
	char *res;

	char numNameBuf[64];
	char *name;

	res = fgets(buf, sizeof(buf), stdin);

	if (res == NULL) {
		lo3_error("Something went wrong while reading the stdin stream.\n"
		          "Please check if you done anything wrong...",
		          buf);
		return;
	}

	// parse the '\n' away
	buf[strcspn(buf, "\n")] = '\0';

	lo3_val temp = pars_resv(buf);

	// transform lo3_val -> lo3_var
	lo3_var newVar;
	newVar.type = temp.chooseType;

	newVar.value = temp.value;

	// correct the a1.~type
	if (!a1.chooseType) {
		snprintf(numNameBuf, sizeof(numNameBuf), "%d", a1.value.num);
		name = numNameBuf;

	} else {
		name = a1.value.string;
	}

	var_set(name, newVar);
}
