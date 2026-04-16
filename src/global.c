#include "./internal/global.h"
#include "./internal/core.h"
#include <stdio.h>

// 0: $100, chooseType = 0;

struct lo3_g {
	lo3_val value[G_SIZE];
};

lo3_g g;

lo3_val g_get(int index) {

	char buf[64];
	snprintf(buf, sizeof(buf), "%d", index);

	if (index > 100 || index < 0) {
		lo3_error("Could not return any g[index],\n"
		          "This is Out of bounce!",
		          buf);
		return g.value[0];
	}
	return g.value[index];
}
void g_set(int index, lo3_val value) {

	char buf[64];
	snprintf(buf, sizeof(buf), "%d", index);

	if (index > 100 || index < 0) {
		lo3_error("Could not set any <g>.\n"
		          "Invalid index for g[]!",
		          buf);
		return;
	}

	if (g.value[index] == NULL) {
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

		g.value[index].chooseType = 0;//type, replace 0 with the coresponding type!
	}
	g.value[index] = value;
}

int g_isSet(int index) {

	char buf[64];
	snprintf(buf, sizeof(buf), "%d", index);

	if (index > 100 || index < 0) {
		lo3_error("Could not prove any <g>.\n"
		          "Invalid index for g[]!",
		          buf);
		return -1;
	}

	if (!g.value[index].chooseType) {
		if (g.value[index].value.num == 0 || g.value[index].value.num == ' ') {
			return 0;
		}

	} else {
		if (g.value[index].value.string == NULL) {
			return 0;
		}
	}

	return 1;
}

int g_getType(int index) {

	lo3_val buf;

	if (index > 100 || index < 0) {
		buf.chooseType = -1;
		return buf;
	}
	
	return  g.value[index].chooseType;

}

int g_setType(int index, lo3_val type) {

	lo3_val buf;

	if (index > 100 || index < 0) {
		buf.chooseType = -1;
		lo3_error("setting Type: Wrong Index, you input any out of bounce!", "");
		return -1;

	}

	if (type.chooseType > 3  || type.chooseType < 0) {
		buf.chooseType = -1;
		lo3_error("setting Type: Wrong datatype, what are you doing?", "");
		return -1;
	}

	g.value[index].chooseType = type.chooseType;

	return 0;
}

lo3_val g_getValue(int index) {

}
