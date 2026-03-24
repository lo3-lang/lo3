#include "../include/core.h"
#include "../include/specific-language.h"
#include <stdio.h>

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

	while (getline(&line, &len, file) != -1) {
		line[strcspn(line, "\n")] = '\0';

		if (line[0] != '#')
			continue;

		for (int i = 1; line[i] != '\0'; i++) {
			lo3_cmds cmds = (lo3_cmds)line[1];

			// find the TYPES of arg
			sscanf(&line[3], " %s %s", arg1, arg2);

			lo3_val a1 = pars_resv(arg1);
			lo3_val a2 = pars_resv(arg2);

			pars_dispatch(cmds, a1, a2);
		}
	}
	return 0;
}

lo3_val pars_resv(char type[64]) {
	lo3_val result;
	result.type = type[0];

	switch (result.type) {

	// find the corresponding type
	case TYPE_num:

		result.num = *(int *)type;
		break;

	case TYPE_array:

		// not avaible now
		break;

	case TYPE_string:

		result.string = type;
		break;

	case TYPE_var:

		// not avaible now
		break;

	default:

		lo3_error("Could not fild the corresponding type! …\n Please enter something valid,"
		          "You may want to visit https://github.com/lo3-lang/learn-the-syntax !!!",
		          type);
		break;
	}
	return result;
}

void pars_dispatch(lo3_cmds cmd, lo3_val a1, lo3_val a2) {

	switch (cmd) {

	case BSC_asn:

		exec_asn(a1, a2);
		break;

	case ALU_add:

		exec_add(a1, a2);
		break;

	case ALU_sub:

		exec_sub(a1, a2);
		break;

	case ALU_mul:

		exec_mul(a1, a2);
		break;

	case ALU_div:

		exec_div(a1, a2);
		break;

	case CNT_jmp:

		exec_jmp(a1, a2);
		break;

	case CNT_call:

		exec_call(a1, a2);
		break;

	case CNT_callS:

		exec_callS(a1, a2);
		break;

	case CNT_label:

		exec_label(a1, a2);
		break;

	case STM_out:

		exec_out(a1, a2);
		break;

	case STM_in:

		exec_in(a1, a2);
		break;

	default:
		lo3_error("Unknown command!", NULL);
		break;
	}
}
