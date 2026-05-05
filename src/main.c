// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "./internal/cf-define.h"
#include "./internal/core.h"
#include "internal/bare-var.h"
#include "version.h"
#include <stdio.h>

FILE *openFile = NULL;
int main(int argc, char *argv[]) {

	if (argc < 2) {
		lo3_error("Please provide more arguments!", "");
		return 0;
	}

	if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
		lo3_printf("lo3 version %s\n", LO3_VERSION);
		return 0;
	}

	FILE *file;
	if (pars_isFileValid(argv[1], &file)) {
		lo3_error("Could not load the coresponding file!", argv[1]);
		(void)(fclose(file));
		return 1;
	}

	openFile = file;

	var_init();
	pars_file(file);

	// cleanup
	(void)fclose(file);
	var_freeAll();
	return 0;
}
