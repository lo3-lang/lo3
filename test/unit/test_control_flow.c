// Copyright (c) 2026 seesee010
// Unit tests for src/control-flow.c

#include "unity.h"
#include "../../src/internal/bare-define.h"
#include "../../src/internal/control-flow.h"
#include "../../src/internal/core.h"
#include <string.h>
#include <stdio.h>

// stubs — not linking parsing.c or main.c here
int currentLine = 0;
FILE *openFile = NULL;

void setUp(void) {
    // reset all control-flow state before each test
    memset(&cf, 0, sizeof(cf));
    rush = FALSE;
    isWarped = FALSE;
    rush_target = NULL;
}

void tearDown(void) {}

// --- cf_findLabel ---

void test_cf_findLabel_empty_returns_minus1(void) {
    TEST_ASSERT_EQUAL_INT(-1, cf_findLabel("anyLabel"));
}

void test_cf_findLabel_after_add(void) {
    cf_addLabel("alpha", 10);
    TEST_ASSERT_TRUE(cf_findLabel("alpha") >= 0);
}

void test_cf_findLabel_unknown_returns_minus1(void) {
    cf_addLabel("known", 5);
    TEST_ASSERT_EQUAL_INT(-1, cf_findLabel("unknown"));
}

void test_cf_findLabel_multiple_labels(void) {
    cf_addLabel("l1", 1);
    cf_addLabel("l2", 2);
    cf_addLabel("l3", 3);
    TEST_ASSERT_TRUE(cf_findLabel("l1") >= 0);
    TEST_ASSERT_TRUE(cf_findLabel("l2") >= 0);
    TEST_ASSERT_TRUE(cf_findLabel("l3") >= 0);
    TEST_ASSERT_EQUAL_INT(-1, cf_findLabel("l4"));
}

// --- cf_addLabel ---

void test_cf_addLabel_returns_zero_on_success(void) {
    int r = cf_addLabel("newlabel", 42);
    TEST_ASSERT_EQUAL_INT(0, r);
}

void test_cf_addLabel_duplicate_returns_minus1(void) {
    cf_addLabel("dup", 1);
    int r = cf_addLabel("dup", 2);
    TEST_ASSERT_EQUAL_INT(-1, r);
}

void test_cf_addLabel_increments_count(void) {
    TEST_ASSERT_EQUAL_INT(0, cf.nextFreePos);
    cf_addLabel("a", 0);
    TEST_ASSERT_EQUAL_INT(1, cf.nextFreePos);
    cf_addLabel("b", 0);
    TEST_ASSERT_EQUAL_INT(2, cf.nextFreePos);
}

void test_cf_addLabel_full_returns_minus1(void) {
    // static so each entry has its own persistent address (not the same stack buf)
    static char names[ARRAY_SIZE][16];
    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
        snprintf(names[i], 16, "lbl%d", i);
        cf_addLabel(names[i], i);
    }
    // array full — next add must fail
    int r = cf_addLabel("overflow", 999);
    TEST_ASSERT_EQUAL_INT(-1, r);
}

// --- cf_getPos ---

void test_cf_getPos_known_label(void) {
    cf_addLabel("pos_test", 100);
    TEST_ASSERT_EQUAL_INT(100, cf_getPos("pos_test"));
}

void test_cf_getPos_unknown_label_returns_minus1(void) {
    TEST_ASSERT_EQUAL_INT(-1, cf_getPos("ghost"));
}

void test_cf_getPos_multiple(void) {
    cf_addLabel("x", 11);
    cf_addLabel("y", 22);
    TEST_ASSERT_EQUAL_INT(11, cf_getPos("x"));
    TEST_ASSERT_EQUAL_INT(22, cf_getPos("y"));
}

// --- cf_getCursorPos ---

void test_cf_getCursorPos_always_minus1(void) {
    // documented as unimplemented — always returns -1
    TEST_ASSERT_EQUAL_INT(-1, cf_getCursorPos());
}

// --- cf_jumpToLabel ---

void test_cf_jumpToLabel_nonexistent_returns_minus1(void) {
    int r = cf_jumpToLabel("noexist");
    TEST_ASSERT_EQUAL_INT(-1, r);
}

void test_cf_jumpToLabel_existing_returns_zero(void) {
    // openFile is NULL — fseek(NULL,...) is UB, so we open a real temp file
    FILE *tmp = tmpfile();
    TEST_ASSERT_NOT_NULL(tmp);
    openFile = tmp;
    cf_addLabel("jmp_target", 0);
    int r = cf_jumpToLabel("jmp_target");
    TEST_ASSERT_EQUAL_INT(0, r);
    fclose(tmp);
    openFile = NULL;
}

// --- rush flag ---

void test_rush_initially_false(void) {
    TEST_ASSERT_EQUAL_INT(FALSE, rush);
}

void test_isWarped_initially_false(void) {
    TEST_ASSERT_EQUAL_INT(FALSE, isWarped);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_cf_findLabel_empty_returns_minus1);
    RUN_TEST(test_cf_findLabel_after_add);
    RUN_TEST(test_cf_findLabel_unknown_returns_minus1);
    RUN_TEST(test_cf_findLabel_multiple_labels);

    RUN_TEST(test_cf_addLabel_returns_zero_on_success);
    RUN_TEST(test_cf_addLabel_duplicate_returns_minus1);
    RUN_TEST(test_cf_addLabel_increments_count);
    RUN_TEST(test_cf_addLabel_full_returns_minus1);

    RUN_TEST(test_cf_getPos_known_label);
    RUN_TEST(test_cf_getPos_unknown_label_returns_minus1);
    RUN_TEST(test_cf_getPos_multiple);

    RUN_TEST(test_cf_getCursorPos_always_minus1);

    RUN_TEST(test_cf_jumpToLabel_nonexistent_returns_minus1);
    RUN_TEST(test_cf_jumpToLabel_existing_returns_zero);

    RUN_TEST(test_rush_initially_false);
    RUN_TEST(test_isWarped_initially_false);

    return UNITY_END();
}
