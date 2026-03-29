// Copyright (c) 2026 seesee010
// All Rights Reserved.

#include "../include/core.h"

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

	pars_file(file);
	fclose(file);
	return 0;
}
