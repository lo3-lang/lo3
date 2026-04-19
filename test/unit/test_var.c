// Copyright (c) 2026 seesee010
// Unit tests for src/var.c

#include "unity.h"
#include "../../src/internal/bare-define.h"
#include "../../src/internal/bare-var.h"
#include "../../src/internal/core.h"

// stub: warnings.c uses currentLine from parsing.c, which we don't link here
int currentLine = 0;

void setUp(void) {
    var_init();
}

void tearDown(void) {
    var_freeAll();
}

// --- var_find ---

void test_var_find_returns_minus1_when_empty(void) {
    TEST_ASSERT_EQUAL_INT(-1, var_find("x"));
}

void test_var_find_returns_minus1_for_unknown(void) {
    var_create("known", 0);
    TEST_ASSERT_EQUAL_INT(-1, var_find("unknown"));
}

void test_var_find_returns_valid_index_after_create(void) {
    var_create("myvar", 0);
    TEST_ASSERT_TRUE(var_find("myvar") >= 0);
}

void test_var_find_multiple_vars(void) {
    var_create("a", 0);
    var_create("b", 0);
    var_create("c", 3);
    TEST_ASSERT_TRUE(var_find("a") >= 0);
    TEST_ASSERT_TRUE(var_find("b") >= 0);
    TEST_ASSERT_TRUE(var_find("c") >= 0);
    TEST_ASSERT_EQUAL_INT(-1, var_find("d"));
}

// --- var_create ---

void test_var_create_num_type(void) {
    var_create("numvar", 0);
    TEST_ASSERT_TRUE(var_find("numvar") >= 0);
}

void test_var_create_string_type(void) {
    var_create("strvar", 3);
    TEST_ASSERT_TRUE(var_find("strvar") >= 0);
}

void test_var_create_empty_name_no_crash(void) {
    // should print error but not crash
    var_create("", 0);
    TEST_ASSERT_EQUAL_INT(-1, var_find(""));
}

void test_var_create_null_name_no_crash(void) {
    // should print error but not crash
    var_create(NULL, 0);
}

void test_var_create_duplicate_no_crash(void) {
    // var_create doesn't check duplicates (exec_new does)
    var_create("dup", 0);
    var_create("dup", 0); // second create is allowed at this level
    // both are accessible via find (first match wins)
    TEST_ASSERT_TRUE(var_find("dup") >= 0);
}

// --- var_get ---

void test_var_get_returns_non_null_for_existing(void) {
    var_create("g_test", 0);
    lo3_var *v = var_get("g_test");
    TEST_ASSERT_NOT_NULL(v);
}

void test_var_get_returns_null_for_missing(void) {
    lo3_var *v = var_get("nonexistent");
    TEST_ASSERT_NULL(v);
}

// --- var_getType ---

void test_var_getType_num(void) {
    var_create("nvar", 0);
    lo3_var *v = var_get("nvar");
    TEST_ASSERT_EQUAL_INT(0, var_getType(v));
}

void test_var_getType_string(void) {
    var_create("svar", 3);
    lo3_var *v = var_get("svar");
    TEST_ASSERT_EQUAL_INT(3, var_getType(v));
}

// --- var_setNum / var_getNum ---

void test_var_setNum_and_getNum(void) {
    var_create("n", 0);
    var_setNum("n", 42);
    lo3_var *v = var_get("n");
    TEST_ASSERT_EQUAL_INT(42, var_getNum(v));
}

void test_var_setNum_negative(void) {
    var_create("neg", 0);
    var_setNum("neg", -99);
    lo3_var *v = var_get("neg");
    TEST_ASSERT_EQUAL_INT(-99, var_getNum(v));
}

void test_var_setNum_zero(void) {
    var_create("zero", 0);
    var_setNum("zero", 0);
    lo3_var *v = var_get("zero");
    TEST_ASSERT_EQUAL_INT(0, var_getNum(v));
}

