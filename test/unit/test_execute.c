// Copyright (c) 2026 seesee010
// Unit tests for src/execute.c

#include "unity.h"
#include "../../src/internal/bare-define.h"
#include "../../src/internal/bare-var.h"
#include "../../src/internal/global.h"
#include "../../src/internal/control-flow.h"
#include "../../src/internal/specific-language.h"
#include "../../src/internal/core.h"
#include <string.h>
#include <stdio.h>

// openFile defined in main.c — provide stub for tests
FILE *openFile = NULL;

// dummy array for the unused array[2] parameter
static char DUMMY[2] = {0, 0};

// --- helpers to build lo3_val ---

static lo3_val num_val(int n) {
    lo3_val v;
    v.type = TYPE_num;
    v.chooseType = 0;
    v.value.num = n;
    return v;
}

static lo3_val str_val(unsigned char *s) {
    lo3_val v;
    v.type = TYPE_string;
    v.chooseType = 3;
    v.value.string = s;
    return v;
}

void setUp(void) {
    var_init();
    memset(&cf, 0, sizeof(cf));
    rush = FALSE;
    isWarped = FALSE;
}

void tearDown(void) {
    var_freeAll();
}

// --- exec_new ---

void test_exec_new_creates_num_var(void) {
    unsigned char name[] = "myvar";
    exec_new(str_val(name), num_val(0), DUMMY);
    TEST_ASSERT_TRUE(var_find("myvar") >= 0);
}

void test_exec_new_creates_string_var(void) {
    unsigned char name[] = "strvar";
    exec_new(str_val(name), num_val(3), DUMMY);
    TEST_ASSERT_TRUE(var_find("strvar") >= 0);
}

void test_exec_new_duplicate_no_crash(void) {
    unsigned char name[] = "dup";
    exec_new(str_val(name), num_val(0), DUMMY);
    exec_new(str_val(name), num_val(0), DUMMY); // second: error printed, no crash
    TEST_ASSERT_TRUE(var_find("dup") >= 0);
}

// --- exec_free ---

void test_exec_free_deletes_var(void) {
    var_create("delme", 0);
    unsigned char name[] = "delme";
    exec_free(str_val(name), num_val(0), DUMMY);
    TEST_ASSERT_EQUAL_INT(-1, var_find("delme"));
}

void test_exec_free_nonexistent_no_crash(void) {
    unsigned char name[] = "ghost";
    exec_free(str_val(name), num_val(0), DUMMY);
}

// --- exec_asn ---

void test_exec_asn_assigns_num(void) {
    var_create("asn", 0);
    unsigned char name[] = "asn";
    exec_asn(str_val(name), num_val(99), DUMMY);
    TEST_ASSERT_EQUAL_INT(99, var_getNum(var_get("asn")));
}

void test_exec_asn_assigns_string(void) {
    var_create("asnstr", 3);
    unsigned char name[] = "asnstr";
    // strdup so var_freeAll can safely free it (var_setString stores raw pointer)
    unsigned char *val = (unsigned char *)strdup("hello");
    exec_asn(str_val(name), str_val(val), DUMMY);
    TEST_ASSERT_EQUAL_STRING("hello", (const char *)var_getString(var_get("asnstr")));
}

void test_exec_asn_nonexistent_no_crash(void) {
    unsigned char name[] = "noexist";
    exec_asn(str_val(name), num_val(1), DUMMY);
}

// --- exec_add ---

void test_exec_add_basic(void) {
    var_create("addvar", 0);
    var_setNum("addvar", 10);
    unsigned char name[] = "addvar";
    exec_add(str_val(name), num_val(5), DUMMY);
    TEST_ASSERT_EQUAL_INT(15, var_getNum(var_get("addvar")));
}

void test_exec_add_negative_operand(void) {
    var_create("addneg", 0);
    var_setNum("addneg", 10);
    unsigned char name[] = "addneg";
    exec_add(str_val(name), num_val(-3), DUMMY);
    TEST_ASSERT_EQUAL_INT(7, var_getNum(var_get("addneg")));
}

void test_exec_add_nonexistent_no_crash(void) {
    unsigned char name[] = "noexist";
    exec_add(str_val(name), num_val(1), DUMMY);
}

void test_exec_add_string_arg2_no_crash(void) {
    var_create("addstr", 0);
    unsigned char name[] = "addstr";
    unsigned char bad[] = "notanumber";
    exec_add(str_val(name), str_val(bad), DUMMY); // error: arg2 must be num
}

// --- exec_sub ---

