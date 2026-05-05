// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "cli.h"
#include "./internal/core.h"
#include "version.h"
#include <stdio.h>
#include <string.h>

void cli_help(void) {
    printf("Usage: lo3 [OPTIONS] [FILE]\n\n");
    printf("  lo3 <FILE.lo3>                  Run a lo3 file\n");
    printf("  lo3 <FILE.LO3>                  Run with C preprocessor first\n");
    printf("  lo3 --cpp <FILE>                Run the C preprocessor on FILE before executing\n");
    printf("  lo3 <FILE> -o <NEWNAME>         Copy FILE to NEWNAME and execute NEWNAME\n");
    printf("  lo3 <FILE> --ignore-suffix      Execute FILE regardless of extension\n");
    printf("  lo3 --dry-run, -n               Read lo3 from stdin and execute\n");
    printf("  lo3 --version, -v               Show the current build version\n");
    printf("  lo3 --help, -h                  Show this help message\n");
}

void cli_version(void) {
    printf("lo3 version %s\n", LO3_VERSION);
}

int cli_parse(int argc, char *argv[], lo3_args *args) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            args->show_help = 1;

        } else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            args->show_version = 1;

        } else if (strcmp(argv[i], "--dry-run") == 0 || strcmp(argv[i], "-n") == 0) {
            args->dry_run = 1;

        } else if (strcmp(argv[i], "--ignore-suffix") == 0) {
            args->ignore_suffix = 1;

        } else if (strcmp(argv[i], "--cpp") == 0) {
            args->use_cpp = 1;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                args->input_file = argv[++i];
            } else {
                lo3_error("--cpp requires a filename", "");
                return -1;
            }

        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                args->output_file = argv[++i];
            } else {
                lo3_error("-o requires a new filename", "");
                return -1;
            }

        } else if (argv[i][0] != '-') {
            args->input_file = argv[i];

        } else {
            lo3_error("Unknown option", argv[i]);
            return -1;
        }
    }
    return 0;
}
