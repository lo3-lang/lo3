use super::*;
use std::ffi::{CStr, CString};

static mut DUMMY: [i8; 2] = [0, 0];

fn setup() {
    unsafe {
        var_init();
        reset_cf();
    }
}
fn teardown() { unsafe { var_freeAll(); } }

macro_rules! with_exec {
    ($body:block) => {{ setup(); $body; teardown(); }};
}

fn cstr(s: &str) -> CString { CString::new(s).unwrap() }
fn sval(s: &str) -> lo3_val { lo3_val::string(cstr(s).into_raw() as *mut u8) }
fn nval(n: i32) -> lo3_val { lo3_val::num(n) }
fn dummy() -> *mut i8 { unsafe { DUMMY.as_mut_ptr() } }

// ── exec_new ─────────────────────────────────────────────────────────────────

#[test]
fn new_creates_num_var() {
    with_exec!({
        unsafe { exec_new(sval("myvar"), nval(0), dummy()); }
        assert!(unsafe { var_find(cstr("myvar").as_ptr()) } >= 0);
    });
}

#[test]
fn new_creates_string_var() {
    with_exec!({
        unsafe { exec_new(sval("strvar"), nval(3), dummy()); }
        assert!(unsafe { var_find(cstr("strvar").as_ptr()) } >= 0);
    });
}

#[test]
fn new_duplicate_no_crash() {
    with_exec!({
        unsafe {
            exec_new(sval("dup"), nval(0), dummy());
            exec_new(sval("dup"), nval(0), dummy());
        }
        assert!(unsafe { var_find(cstr("dup").as_ptr()) } >= 0);
    });
}

// ── exec_free ────────────────────────────────────────────────────────────────

#[test]
fn free_deletes_var() {
    with_exec!({
        unsafe {
            var_create(cstr("delme").as_ptr(), 0);
            exec_free(sval("delme"), nval(0), dummy());
        }
        assert_eq!(-1, unsafe { var_find(cstr("delme").as_ptr()) });
    });
}

#[test]
fn free_nonexistent_no_crash() {
    with_exec!({
        unsafe { exec_free(sval("ghost"), nval(0), dummy()); }
    });
}

// ── exec_asn ─────────────────────────────────────────────────────────────────

#[test]
fn asn_assigns_num() {
    with_exec!({
        unsafe {
            var_create(cstr("asn").as_ptr(), 0);
            exec_asn(sval("asn"), nval(99), dummy());
        }
        let v = unsafe { var_get(cstr("asn").as_ptr()) };
        assert_eq!(99, unsafe { var_getNum(v) });
    });
}

#[test]
fn asn_assigns_string() {
    with_exec!({
        unsafe {
            var_create(cstr("asnstr").as_ptr(), 3);
            let ptr = c_strdup("hello");
            exec_asn(sval("asnstr"), lo3_val::string(ptr as *mut u8), dummy());
        }
        let v = unsafe { var_get(cstr("asnstr").as_ptr()) };
        let got = unsafe { CStr::from_ptr(var_getString(v)) };
        assert_eq!("hello", got.to_str().unwrap());
    });
}

#[test]
fn asn_nonexistent_no_crash() {
    with_exec!({
        unsafe { exec_asn(sval("noexist"), nval(1), dummy()); }
    });
}

// ── exec_add ─────────────────────────────────────────────────────────────────

#[test]
fn add_basic() {
    with_exec!({
        unsafe {
            var_create(cstr("addvar").as_ptr(), 0);
            var_setNum(cstr("addvar").as_ptr(), 10);
            exec_add(sval("addvar"), nval(5), dummy());
        }
        let v = unsafe { var_get(cstr("addvar").as_ptr()) };
        assert_eq!(15, unsafe { var_getNum(v) });
    });
}

#[test]
fn add_negative_operand() {
    with_exec!({
        unsafe {
            var_create(cstr("addneg").as_ptr(), 0);
            var_setNum(cstr("addneg").as_ptr(), 10);
            exec_add(sval("addneg"), nval(-3), dummy());
        }
        let v = unsafe { var_get(cstr("addneg").as_ptr()) };
        assert_eq!(7, unsafe { var_getNum(v) });
    });
}

#[test]
fn add_nonexistent_no_crash() {
    with_exec!({
        unsafe { exec_add(sval("noexist"), nval(1), dummy()); }
    });
}

