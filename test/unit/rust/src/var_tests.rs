use super::*;
use std::ffi::{CStr, CString};

fn setup() { unsafe { var_init(); } }
fn teardown() { unsafe { var_freeAll(); } }

macro_rules! with_vars {
    ($body:block) => {{ setup(); $body; teardown(); }};
}

fn cstr(s: &str) -> CString { CString::new(s).unwrap() }

// ── var_find ─────────────────────────────────────────────────────────────────

#[test]
fn find_returns_minus1_when_empty() {
    with_vars!({
        assert_eq!(-1, unsafe { var_find(cstr("x").as_ptr()) });
    });
}

#[test]
fn find_returns_minus1_for_unknown() {
    with_vars!({
        unsafe { var_create(cstr("known").as_ptr(), 0); }
        assert_eq!(-1, unsafe { var_find(cstr("unknown").as_ptr()) });
    });
}

#[test]
fn find_returns_valid_index_after_create() {
    with_vars!({
        unsafe { var_create(cstr("myvar").as_ptr(), 0); }
        assert!(unsafe { var_find(cstr("myvar").as_ptr()) } >= 0);
    });
}

#[test]
fn find_multiple_vars() {
    with_vars!({
        unsafe {
            var_create(cstr("a").as_ptr(), 0);
            var_create(cstr("b").as_ptr(), 0);
            var_create(cstr("c").as_ptr(), 3);
        }
        assert!(unsafe { var_find(cstr("a").as_ptr()) } >= 0);
        assert!(unsafe { var_find(cstr("b").as_ptr()) } >= 0);
        assert!(unsafe { var_find(cstr("c").as_ptr()) } >= 0);
        assert_eq!(-1, unsafe { var_find(cstr("d").as_ptr()) });
    });
}

// ── var_create ───────────────────────────────────────────────────────────────

#[test]
fn create_num_type() {
    with_vars!({
        unsafe { var_create(cstr("numvar").as_ptr(), 0); }
        assert!(unsafe { var_find(cstr("numvar").as_ptr()) } >= 0);
    });
}

#[test]
fn create_string_type() {
    with_vars!({
        unsafe { var_create(cstr("strvar").as_ptr(), 3); }
        assert!(unsafe { var_find(cstr("strvar").as_ptr()) } >= 0);
    });
}

#[test]
fn create_empty_name_no_crash() {
    with_vars!({
        unsafe { var_create(cstr("").as_ptr(), 0); }
        assert_eq!(-1, unsafe { var_find(cstr("").as_ptr()) });
    });
}

#[test]
fn create_null_name_no_crash() {
    with_vars!({
        unsafe { var_create(std::ptr::null(), 0); }
    });
}

#[test]
fn create_duplicate_no_crash() {
    with_vars!({
        unsafe {
            var_create(cstr("dup").as_ptr(), 0);
            var_create(cstr("dup").as_ptr(), 0);
        }
        assert!(unsafe { var_find(cstr("dup").as_ptr()) } >= 0);
    });
}

// ── var_get ──────────────────────────────────────────────────────────────────

#[test]
fn get_returns_non_null_for_existing() {
    with_vars!({
        unsafe { var_create(cstr("g_test").as_ptr(), 0); }
        assert!(!unsafe { var_get(cstr("g_test").as_ptr()) }.is_null());
    });
}

#[test]
fn get_returns_null_for_missing() {
    with_vars!({
        assert!(unsafe { var_get(cstr("ghost").as_ptr()) }.is_null());
    });
}

// ── var_getType ──────────────────────────────────────────────────────────────

#[test]
fn gettype_num() {
    with_vars!({
        unsafe { var_create(cstr("nvar").as_ptr(), 0); }
        let v = unsafe { var_get(cstr("nvar").as_ptr()) };
        assert_eq!(0, unsafe { var_getType(v) });
    });
}

#[test]
fn gettype_string() {
    with_vars!({
        unsafe { var_create(cstr("svar").as_ptr(), 3); }
        let v = unsafe { var_get(cstr("svar").as_ptr()) };
        assert_eq!(3, unsafe { var_getType(v) });
    });
}

// ── var_setNum / var_getNum ───────────────────────────────────────────────────

#[test]
fn setnum_and_getnum() {
    with_vars!({
        unsafe {
            var_create(cstr("n").as_ptr(), 0);
            var_setNum(cstr("n").as_ptr(), 42);
        }
        let v = unsafe { var_get(cstr("n").as_ptr()) };
        assert_eq!(42, unsafe { var_getNum(v) });
    });
}

