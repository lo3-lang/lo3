#include "./internal/global.h"
#include "./internal/core.h"
#include <stdio.h>

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
