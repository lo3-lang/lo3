// Copyright (c) 2026 seesee010
// Unit tests for src/global.c

#include "unity.h"
#include "../../src/internal/bare-define.h"
#include "../../src/internal/global.h"
#include "../../src/internal/core.h"

// g_getString is defined in global.c but missing from global.h
char *g_getString(int index);

// stub: warnings.c uses currentLine (from parsing.c, not linked here)
int currentLine = 0;

// helpers
static lo3_val make_num_val(int n) {
    lo3_val v;
    v.type = TYPE_num;
    v.chooseType = 0;
    v.value.num = n;
    return v;
}

static lo3_val make_str_val(unsigned char *s) {
    lo3_val v;
    v.type = TYPE_string;
    v.chooseType = 3;
    v.value.string = s;
    return v;
}

static lo3_val make_type_val(int ct) {
    lo3_val v;
    v.chooseType = ct;
    v.type = (ct == 0) ? TYPE_num : TYPE_string;
    v.value.num = 0;
    return v;
}

void setUp(void) {}
void tearDown(void) {}

// --- g_isSet ---

void test_g_isSet_fresh_index_is_zero(void) {
    // index 50 — using a fresh index unlikely to be set by other tests
    TEST_ASSERT_EQUAL_INT(0, g_isSet(50));
}

void test_g_isSet_after_set_returns_one(void) {
    lo3_val v = make_num_val(123);
    g_set(51, v);
    TEST_ASSERT_EQUAL_INT(1, g_isSet(51));
}

void test_g_isSet_oob_high_returns_minus1(void) {
    TEST_ASSERT_EQUAL_INT(-1, g_isSet(100)); // G_SIZE is 100, valid: 0..99
}

void test_g_isSet_oob_low_returns_minus1(void) {
    TEST_ASSERT_EQUAL_INT(-1, g_isSet(-1));
}

// --- g_set / g_get ---

void test_g_set_get_num(void) {
    lo3_val v = make_num_val(777);
    g_set(10, v);
    lo3_val got = g_get(10);
    TEST_ASSERT_EQUAL_INT(777, got.value.num);
}

void test_g_set_get_string(void) {
    unsigned char s[] = "teststring";
    lo3_val v = make_str_val(s);
    g_set(11, v);
    lo3_val got = g_get(11);
    TEST_ASSERT_EQUAL_STRING("teststring", (const char *)got.value.string);
}

void test_g_get_oob_no_crash(void) {
    // should print error and return g.value[0] (not crash)
    lo3_val r = g_get(100);
    (void)r;
}

void test_g_set_oob_no_crash(void) {
    lo3_val v = make_num_val(1);
    g_set(100, v); // should print error and return
}

// --- g_getNum ---

void test_g_getNum_after_set(void) {
    lo3_val v = make_num_val(42);
    g_set(20, v);
    TEST_ASSERT_EQUAL_INT(42, g_getNum(20));
}

void test_g_getNum_oob_returns_minus1(void) {
    TEST_ASSERT_EQUAL_INT(-1, g_getNum(100));
}

void test_g_getNum_oob_low_returns_minus1(void) {
    TEST_ASSERT_EQUAL_INT(-1, g_getNum(-1));
}

// --- g_getString ---

void test_g_getString_after_set(void) {
    unsigned char str[] = "lo3test";
    lo3_val v = make_str_val(str);
    g_set(30, v);
    char *result = g_getString(30);
    TEST_ASSERT_EQUAL_STRING("lo3test", result);
}

void test_g_getString_oob_returns_error_sentinel(void) {
    // returns "_error" on OOB — as documented in global.c
    char *r = g_getString(100);
    TEST_ASSERT_EQUAL_STRING("_error", r);
}

// --- g_getType / g_setType ---

void test_g_getType_after_setType_num(void) {
    lo3_val t = make_type_val(0);
    g_setType(40, t);
    TEST_ASSERT_EQUAL_INT(0, g_getType(40));
}

void test_g_getType_after_setType_string(void) {
    lo3_val t = make_type_val(3);
    g_setType(41, t);
    TEST_ASSERT_EQUAL_INT(3, g_getType(41));
}

void test_g_getType_oob_returns_minus1(void) {
    TEST_ASSERT_EQUAL_INT(-1, g_getType(100));
}

void test_g_setType_oob_returns_minus1(void) {
    lo3_val t = make_type_val(0);
    TEST_ASSERT_EQUAL_INT(-1, g_setType(100, t));
}

void test_g_setType_invalid_type_returns_minus1(void) {
    lo3_val t = make_type_val(5); // chooseType = 5, invalid (must be 0-3)
    TEST_ASSERT_EQUAL_INT(-1, g_setType(42, t));
}

// --- g_getValue ---

void test_g_getValue_returns_placeholder(void) {
    // g_getValue is marked WIP and returns chooseType = -1
    lo3_val r = g_getValue(0);
    TEST_ASSERT_EQUAL_INT(-1, r.chooseType);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_g_isSet_fresh_index_is_zero);
    RUN_TEST(test_g_isSet_after_set_returns_one);
    RUN_TEST(test_g_isSet_oob_high_returns_minus1);
    RUN_TEST(test_g_isSet_oob_low_returns_minus1);

    RUN_TEST(test_g_set_get_num);
    RUN_TEST(test_g_set_get_string);
    RUN_TEST(test_g_get_oob_no_crash);
    RUN_TEST(test_g_set_oob_no_crash);

    RUN_TEST(test_g_getNum_after_set);
    RUN_TEST(test_g_getNum_oob_returns_minus1);
    RUN_TEST(test_g_getNum_oob_low_returns_minus1);

    RUN_TEST(test_g_getString_after_set);
    RUN_TEST(test_g_getString_oob_returns_error_sentinel);

    RUN_TEST(test_g_getType_after_setType_num);
    RUN_TEST(test_g_getType_after_setType_string);
    RUN_TEST(test_g_getType_oob_returns_minus1);
    RUN_TEST(test_g_setType_oob_returns_minus1);
    RUN_TEST(test_g_setType_invalid_type_returns_minus1);

    RUN_TEST(test_g_getValue_returns_placeholder);

    return UNITY_END();
}
