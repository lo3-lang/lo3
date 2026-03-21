// Copyright (c) 2026 seesee010
// All Rights Reserved.

#include "../include/core.h"

int main(int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(stderr, "[Error]: You must input more args then none\n");
		return 1;
	}

	FILE *file;
	if (pars_readFile(argv[1], &file)) {
		lo3_error("Could not load the corosponding file!", argv[1]);
		return 1;
	}

	fclose(file);
	return 0;
}
