// Copyright (c) 2026 seesee010
// Read the License file for more informations about the license.

#include "./internal/cli.h"
#include "./internal/core.h"
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <process.h>
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#define BUF_SIZE 4096

void cli_help(void) {
    lo3_printD("Usage: lo3 [OPTIONS] [FILE]\n\n");
    lo3_printD("  lo3 <FILE.lo3>                  Run a lo3 file\n");
    lo3_printD("  lo3 <FILE.LO3>                  Run with C preprocessor first\n");
    lo3_printD("  lo3 --cpp <FILE>                Run the C preprocessor on FILE before executing\n");
    lo3_printD("  lo3 <FILE> -o <NEWNAME>         Copy FILE to NEWNAME and execute NEWNAME\n");
    lo3_printD("  lo3 <FILE> --ignore-suffix      Execute FILE regardless of extension\n");
    lo3_printD("  lo3 --dry-run, -n               Read lo3 from stdin and execute\n");
    lo3_printD("  lo3 --version, -v               Show the current build version\n");
    lo3_printD("  lo3 --help, -h                  Show this help message\n");
}

void cli_version(void) {
    lo3_printD("lo3 version %s\n", LO3_VERSION);
}

lo3_mode cli_get_mode(const lo3_args *args) {
    if (args->show_help)    return MODE_HELP;
    if (args->show_version) return MODE_VERSION;
    if (args->dry_run)      return MODE_DRY_RUN;
    return MODE_NORMAL;
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
            if (i + 1 < argc) {
                if (args->input_file) {
                    lo3_error("Multiple input files are not supported", argv[i + 1]);
                    return -1;
                }
                args->input_file = argv[++i];
            } else {
                lo3_error("--cpp requires a filename", "");
                return -1;
            }

        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                args->output_file = argv[++i];
            } else {
                lo3_error("-o requires a new filename", "");
                return -1;
            }

        } else if (argv[i][0] != '-') {
            if (args->input_file) {
                lo3_error("Multiple input files are not supported", argv[i]);
                return -1;
            }
            args->input_file = argv[i];

        } else {
            lo3_error("Unknown option", argv[i]);
            return -1;
        }
    }
    return 0;
}

int cli_copy_file(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");
    if (!in) {
        return -1;
    }
    FILE *out = fopen(dst, "wb");
    if (!out) {
        fclose(in);
        return -1;
    }
    char buf[BUF_SIZE];
    size_t n;
    int ok = 1;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) {
            ok = 0;
            break;
        }
    }
    if (ferror(in)) {
        ok = 0;
    }
    fclose(in);
    fclose(out);
    if (!ok) {
        remove(dst);
    }
    return ok ? 0 : -1;
}

int cli_make_tmp(char *buf, size_t size, const char *prefix, int *fd_out) {
#ifdef _WIN32
    char tmp_dir[MAX_PATH];
    DWORD len = GetTempPathA(MAX_PATH, tmp_dir);
    if (len == 0 || len > MAX_PATH) {
        const char *fallback = getenv("TEMP");
        if (!fallback) {
            fallback = "C:\\Temp";
        }
        snprintf(tmp_dir, sizeof(tmp_dir), "%s", fallback);
    }

    // Generate unique filename using GUID-like approach with high-res counter
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    snprintf(buf, size, "%s\\lo3_%s_%d_%lld.lo3", tmp_dir, prefix,
             (int)_getpid(), (long long)counter.QuadPart);

    // Create file atomically with CREATE_NEW flag
    HANDLE hFile = CreateFileA(buf, GENERIC_WRITE | GENERIC_READ, 0, NULL,
                               CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // Convert Windows HANDLE to C file descriptor
    int fd = _open_osfhandle((intptr_t)hFile, 0);
    if (fd < 0) {
        CloseHandle(hFile);
        return -1;
    }

    if (fd_out) {
        *fd_out = fd;
    } else {
        _close(fd);
    }
    return 0;
#else
    snprintf(buf, size, "/tmp/lo3_%s_XXXXXX", prefix);
    int fd = mkstemp(buf);
    if (fd < 0) {
        return -1;
    }

    // Keep fd open and return it to caller to avoid TOCTOU
    if (fd_out) {
        *fd_out = fd;
    } else {
        close(fd);
    }
    return 0;
#endif
}

int cli_run_cpp(const char *input, const char *output) {
#ifdef _WIN32
    char *cpp_argv[] = {"cpp", "-P", (char *)input, "-o", (char *)output, NULL};
    int rc = (int)_spawnvp(_P_WAIT, "cpp", (const char *const *)cpp_argv);
    return (rc == 0) ? 0 : -1;
#else
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    }
    if (pid == 0) {
        char *cpp_argv[] = {"cpp", "-P", (char *)input, "-o", (char *)output, NULL};
        execvp("cpp", cpp_argv);
        _exit(127);
    }
    int status;
    if (waitpid(pid, &status, 0) < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        return -1;
    }
    return 0;
#endif
}

int cli_has_upper_ext(const char *name) {
    size_t len = strlen(name);
    return len >= 4 && strcmp(&name[len - 4], ".LO3") == 0;
}
