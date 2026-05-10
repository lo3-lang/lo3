// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "./internal/cf-define.h"
#include "./internal/core.h"
#include "internal/bare-var.h"
#include "internal/cli.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

FILE *openFile = NULL;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cli_help();
        return 1;
    }

    lo3_args args = {0};
    if (cli_parse(argc, argv, &args) != 0) {
        return 1;
    }

    switch (cli_get_mode(&args)) {
        case MODE_HELP:
            cli_help();
            return 0;

        case MODE_VERSION:
            cli_version();
            return 0;

        case MODE_DRY_RUN: {
            char dry_tmp[256];
            int dry_fd = -1;
            if (cli_make_tmp(dry_tmp, sizeof(dry_tmp), "dry", &dry_fd) != 0) {
                lo3_error("Could not create temp file for dry-run!", "");
                return 1;
            }
            PRINTLI("lo3 dry-run: enter lo3 code (Ctrl+D when done):");
            FILE *tmp = fdopen(dry_fd, "w");
            if (!tmp) {
                lo3_error("Could not create temp file for dry-run!", dry_tmp);
                if (dry_fd >= 0) {
#ifdef _WIN32
                    _close(dry_fd);
#else
                    close(dry_fd);
#endif
                }
                return 1;
            }
            char *line = NULL;
            size_t len = 0;
            while (GETLINE(&line, &len, stdin)) {
                fputs(line, tmp);
                free(line);
                line = NULL;
                len = 0;
            }
            fclose(tmp);
            FILE *file = fopen(dry_tmp, "r");
            if (!file) {
                lo3_error("Could not open dry-run temp file!", dry_tmp);
                remove(dry_tmp);
                return 1;
            }
            openFile = file;
            var_init();
            pars_file(file);
            fclose(file);
            var_freeAll();
            remove(dry_tmp);
            return 0;
        }

        case MODE_NORMAL:
        default:
            break;
    }

    if (!args.input_file) {
        lo3_error("Please provide a file!", "");
        return 1;
    }

    if (cli_has_upper_ext(args.input_file)) {
        args.use_cpp = 1;
    }

    const char *run_file = args.input_file;
    char cpp_tmp[256] = {0};
    int cpp_fd = -1;
    int ret = 1;

    if (args.use_cpp) {
        if (cli_make_tmp(cpp_tmp, sizeof(cpp_tmp), "cpp", &cpp_fd) != 0) {
            lo3_error("Could not create temp file for cpp!", args.input_file);
            return 1;
        }
        // Close the fd since cli_run_cpp will write to the file by path
        if (cpp_fd >= 0) {
#ifdef _WIN32
            _close(cpp_fd);
#else
            close(cpp_fd);
#endif
            cpp_fd = -1;
        }
        if (cli_run_cpp(args.input_file, cpp_tmp) != 0) {
            lo3_error("C preprocessor failed!", args.input_file);
            goto cleanup;
        }
        run_file = cpp_tmp;
    }

    if (args.output_file) {
        if (cli_copy_file(run_file, args.output_file) != 0) {
            lo3_error("Could not copy the file!", args.output_file);
            goto cleanup;
        }
        run_file = args.output_file;
    }

    FILE *file;
    switch (args.ignore_suffix) {
        case 1:
            file = fopen(run_file, "r");
            if (!file) {
                lo3_error("Could not load the corresponding file!", run_file);
                goto cleanup;
            }
            break;
        default:
            if (pars_isFileValid((char *)run_file, &file) != 0) {
                goto cleanup;
            }
            break;
    }

    openFile = file;
    var_init();
    pars_file(file);
    fclose(file);
    var_freeAll();
    ret = 0;

cleanup:
    if (cpp_tmp[0]) {
        remove(cpp_tmp);
    }
    return ret;
}
