#include "../include/core.h"

int pars_readFile(char *name, FILE **file) {

	*file = fopen(name, "r+");

	if (*file == NULL) {
		return -1;
	}

	return 0;
}
