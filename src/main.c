// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "./internal/cf-define.h"
#include "./internal/core.h"
#include "internal/bare-var.h"
#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <process.h>
#define getpid _getpid
#else
#include <unistd.h>
#endif

FILE *openFile = NULL;

static int copy_file(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");
    if (!in) return -1;
    FILE *out = fopen(dst, "wb");
    if (!out) { fclose(in); return -1; }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
        fwrite(buf, 1, n, out);
    fclose(in);
    fclose(out);
    return 0;
}

static int has_upper_lo3_suffix(const char *name) {
    size_t len = strlen(name);
    return len >= 4 && strcmp(&name[len - 4], ".LO3") == 0;
}

static void make_tmp_path(char *buf, size_t size, const char *prefix) {
#ifdef _WIN32
    const char *tmp_dir = getenv("TEMP");
    if (!tmp_dir) tmp_dir = "C:\\Temp";
    snprintf(buf, size, "%s\\lo3_%s_%d.lo3", tmp_dir, prefix, (int)getpid());
#else
    snprintf(buf, size, "/tmp/lo3_%s_%d.lo3", prefix, (int)getpid());
#endif
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cli_help();
        return 1;
    }

    lo3_args args = {0};
    if (cli_parse(argc, argv, &args) != 0)
        return 1;

    if (args.show_help) {
        cli_help();
        return 0;
    }

    if (args.show_version) {
        cli_version();
        return 0;
    }

    // dry-run: read from stdin into a temp file, then execute it
    if (args.dry_run) {
        char dry_tmp[256];
        make_tmp_path(dry_tmp, sizeof(dry_tmp), "dry");

        printf("lo3 dry-run: enter lo3 code (Ctrl+D when done):\n");

        FILE *tmp = fopen(dry_tmp, "w");
        if (!tmp) {
            lo3_error("Could not create temp file for dry-run!", dry_tmp);
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

    if (!args.input_file) {
        lo3_error("Please provide a file!", "");
        return 1;
    }

    // .LO3 (uppercase extension) automatically uses the C preprocessor
    if (has_upper_lo3_suffix(args.input_file))
        args.use_cpp = 1;

    const char *run_file = args.input_file;
    char cpp_tmp[256] = {0};

    if (args.use_cpp) {
        make_tmp_path(cpp_tmp, sizeof(cpp_tmp), "cpp");
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "cpp -P \"%s\" -o \"%s\"", args.input_file, cpp_tmp);
        if (system(cmd) != 0) {
            lo3_error("C preprocessor failed!", args.input_file);
            return 1;
        }
        run_file = cpp_tmp;
    }

    if (args.output_file) {
        if (copy_file(run_file, args.output_file) != 0) {
            lo3_error("Could not copy the file!", args.output_file);
            if (cpp_tmp[0]) remove(cpp_tmp);
            return 1;
        }
        run_file = args.output_file;
    }

    FILE *file;
    if (args.ignore_suffix) {
        file = fopen(run_file, "r");
        if (!file) {
            lo3_error("Could not load the corresponding file!", run_file);
            if (cpp_tmp[0]) remove(cpp_tmp);
            return 1;
        }
    } else {
        if (pars_isFileValid((char *)run_file, &file) != 0) {
            if (cpp_tmp[0]) remove(cpp_tmp);
            return 1;
        }
    }

    openFile = file;
    var_init();
    pars_file(file);
    fclose(file);
    var_freeAll();

    if (cpp_tmp[0])
        remove(cpp_tmp);

    return 0;
}
