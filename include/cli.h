// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#pragma once

typedef struct {
    const char *input_file;
    const char *output_file;
    int use_cpp;
    int ignore_suffix;
    int dry_run;
    int show_version;
    int show_help;
} lo3_args;

// Returns 0 on success, -1 on parse error (error already printed)
int cli_parse(int argc, char *argv[], lo3_args *args);
void cli_help(void);
void cli_version(void);