// ── exec_sub ─────────────────────────────────────────────────────────────────

#[test]
fn sub_basic() {
    with_exec!({
        unsafe {
            var_create(cstr("subvar").as_ptr(), 0);
            var_setNum(cstr("subvar").as_ptr(), 10);
            exec_sub(sval("subvar"), nval(3), dummy());
        }
        let v = unsafe { var_get(cstr("subvar").as_ptr()) };
        assert_eq!(7, unsafe { var_getNum(v) });
    });
}

#[test]
fn sub_result_negative() {
    with_exec!({
        unsafe {
            var_create(cstr("subneg").as_ptr(), 0);
            var_setNum(cstr("subneg").as_ptr(), 2);
            exec_sub(sval("subneg"), nval(10), dummy());
        }
        let v = unsafe { var_get(cstr("subneg").as_ptr()) };
        assert_eq!(-8, unsafe { var_getNum(v) });
    });
}

// ── exec_mul ─────────────────────────────────────────────────────────────────

#[test]
fn mul_basic() {
    with_exec!({
        unsafe {
            var_create(cstr("mulvar").as_ptr(), 0);
            var_setNum(cstr("mulvar").as_ptr(), 4);
            exec_mul(sval("mulvar"), nval(3), dummy());
        }
        let v = unsafe { var_get(cstr("mulvar").as_ptr()) };
        assert_eq!(12, unsafe { var_getNum(v) });
    });
}

#[test]
fn mul_by_zero() {
    with_exec!({
        unsafe {
            var_create(cstr("mulzero").as_ptr(), 0);
            var_setNum(cstr("mulzero").as_ptr(), 100);
            exec_mul(sval("mulzero"), nval(0), dummy());
        }
        let v = unsafe { var_get(cstr("mulzero").as_ptr()) };
        assert_eq!(0, unsafe { var_getNum(v) });
    });
}

// ── exec_div ─────────────────────────────────────────────────────────────────

#[test]
fn div_basic() {
    with_exec!({
        unsafe {
            var_create(cstr("divvar").as_ptr(), 0);
            var_setNum(cstr("divvar").as_ptr(), 12);
            exec_div(sval("divvar"), nval(4), dummy());
        }
        let v = unsafe { var_get(cstr("divvar").as_ptr()) };
        assert_eq!(3, unsafe { var_getNum(v) });
    });
}

#[test]
fn div_truncates() {
    with_exec!({
        unsafe {
            var_create(cstr("divtrunc").as_ptr(), 0);
            var_setNum(cstr("divtrunc").as_ptr(), 7);
            exec_div(sval("divtrunc"), nval(2), dummy());
        }
        let v = unsafe { var_get(cstr("divtrunc").as_ptr()) };
        assert_eq!(3, unsafe { var_getNum(v) });
    });
}

#[test]
fn div_by_zero_no_crash_value_unchanged() {
    with_exec!({
        unsafe {
            var_create(cstr("divz").as_ptr(), 0);
            var_setNum(cstr("divz").as_ptr(), 10);
            exec_div(sval("divz"), nval(0), dummy());
        }
        let v = unsafe { var_get(cstr("divz").as_ptr()) };
        assert_eq!(10, unsafe { var_getNum(v) });
    });
}

// ── exec_label ───────────────────────────────────────────────────────────────

#[test]
fn label_registers_label() {
    with_exec!({
        unsafe { exec_label(sval("myLabel"), nval(0), dummy()); }
        assert!(unsafe { cf_findLabel(cstr("myLabel").as_ptr()) } >= 0);
    });
}

#[test]
fn label_duplicate_no_crash() {
    with_exec!({
        unsafe {
            exec_label(sval("dupLabel"), nval(0), dummy());
            exec_label(sval("dupLabel"), nval(0), dummy());
        }
        assert!(unsafe { cf_findLabel(cstr("dupLabel").as_ptr()) } >= 0);
    });
}

// ── exec_out ─────────────────────────────────────────────────────────────────

#[test]
fn out_num_no_crash() {
    with_exec!({
        unsafe { exec_out(nval(42), nval(0), dummy()); }
    });
}

#[test]
fn out_string_no_crash() {
    with_exec!({
        unsafe { exec_out(sval("hello world"), nval(0), dummy()); }
    });
}