void test_var_setNum_overwrite(void) {
    var_create("ow", 0);
    var_setNum("ow", 10);
    var_setNum("ow", 99);
    lo3_var *v = var_get("ow");
    TEST_ASSERT_EQUAL_INT(99, var_getNum(v));
}

void test_var_setNum_wrong_type_no_crash(void) {
    var_create("s", 3);
    var_setNum("s", 42); // should print error but not crash
}

void test_var_setNum_nonexistent_no_crash(void) {
    var_setNum("noexist", 1); // should print error but not crash
}

// --- var_setString / var_getString ---

void test_var_setString_and_getString(void) {
    var_create("str", 3);
    // strdup so var_freeAll can safely free it (var_setString stores raw pointer)
    var_setString("str", strdup("hello"));
    lo3_var *v = var_get("str");
    TEST_ASSERT_EQUAL_STRING("hello", (const char *)var_getString(v));
}

void test_var_setString_wrong_type_no_crash(void) {
    var_create("num", 0);
    var_setString("num", "hi"); // should print error but not crash
}

void test_var_setString_nonexistent_no_crash(void) {
    var_setString("noexist", "x"); // should print error but not crash
}

// --- var_free ---

void test_var_free_removes_var(void) {
    var_create("tmp", 0);
    TEST_ASSERT_TRUE(var_find("tmp") >= 0);
    var_free("tmp");
    TEST_ASSERT_EQUAL_INT(-1, var_find("tmp"));
}

void test_var_free_nonexistent_no_crash(void) {
    var_free("ghost"); // should print error but not crash
}

void test_var_free_last_element(void) {
    var_create("only", 0);
    var_free("only");
    TEST_ASSERT_EQUAL_INT(-1, var_find("only"));
}

void test_var_free_middle_element(void) {
    var_create("a", 0);
    var_create("b", 0);
    var_create("c", 0);
    var_free("b");
    TEST_ASSERT_TRUE(var_find("a") >= 0);
    TEST_ASSERT_EQUAL_INT(-1, var_find("b"));
    TEST_ASSERT_TRUE(var_find("c") >= 0);
}

// --- var_getValue ---

void test_var_getValue_num(void) {
    var_create("gv", 0);
    var_setNum("gv", 77);
    lo3_var *v = var_get("gv");
    lo3_value val = var_getValue(v);
    TEST_ASSERT_EQUAL_INT(77, val.num);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_var_find_returns_minus1_when_empty);
    RUN_TEST(test_var_find_returns_minus1_for_unknown);
    RUN_TEST(test_var_find_returns_valid_index_after_create);
    RUN_TEST(test_var_find_multiple_vars);

    RUN_TEST(test_var_create_num_type);
    RUN_TEST(test_var_create_string_type);
    RUN_TEST(test_var_create_empty_name_no_crash);
    RUN_TEST(test_var_create_null_name_no_crash);
    RUN_TEST(test_var_create_duplicate_no_crash);

    RUN_TEST(test_var_get_returns_non_null_for_existing);
    RUN_TEST(test_var_get_returns_null_for_missing);

    RUN_TEST(test_var_getType_num);
    RUN_TEST(test_var_getType_string);

    RUN_TEST(test_var_setNum_and_getNum);
    RUN_TEST(test_var_setNum_negative);
    RUN_TEST(test_var_setNum_zero);
    RUN_TEST(test_var_setNum_overwrite);
    RUN_TEST(test_var_setNum_wrong_type_no_crash);
    RUN_TEST(test_var_setNum_nonexistent_no_crash);

    RUN_TEST(test_var_setString_and_getString);
    RUN_TEST(test_var_setString_wrong_type_no_crash);
    RUN_TEST(test_var_setString_nonexistent_no_crash);

    RUN_TEST(test_var_free_removes_var);
    RUN_TEST(test_var_free_nonexistent_no_crash);
    RUN_TEST(test_var_free_last_element);
    RUN_TEST(test_var_free_middle_element);

    RUN_TEST(test_var_getValue_num);

    return UNITY_END();
}
