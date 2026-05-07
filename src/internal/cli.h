// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#pragma once

#include <stddef.h>

typedef struct {
    const char *input_file;
    const char *output_file;
    int use_cpp;
    int ignore_suffix;
    int dry_run;
    int show_version;
    int show_help;
} lo3_args;

typedef enum {
    MODE_HELP,
    MODE_VERSION,
    MODE_DRY_RUN,
    MODE_NORMAL
} lo3_mode;

// Returns 0 on success, -1 on parse error (error already printed)
int       cli_parse(int argc, char *argv[], lo3_args *args);
lo3_mode  cli_get_mode(const lo3_args *args);
void      cli_help(void);
void      cli_version(void);
int       cli_copy_file(const char *src, const char *dst);
int       cli_make_tmp(char *buf, size_t size, const char *prefix);
int       cli_run_cpp(const char *input, const char *output);
int       cli_has_upper_ext(const char *name);
