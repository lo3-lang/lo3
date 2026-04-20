#include "./internal/core.h"
#include "./internal/bare-var.h"
#include <stdio.h>
#include <string.h>

EMSCRIPTEN_KEEPALIVE
const char* lo3_version(void) {
    return "0.3.x";
}

EMSCRIPTEN_KEEPALIVE
int lo3_run(const char* code) {
    if (!code) return -1;

    size_t len = strlen(code);
    char* buf = (char*)malloc(len + 1);
    if (!buf) return -1;
    memcpy(buf, code, len + 1);

    FILE* mem_file = fmemopen(buf, len, "r");
    if (!mem_file) {
        free(buf);
        return -1;
    }

    var_init();
    rush = FALSE;
    isWarped = FALSE;
    currentLine = 0;

    int result = pars_file(mem_file);

    (void)fclose(mem_file);
    free(buf);
    var_freeAll();

    return result;
}