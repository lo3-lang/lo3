// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "./internal/core.h"

int main(int argc, char *argv[]) {

	if (argc < 2) {
		lo3_error("You must input more args then none!", NULL);
		return 1;
	}

	FILE *file;
	if (pars_isFileValid(argv[1], &file)) {
		lo3_error("Could not load the coresponding file!", argv[1]);
		return 1;
	}

	var_init();
	pars_file(file);

	// cleanup
	(void)fclose(file);
	var_freeAll();
	return 0;
}
