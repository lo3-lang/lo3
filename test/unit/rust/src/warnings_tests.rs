use super::*;
use std::ffi::CString;

fn cstr(s: &str) -> CString { CString::new(s).unwrap() }

// All tests here only check that the functions don't crash (no return value).
// Output goes to stderr, which is fine in test runs.

#[test]
fn warn_basic_no_crash() {
    unsafe { lo3_warn(cstr("test warning").as_ptr(), cstr("ctx").as_ptr()); }
}

#[test]
fn warn_null_context_no_crash() {
    unsafe { lo3_warn(cstr("no context").as_ptr(), std::ptr::null()); }
}

#[test]
fn warn_empty_message_no_crash() {
    // Empty msg triggers the internal lo3_error path — must not crash.
    unsafe { lo3_warn(cstr("").as_ptr(), cstr("ctx").as_ptr()); }
}

#[test]
fn warn_with_line_number_no_crash() {
    unsafe {
        currentLine = 42;
        lo3_warn(cstr("line aware warning").as_ptr(), cstr("file.lo3").as_ptr());
        currentLine = 0;
    }
}

#[test]
fn error_basic_no_crash() {
    unsafe { lo3_error(cstr("test error").as_ptr(), cstr("ctx").as_ptr()); }
}

#[test]
fn error_null_context_no_crash() {
    unsafe { lo3_error(cstr("no ctx error").as_ptr(), std::ptr::null()); }
}

#[test]
fn error_empty_message_no_crash() {
    // Empty msg triggers lo3_warn fallback — must not crash.
    unsafe { lo3_error(cstr("").as_ptr(), cstr("ctx").as_ptr()); }
}

#[test]
fn error_with_line_number_no_crash() {
    unsafe {
        currentLine = 7;
        lo3_error(cstr("line aware error").as_ptr(), cstr("here").as_ptr());
        currentLine = 0;
    }
}
