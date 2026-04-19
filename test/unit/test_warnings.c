// Copyright (c) 2026 seesee010
// Unit tests for src/warnings.c

#include "unity.h"
#include "../../src/internal/core.h"

// stub: warnings.c uses currentLine as extern
int currentLine = 0;

void setUp(void) {
    currentLine = 0;
}

void tearDown(void) {}

// --- lo3_warn ---

void test_lo3_warn_valid_msg_and_context_no_crash(void) {
    lo3_warn("test warning message", "some_context");
}

void test_lo3_warn_null_context_no_crash(void) {
    lo3_warn("warning without context", NULL);
}

void test_lo3_warn_empty_msg_triggers_error_no_crash(void) {
    // empty msg causes lo3_error internally — must not crash
    lo3_warn("", "ctx");
}

void test_lo3_warn_with_line_number(void) {
    currentLine = 42;
    lo3_warn("line number test", "file.lo3");
    // just verify no crash — output goes to stderr
}

// --- lo3_error ---

void test_lo3_error_valid_msg_and_context_no_crash(void) {
    lo3_error("test error message", "some_context");
}

void test_lo3_error_null_context_no_crash(void) {
    lo3_error("error without context", NULL);
}

void test_lo3_error_empty_msg_triggers_warn_no_crash(void) {
    // empty msg causes lo3_warn internally — must not crash
    lo3_error("", NULL);
}

void test_lo3_error_with_line_number(void) {
    currentLine = 7;
    lo3_error("line number test", "myfile.lo3");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_lo3_warn_valid_msg_and_context_no_crash);
    RUN_TEST(test_lo3_warn_null_context_no_crash);
    RUN_TEST(test_lo3_warn_empty_msg_triggers_error_no_crash);
    RUN_TEST(test_lo3_warn_with_line_number);

    RUN_TEST(test_lo3_error_valid_msg_and_context_no_crash);
    RUN_TEST(test_lo3_error_null_context_no_crash);
    RUN_TEST(test_lo3_error_empty_msg_triggers_warn_no_crash);
    RUN_TEST(test_lo3_error_with_line_number);

    return UNITY_END();
}
