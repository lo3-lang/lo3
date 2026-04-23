use super::*;

// Global array `g` persists between tests; use distinct indices per test to avoid
// cross-test contamination. Indices >= 50 are reserved for these tests.

fn num_val(n: i32) -> lo3_val { lo3_val::num(n) }

// ── g_isSet ───────────────────────────────────────────────────────────────────

#[test]
fn isset_initial_zero() {
    assert_eq!(0, unsafe { g_isSet(50) });
}

#[test]
fn isset_after_set_is_one() {
    unsafe { g_set(51, num_val(5)); }
    assert_eq!(1, unsafe { g_isSet(51) });
}

#[test]
fn isset_oob_high_returns_minus1() {
    assert_eq!(-1, unsafe { g_isSet(100) });
}

#[test]
fn isset_oob_low_returns_minus1() {
    assert_eq!(-1, unsafe { g_isSet(-1) });
}

// ── g_set / g_get ─────────────────────────────────────────────────────────────

#[test]
fn set_and_get_num() {
    unsafe {
        g_set(52, num_val(42));
        let v = g_get(52);
        assert_eq!(42, v.value.num);
    }
}

#[test]
fn set_oob_no_crash() {
    unsafe { g_set(200, num_val(1)); }
}

// ── g_getNum ─────────────────────────────────────────────────────────────────

#[test]
fn getnum_after_set() {
    unsafe { g_set(53, num_val(99)); }
    assert_eq!(99, unsafe { g_getNum(53) });
}

#[test]
fn getnum_oob_returns_minus1() {
    assert_eq!(-1, unsafe { g_getNum(100) });
}

// ── g_getType / g_setType ─────────────────────────────────────────────────────

#[test]
fn gettype_after_set_is_num() {
    unsafe { g_set(54, num_val(1)); }
    // g_set always writes chooseType=0 (num) for new entries
    assert_eq!(0, unsafe { g_getType(54) });
}

#[test]
fn settype_string() {
    unsafe {
        g_set(55, num_val(0));
        let type_val = lo3_val { r#type: TYPE_STRING, chooseType: 3, value: lo3_value { num: 3 } };
        g_setType(55, type_val);
    }
    assert_eq!(3, unsafe { g_getType(55) });
}

#[test]
fn settype_oob_returns_minus1() {
    let type_val = lo3_val::num(0);
    assert_eq!(-1, unsafe { g_setType(200, type_val) });
}

#[test]
fn settype_invalid_type_returns_minus1() {
    unsafe { g_set(56, num_val(0)); }
    let bad_type = lo3_val { r#type: TYPE_NUM, chooseType: 99, value: lo3_value { num: 99 } };
    assert_eq!(-1, unsafe { g_setType(56, bad_type) });
}

// ── g_getValue ────────────────────────────────────────────────────────────────

#[test]
fn getvalue_returns_placeholder_choosetype_minus1() {
    let v = unsafe { g_getValue(57) };
    assert_eq!(-1, v.chooseType);
}

// ── g_fasterInit ─────────────────────────────────────────────────────────────
// Indices 60-69 are reserved for these tests.

#[test]
fn faster_init_single_num_sets_value() {
    // @{60:$42} — numeric entry must store 42 at index 60
    unsafe {
        let mut line = b"@{60:$42}\0".to_vec();
        g_fasterInit(line.as_mut_ptr() as *mut i8);
        assert_eq!(42, g_getNum(60));
    }
}

#[test]
fn faster_init_single_num_marks_isset() {
    // g_isSet must flip to 1 after g_fasterInit writes the entry
    unsafe {
        let mut line = b"@{61:$7}\0".to_vec();
        g_fasterInit(line.as_mut_ptr() as *mut i8);
        assert_eq!(1, g_isSet(61));
    }
}

#[test]
fn faster_init_string_entry_sets_string_type() {
    // @{62:_Hello} — string entry must record chooseType=3
    unsafe {
        let mut line = b"@{62:_Hello}\0".to_vec();
        g_fasterInit(line.as_mut_ptr() as *mut i8);
        assert_eq!(3, g_getType(62));
    }
}

#[test]
fn faster_init_empty_braces_no_crash() {
    // @{} — nothing to initialise, must not crash or panic
    unsafe {
        let mut line = b"@{}\0".to_vec();
        g_fasterInit(line.as_mut_ptr() as *mut i8);
    }
}

#[test]
fn faster_init_negative_num() {
    // @{63:$-5} — atoi must handle the minus sign correctly
    unsafe {
        let mut line = b"@{63:$-5}\0".to_vec();
        g_fasterInit(line.as_mut_ptr() as *mut i8);
        assert_eq!(-5, g_getNum(63));
    }
}

#[test]
fn faster_init_last_valid_index_no_crash() {
    // index 99 is the last in-bounds slot (G_SIZE = 100)
    unsafe {
        let mut line = b"@{99:$1}\0".to_vec();
        g_fasterInit(line.as_mut_ptr() as *mut i8);
        assert_eq!(1, g_getNum(99));
    }
}
