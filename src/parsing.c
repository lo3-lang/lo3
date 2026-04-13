// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.
//
// You can find todos by reading the "todo" blocks.
// But please read the whole block not only the single todo line
#include "./internal/bare-var.h"
#include "./internal/core.h"
#include "./internal/bare-define.h"
#include "./internal/specific-language.h"
#include "./internal/global.h"

int currentLine = 0;

int pars_isFileValid(char *name, FILE **file) {

	*file = fopen(name, "r");

	if (*file == NULL) {
		return -1;
	}

	size_t len = strlen(name);

	if (len < 4 || strcmp(&name[len - 4], ".lo3") != 0) {
		lo3_error("File must end with .lo3\n", name);
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

		if (strlen(line) < 3) {
			lo3_warn("You used some kind of magic line,"
			         "which is propaply not lo3-core syntax, are you sure you wanna do "
			         "this???",
			         "");
			continue;
		}

		lo3_cmds cmds = (lo3_cmds)line[1];

		// find the TYPES of arg
		(void)sscanf(&line[3], " %s %s", arg1, arg2);

		lo3_val a1 = pars_resv(arg1);
		lo3_val a2 = pars_resv(arg2);

		// todo:
		// delete the func pars_getToKnowType(...)
		//
		// ///// More Information /////
		// this func is not used anymore, because buff_types is ether.
		(void)pars_getToKnowType(buff_types, a1, a2);

		// todo:
		// parse that prefix away.
		//
		// ///// More Information: /////
		// here the program should parse the prefix, for example: '$' away.
		// So the Exec dont have to do that.
		int returnVal = pars_dispatch(cmds, a1, a2, buff_types);

		if (returnVal != -1) {
			return returnVal;
		}

		free(line);
		return 0;
	}
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

	// todo:
	// num[] could be deleted
	//
	// ///// More Information /////
	// Deleted because num[] is an additional array, which is not needed...
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
			lo3_error("Invalid Type of <STRING> found!", values[i].value.string);
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

		result.value.num = atoi(&type[1]);
		result.chooseType = 0;
		break;

	case TYPE_array:

		// logic:
		// *X, lookup X, resolve the value as long as it is a number,
		// 0 - 9 counts as values
		//
		// not allowed: "*A"
		
		// *100 -> _Hello
		int value = g_get(atoi(&tpye[1]));

		result.type = g_getType(value) ? TYPE_string : TYPE_num;
		result.value = g_getValue(value);
		result.chooseType = g_getType(value) ? 3 : 0;
		break;

	case TYPE_string:

		result.value.string = &type[1];
		result.chooseType = 3;
		break;

	case TYPE_var:

		// resolve var
		lo3_var *var = var_get(&type[1]);

		if (var == NULL) {
			lo3_error("Could not resolve var, because it was invalid! Or empty.", "");
			break;
		}

		result.type = var_getType(var) ? TYPE_string : TYPE_num;
		result.value = var_getValue(var);

		result.chooseType = var_getType(var) ? 3 : 0;

		// type: 0=num, 3=string (from ATYPE_ ... bitmasks, 1 and 2 are getting
		// resolved, so it would be useless if you can write them)

		// todo:
		// If double exists: var need to resolve this!
		break;

	case TYPE_double:

		// todo:
		// Add doubles in lo3
		//
		// ///// More Information: /////
		// Every exec_ will need other/more/etc code.
		break;
	default:

		lo3_error("Could not fild the corresponding type! …\n Please enter "
		          "something valid,"
		          "You may want to visit "
		          "https://github.com/lo3-lang/learn-the-syntax !!!",
		          type);
		break;
	}
	return result;
}

// todo:
// char array[2] should be deleted in every exec_...
//
// ///// More Information /////
// Now array is very useless and not very helping. Now lo3_val has chooseType to choose its
// type.
int pars_dispatch(lo3_cmds cmd, lo3_val a1, lo3_val a2, char array[2]) {

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
	https: // github.com/lo3-lang/lo3-core.git

	case CNT_free:
		exec_free(a1, a2, array);
		break;

	case STM_out:

		exec_out(a1, a2, array);
		break;

	case STM_in:

		exec_in(a1, a2, array);
		break;

	case RET_good:

		lo3_warn("Now it will stop the interpreter.\nEXITCODE: 0", "");
		return 0;

	case RET_bad:
		lo3_warn("Now it will stop the interpreter\nEXITCODE: 1", "");
		return 1;

	default:
		lo3_error("Unknown command!", "");
		break;
	}

	return -1;
}
