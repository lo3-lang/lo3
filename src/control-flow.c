#include "./internal/control-flow.h"
#include "./internal/core.h"
#include "./internal/specific-language.h"

lo3_label cf;
char rush = FALSE;
char isWarped = FALSE;
char *rush_target = NULL;

void cf_reset(void) {
	cf.nextFreePos = 0;
}

int cf_findLabel(const char *name) {

	for (int i = 0; i < cf.nextFreePos; i++) {
		if (cf.names[i] && !strcmp(name, cf.names[i])) {
			return i;
		}
	}
	return -1;
}

int cf_addLabel(const char *name, const int pos) {

	if (cf.nextFreePos == ARRAY_SIZE - 1) {
		return -1;
	}

	// is already known?
	int existing = cf_findLabel(name);
	if (existing != -1) {
		// Update position instead of erroring - rush mode jumped back here
		cf.pos[existing] = pos;
		return 0;
	}

	cf.names[cf.nextFreePos] = name;
	cf.pos[cf.nextFreePos] = pos;
	cf.nextFreePos++;
	return 0;
}

int cf_getPos(const char *name) {

	int index = cf_findLabel(name);

	if (index == -1) {
		return -1;
	}

	return cf.pos[index];
}

// important: this func should only be used intern, because it's not error proof
void cf_setCursorPos(const int i) {

	fseek(openFile, i, SEEK_SET);
}

int cf_getCursorPos(void) {
	return -1;
}

int cf_jumpToLabel(const char *name) {

	int index = cf_findLabel(name);

	if (index == -1) {
		lo3_error("Label not found!, Was this the right name?", name);
		return -1;
	}

	cf_setCursorPos(cf.pos[index]);
	return 0;
}
