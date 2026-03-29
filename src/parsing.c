#include "../include/core.h"
#include "../include/var.h"

int currentLine = 0;

int pars_isFileValid(char *name, FILE **file) {

	*file = fopen(name, "r+");

	if (*file == NULL) {
		return -1;
	}

	size_t len = strlen(name);

	if (len < 4 || strcmp(&name[len - 4], ".lo3") != 0) {
		lo3_error("File must end with .lo3\n"
		          "But it will not stop...",
		          name);
		return -1;
	}

	return 0;
}

int pars_file(FILE *file) {

	char *line = NULL;
	size_t len = 0;
	char arg1[64], arg2[64];
	char buff_types[2];

	while (getline(&line, &len, file) != -1) {

		currentLine++;
		line[strcspn(line, "\n")] = '\0';

		if (line[0] != '#') {
			continue;
		}

		lo3_cmds cmds = (lo3_cmds)line[1];

		// find the TYPES of arg
		sscanf(&line[3], " %s %s", arg1, arg2);

		lo3_val a1 = pars_resv(arg1);
		lo3_val a2 = pars_resv(arg2);

		pars_getToKnowType(buff_types, a1, a2);
		pars_dispatch(cmds, a1, a2, buff_types);
	}

	free(line);
	return 0;
}

int pars_getToKnowType(char buffer[2], lo3_val val1, lo3_val val2) {

	/* types:
	 * 00|xx: num
	 * 01|xx: var
	 * 10|xx: array
	 * 11|xx: string
	 *
	 * Both sides have that syntax.
	 */

	char num[2];

	lo3_types possibleType[] = {val1.type, val2.type};
	lo3_val values[] = {val1, val2};

	// todo: num[] could be deleted
	// because num[] is an additional array, which is sth really not needed...
	// simply using buffer[] direct instead of num[].
	for (int i = 0; i < 2; i++) {
		switch (possibleType[i]) {

		case '$':
			num[i] = 0b0;
			break;

		case '%':
			num[i] = 0b01;
			break;

		case '*':
			num[i] = 0b10;
			break;

		case '_':
			num[i] = 0b11;
			break;

		default:
			lo3_error("Invalid Type of <STRING> found!", values[i].string);
			return 1;
			break;
		}
		buffer[i] = num[i];
	}
	return 0;
}

lo3_val pars_resv(char type[64]) {
	lo3_val result;
	result.type = type[0];

	switch (result.type) {

	// find the corresponding type
	case TYPE_num:

		result.num = atoi(&type[1]);
		break;

	case TYPE_array:

		// not avaible now
		break;

	case TYPE_string:

		result.string = type;
		break;

	case TYPE_var:

		// resolve var
		lo3_var *var = var_get(&type[1]);

		if (var == NULL) {
			lo3_error("Could not resolve var, because it was invalid! Or empty.",
			          &type[1]);
			break;
		}

		result.type = var->type;

		if (!var->type) {
			result.num = var->value.num;
		} else {
			result.string = var->value.string;
		}
		break;

	default:

		lo3_error("Could not fild the corresponding type! …\n Please enter something valid,"
		          "You may want to visit https://github.com/lo3-lang/learn-the-syntax !!!",
		          type);
		break;
	}
	return result;
}

void pars_dispatch(lo3_cmds cmd, lo3_val a1, lo3_val a2, char array[2]) {

	switch (cmd) {

	case BSC_asn:

		exec_asn(a1, a2, array);
		break;

	case ALU_add:

		exec_add(a1, a2, array);
		break;

	case ALU_sub:

		exec_sub(a1, a2, array);
		break;

	case ALU_mul:

		exec_mul(a1, a2, array);
		break;

	case ALU_div:

		exec_div(a1, a2, array);
		break;

	case CNT_jmp:

		exec_jmp(a1, a2, array);
		break;

	case CNT_call:

		exec_call(a1, a2, array);
		break;

	case CNT_callS:

		exec_callS(a1, a2, array);
		break;

	case CNT_label:

		exec_label(a1, a2, array);
		break;

	case CNT_new:
		exec_new(a1, a2, array);
		break;

	case STM_out:

		exec_out(a1, a2, array);
		break;

	case STM_in:

		exec_in(a1, a2, array);
		break;

	default:
		lo3_error("Unknown command!", NULL);
		break;
	}
}