#[test]
fn setnum_negative() {
    with_vars!({
        unsafe {
            var_create(cstr("neg").as_ptr(), 0);
            var_setNum(cstr("neg").as_ptr(), -99);
        }
        let v = unsafe { var_get(cstr("neg").as_ptr()) };
        assert_eq!(-99, unsafe { var_getNum(v) });
    });
}

#[test]
fn setnum_zero() {
    with_vars!({
        unsafe {
            var_create(cstr("zero").as_ptr(), 0);
            var_setNum(cstr("zero").as_ptr(), 0);
        }
        let v = unsafe { var_get(cstr("zero").as_ptr()) };
        assert_eq!(0, unsafe { var_getNum(v) });
    });
}

#[test]
fn setnum_overwrite() {
    with_vars!({
        unsafe {
            var_create(cstr("ow").as_ptr(), 0);
            var_setNum(cstr("ow").as_ptr(), 10);
            var_setNum(cstr("ow").as_ptr(), 99);
        }
        let v = unsafe { var_get(cstr("ow").as_ptr()) };
        assert_eq!(99, unsafe { var_getNum(v) });
    });
}

#[test]
fn setnum_wrong_type_no_crash() {
    with_vars!({
        unsafe {
            var_create(cstr("s").as_ptr(), 3);
            var_setNum(cstr("s").as_ptr(), 42);
        }
    });
}

#[test]
fn setnum_nonexistent_no_crash() {
    with_vars!({
        unsafe { var_setNum(cstr("noexist").as_ptr(), 1); }
    });
}

// ── var_setString / var_getString ─────────────────────────────────────────────

#[test]
fn setstring_and_getstring() {
    with_vars!({
        unsafe {
            var_create(cstr("str").as_ptr(), 3);
            let ptr = c_strdup("hello"); // libc::strdup → malloc-owned, C can free()
            var_setString(cstr("str").as_ptr(), ptr);
        }
        let v = unsafe { var_get(cstr("str").as_ptr()) };
        let got = unsafe { CStr::from_ptr(var_getString(v)) };
        assert_eq!("hello", got.to_str().unwrap());
    });
}

#[test]
fn setstring_wrong_type_no_crash() {
    with_vars!({
        unsafe {
            var_create(cstr("num").as_ptr(), 0);
            var_setString(cstr("num").as_ptr(), cstr("hi").as_ptr() as *mut i8);
        }
    });
}

#[test]
fn setstring_nonexistent_no_crash() {
    with_vars!({
        unsafe { var_setString(cstr("noexist").as_ptr(), cstr("x").as_ptr() as *mut i8); }
    });
}

// ── var_free ──────────────────────────────────────────────────────────────────

#[test]
fn free_removes_var() {
    with_vars!({
        unsafe {
            var_create(cstr("tmp").as_ptr(), 0);
            var_free(cstr("tmp").as_ptr());
        }
        assert_eq!(-1, unsafe { var_find(cstr("tmp").as_ptr()) });
    });
}

#[test]
fn free_nonexistent_no_crash() {
    with_vars!({
        unsafe { var_free(cstr("ghost").as_ptr()); }
    });
}

#[test]
fn free_last_element() {
    with_vars!({
        unsafe {
            var_create(cstr("only").as_ptr(), 0);
            var_free(cstr("only").as_ptr());
        }
        assert_eq!(-1, unsafe { var_find(cstr("only").as_ptr()) });
    });
}

#[test]
fn free_middle_element() {
    with_vars!({
        unsafe {
            var_create(cstr("a").as_ptr(), 0);
            var_create(cstr("b").as_ptr(), 0);
            var_create(cstr("c").as_ptr(), 0);
            var_free(cstr("b").as_ptr());
        }
        assert!(unsafe { var_find(cstr("a").as_ptr()) } >= 0);
        assert_eq!(-1, unsafe { var_find(cstr("b").as_ptr()) });
        assert!(unsafe { var_find(cstr("c").as_ptr()) } >= 0);
    });
}

// ── var_getValue ──────────────────────────────────────────────────────────────

#[test]
fn getvalue_num() {
    with_vars!({
        unsafe {
            var_create(cstr("gv").as_ptr(), 0);
            var_setNum(cstr("gv").as_ptr(), 77);
        }
        let v = unsafe { var_get(cstr("gv").as_ptr()) };
        let val = unsafe { var_getValue(v) };
        assert_eq!(77, unsafe { val.num });
    });
}
