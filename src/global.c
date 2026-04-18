#include "./internal/global.h"
#include "./internal/core.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

// 0: $100, chooseType = 0;

struct lo3_g {
	lo3_val value[G_SIZE];
	int isSet[G_SIZE];
};

lo3_g g;

lo3_val g_get(int index) {

	char buf[64];
	snprintf(buf, sizeof(buf), "%d", index);

	if (index > G_SIZE - 1 || index < 0) {
		lo3_error("Could not return any g[index],\n"
		          "This is Out of bounce!",
		          buf);
		return g.value[0];
	}
	return g.value[index];
}

// todo: add the way to check for isSet[index]
// NOTE: it could be fixed already ....!
//
// ///// More Informations /////
// there is a array called isSet, the purpose is to
// check if any of the indexes are already set or not set at all.
// This is needed because isSet() func, will use them to check if g.value[]
// is already set or not!
//
// Why like that?
// /////
// Yes. Everyone who whould say that this second array is not needed
// is right. It could be inpli as lo3_val *value[G_SIZE]
// But now it whould break to much, so I decided to create a second array
// and setting it the way it is.
void g_set(int index, lo3_val value) {

	char buf[64];
	snprintf(buf, sizeof(buf), "%d", index);

	if (index > G_SIZE - 1 || index < 0) {
		lo3_error("Could not set any <g>.\n"
		          "Invalid index for g[]!",
		          buf);
		return;
	}

	if (g.isSet[index] == 0) {
		// todo:
		// inpliment that feature
		//
		// ///// More Informations /////
		// Look at the first char of value,
		// if [0] is $ -> type 0
		// if [0] is _ -> type type 3
		// if [0] is / -> type 2
		// else Invalid
		// the whole with switch cases !

		g.value[index].chooseType = 0; // type, replace 0 with the coresponding type!
		g.isSet[index] = 1;
	}
	g.value[index] = value;
}

// todo: add the way to check for isSet[index]
//
// ///// More Informations /////
// there is a array called isSet, the purpose is to
// check if any of the indexes are already set or not set at all.
// This is needed because isSet() func, will use them to check if g.value[]
// is already set or not!
//
// Why like that?
// /////
// Yes. Everyone who whould say that this second array is not needed
// is right. It could be inpli as lo3_val *value[G_SIZE]
// But now it whould break to much, so I decided to create a second array
// and setting it the way it is.
int g_isSet(int index) {

	char buf[64];
	snprintf(buf, sizeof(buf), "%d", index);

	if (index > G_SIZE - 1 || index < 0) {
		lo3_error("OOB: <g>.\n"
		          "Invalid index for g[]!",
		          buf);
		return -1;
	}

	// 0: is not set / 1: is set
	return g.isSet[index];
}

int g_getType(int index) {
	if (index >= G_SIZE || index < 0) {
		return -1;
	}
	return g.value[index].chooseType;
}

int g_setType(int index, lo3_val type) {

	lo3_val buf;

	if (index > G_SIZE - 1 || index < 0) {
		buf.chooseType = -1;
		lo3_error("setting Type: Wrong Index, you input any out of bounce!", "");
		return -1;
	}

	if (type.chooseType > 3 || type.chooseType < 0) {
		buf.chooseType = -1;
		lo3_error("setting Type: Wrong datatype, what are you doing?", "");
		return -1;
	}

	g.value[index].chooseType = type.chooseType;

	return 0;
}

// todo: inpliment g_getValue
// NOTE: WIP — not implemented yet!
//
// ///// More Informations /////
// This function should return the lo3_val stored at g.value[index].
// For now it returns a placeholder (chooseType = -1) to avoid
// undefined behavior from an empty function body.
// /////
lo3_val g_getValue(int index) {
	lo3_val placeholder;
	placeholder.chooseType = -1;
	return placeholder;
}

// NOTE: this func should not be used by any seriously codebase, this func
// can selfish not fully cover if it is OOB or if the value is even an integer.
// PLEASE:
// Consider using the func g_getValue() even if you then must check which type it is by yourself!
// if you really want to use this func, use the MACRO called G_GETV(index), this will be replaced by
// g_getNum(...) or g_getString(...)
int g_getNum(int index) {
	if (index >= G_SIZE || index < 0) {
		return -1; // could also eventually be data!!!
	}

	return g.value[index].value.num;
}

// NOTE: this func should not be used by any seriously codebase, this func
// can selfish not fully cover if it is OOB or if the value is even a String.
// PLEASE:
// Consider using the func g_getValue() even if you then must check which type it is by yourself!
// if you really want to use this func, use the MACRO called G_GETV(index), this will be replaced by
// g_getNum(...) or g_getString(...) !!!
char *g_getString(int index) {
	if (index >= G_SIZE || index < 0) {
		return "_error"; // could also eventually be data!!!
	}

	return g.value[index].value.string;
}