void test_exec_sub_basic(void) {
    var_create("subvar", 0);
    var_setNum("subvar", 10);
    unsigned char name[] = "subvar";
    exec_sub(str_val(name), num_val(3), DUMMY);
    TEST_ASSERT_EQUAL_INT(7, var_getNum(var_get("subvar")));
}

void test_exec_sub_result_negative(void) {
    var_create("subneg", 0);
    var_setNum("subneg", 2);
    unsigned char name[] = "subneg";
    exec_sub(str_val(name), num_val(10), DUMMY);
    TEST_ASSERT_EQUAL_INT(-8, var_getNum(var_get("subneg")));
}

// --- exec_mul ---

void test_exec_mul_basic(void) {
    var_create("mulvar", 0);
    var_setNum("mulvar", 4);
    unsigned char name[] = "mulvar";
    exec_mul(str_val(name), num_val(3), DUMMY);
    TEST_ASSERT_EQUAL_INT(12, var_getNum(var_get("mulvar")));
}

void test_exec_mul_by_zero(void) {
    var_create("mulzero", 0);
    var_setNum("mulzero", 100);
    unsigned char name[] = "mulzero";
    exec_mul(str_val(name), num_val(0), DUMMY);
    TEST_ASSERT_EQUAL_INT(0, var_getNum(var_get("mulzero")));
}

// --- exec_div ---

void test_exec_div_basic(void) {
    var_create("divvar", 0);
    var_setNum("divvar", 12);
    unsigned char name[] = "divvar";
    exec_div(str_val(name), num_val(4), DUMMY);
    TEST_ASSERT_EQUAL_INT(3, var_getNum(var_get("divvar")));
}

void test_exec_div_truncates(void) {
    var_create("divtrunc", 0);
    var_setNum("divtrunc", 7);
    unsigned char name[] = "divtrunc";
    exec_div(str_val(name), num_val(2), DUMMY);
    TEST_ASSERT_EQUAL_INT(3, var_getNum(var_get("divtrunc"))); // integer truncation
}

void test_exec_div_by_zero_no_crash(void) {
    var_create("divz", 0);
    var_setNum("divz", 10);
    unsigned char name[] = "divz";
    exec_div(str_val(name), num_val(0), DUMMY); // should print error, not crash
    // value should remain unchanged (10)
    TEST_ASSERT_EQUAL_INT(10, var_getNum(var_get("divz")));
}

// --- exec_label ---

void test_exec_label_registers_label(void) {
    unsigned char lname[] = "myLabel";
    exec_label(str_val(lname), num_val(0), DUMMY);
    TEST_ASSERT_TRUE(cf_findLabel("myLabel") >= 0);
}

void test_exec_label_duplicate_no_crash(void) {
    unsigned char lname[] = "dupLabel";
    exec_label(str_val(lname), num_val(0), DUMMY);
    exec_label(str_val(lname), num_val(0), DUMMY); // error: already exists
    // still findable
    TEST_ASSERT_TRUE(cf_findLabel("dupLabel") >= 0);
}

// --- exec_out ---

void test_exec_out_num_no_crash(void) {
    exec_out(num_val(42), num_val(0), DUMMY);
}

void test_exec_out_string_no_crash(void) {
    unsigned char s[] = "hello world";
    exec_out(str_val(s), num_val(0), DUMMY);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_exec_new_creates_num_var);
    RUN_TEST(test_exec_new_creates_string_var);
    RUN_TEST(test_exec_new_duplicate_no_crash);

    RUN_TEST(test_exec_free_deletes_var);
    RUN_TEST(test_exec_free_nonexistent_no_crash);

    RUN_TEST(test_exec_asn_assigns_num);
    RUN_TEST(test_exec_asn_assigns_string);
    RUN_TEST(test_exec_asn_nonexistent_no_crash);

    RUN_TEST(test_exec_add_basic);
    RUN_TEST(test_exec_add_negative_operand);
    RUN_TEST(test_exec_add_nonexistent_no_crash);
    RUN_TEST(test_exec_add_string_arg2_no_crash);

    RUN_TEST(test_exec_sub_basic);
    RUN_TEST(test_exec_sub_result_negative);

    RUN_TEST(test_exec_mul_basic);
    RUN_TEST(test_exec_mul_by_zero);

    RUN_TEST(test_exec_div_basic);
    RUN_TEST(test_exec_div_truncates);
    RUN_TEST(test_exec_div_by_zero_no_crash);

    RUN_TEST(test_exec_label_registers_label);
    RUN_TEST(test_exec_label_duplicate_no_crash);

    RUN_TEST(test_exec_out_num_no_crash);
    RUN_TEST(test_exec_out_string_no_crash);

    return UNITY_END();
}
