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
