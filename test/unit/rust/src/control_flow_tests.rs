use super::*;
use std::ffi::CString;

fn setup() { unsafe { reset_cf(); } }
fn cstr(s: &str) -> CString { CString::new(s).unwrap() }

macro_rules! with_cf {
    ($body:block) => {{ setup(); $body; }};
}

// IMPORTANT: cf_addLabel stores raw pointers into cf.names[].
// Every CString whose pointer is passed to cf_addLabel MUST be bound to a
// let binding that lives for the full duration of the test — temporaries are
// dropped at the end of the statement, leaving a dangling pointer in cf.names.

// ── cf_findLabel ──────────────────────────────────────────────────────────────

#[test]
fn findlabel_empty_returns_minus1() {
    with_cf!({
        assert_eq!(-1, unsafe { cf_findLabel(cstr("any").as_ptr()) });
    });
}

#[test]
fn findlabel_after_add() {
    with_cf!({
        let alpha = cstr("alpha");
        unsafe { cf_addLabel(alpha.as_ptr(), 10); }
        assert!(unsafe { cf_findLabel(alpha.as_ptr()) } >= 0);
    });
}

#[test]
fn findlabel_unknown_returns_minus1() {
    with_cf!({
        let known = cstr("known");
        unsafe { cf_addLabel(known.as_ptr(), 5); }
        assert_eq!(-1, unsafe { cf_findLabel(cstr("unknown").as_ptr()) });
    });
}

#[test]
fn findlabel_multiple() {
    with_cf!({
        let l1 = cstr("l1");
        let l2 = cstr("l2");
        let l3 = cstr("l3");
        unsafe {
            cf_addLabel(l1.as_ptr(), 1);
            cf_addLabel(l2.as_ptr(), 2);
            cf_addLabel(l3.as_ptr(), 3);
        }
        assert!(unsafe { cf_findLabel(l1.as_ptr()) } >= 0);
        assert!(unsafe { cf_findLabel(l2.as_ptr()) } >= 0);
        assert!(unsafe { cf_findLabel(l3.as_ptr()) } >= 0);
        assert_eq!(-1, unsafe { cf_findLabel(cstr("l4").as_ptr()) });
    });
}

// ── cf_addLabel ───────────────────────────────────────────────────────────────

#[test]
fn addlabel_returns_zero_on_success() {
    with_cf!({
        let lbl = cstr("newlabel");
        assert_eq!(0, unsafe { cf_addLabel(lbl.as_ptr(), 42) });
    });
}

#[test]
fn addlabel_duplicate_returns_minus1() {
    with_cf!({
        let dup = cstr("dup");
        unsafe { cf_addLabel(dup.as_ptr(), 1); }
        assert_eq!(-1, unsafe { cf_addLabel(dup.as_ptr(), 2) });
    });
}

#[test]
fn addlabel_increments_count() {
    with_cf!({
        let a = cstr("a");
        let b = cstr("b");
        assert_eq!(0, unsafe { cf.nextFreePos });
        unsafe { cf_addLabel(a.as_ptr(), 0); }
        assert_eq!(1, unsafe { cf.nextFreePos });
        unsafe { cf_addLabel(b.as_ptr(), 0); }
        assert_eq!(2, unsafe { cf.nextFreePos });
    });
}

#[test]
fn addlabel_full_returns_minus1() {
    with_cf!({
        let labels: Vec<CString> = (0..ARRAY_SIZE - 1)
            .map(|i| CString::new(format!("lbl{i}")).unwrap())
            .collect();
        for lbl in &labels {
            unsafe { cf_addLabel(lbl.as_ptr(), 0); }
        }
        let overflow = cstr("overflow");
        assert_eq!(-1, unsafe { cf_addLabel(overflow.as_ptr(), 999) });
    });
}

// ── cf_getPos ─────────────────────────────────────────────────────────────────

#[test]
fn getpos_known_label() {
    with_cf!({
        let pos_test = cstr("pos_test");
        unsafe { cf_addLabel(pos_test.as_ptr(), 100); }
        assert_eq!(100, unsafe { cf_getPos(pos_test.as_ptr()) });
    });
}

#[test]
fn getpos_unknown_returns_minus1() {
    with_cf!({
        assert_eq!(-1, unsafe { cf_getPos(cstr("ghost").as_ptr()) });
    });
}

#[test]
fn getpos_multiple() {
    with_cf!({
        let x = cstr("x");
        let y = cstr("y");
        unsafe {
            cf_addLabel(x.as_ptr(), 11);
            cf_addLabel(y.as_ptr(), 22);
        }
        assert_eq!(11, unsafe { cf_getPos(x.as_ptr()) });
        assert_eq!(22, unsafe { cf_getPos(y.as_ptr()) });
    });
}

// ── cf_getCursorPos ───────────────────────────────────────────────────────────

#[test]
fn getcursorpos_always_minus1() {
    assert_eq!(-1, unsafe { cf_getCursorPos() });
}

// ── cf_jumpToLabel ────────────────────────────────────────────────────────────

#[test]
fn jumptolabel_nonexistent_returns_minus1() {
    with_cf!({
        assert_eq!(-1, unsafe { cf_jumpToLabel(cstr("noexist").as_ptr()) });
    });
}

#[test]
fn jumptolabel_existing_returns_zero() {
    with_cf!({
        let jmp = cstr("jmp_target");
        let tmp = unsafe { libc::tmpfile() };
        assert!(!tmp.is_null());
        unsafe {
            openFile = tmp;
            cf_addLabel(jmp.as_ptr(), 0);
            let r = cf_jumpToLabel(jmp.as_ptr());
            libc::fclose(tmp);
            openFile = std::ptr::null_mut();
            assert_eq!(0, r);
        }
    });
}

// ── rush / isWarped flags ─────────────────────────────────────────────────────

#[test]
fn rush_initially_false() {
    with_cf!({
        assert_eq!(FALSE, unsafe { rush });
    });
}

#[test]
fn iswarped_initially_false() {
    with_cf!({
        assert_eq!(FALSE, unsafe { isWarped });
    });
}
