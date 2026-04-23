//! FFI bindings to lo3-core C library + shared test helpers.
#![allow(non_snake_case, non_camel_case_types, dead_code, clippy::missing_safety_doc)]

// ── type constants (lo3_types enum values) ──────────────────────────────────
pub const TYPE_NUM: i32    = b'$' as i32;
pub const TYPE_STRING: i32 = b'_' as i32;
pub const TYPE_ARRAY: i32  = b'*' as i32;
pub const TYPE_VAR: i32    = b'%' as i32;

pub const FALSE: i8 = 0;
pub const TRUE:  i8 = 1;

pub const ARRAY_SIZE: usize = 254;
pub const G_SIZE:     usize = 100;

// ── lo3_value union ──────────────────────────────────────────────────────────
#[repr(C)]
pub union lo3_value {
    pub num:    i32,
    pub string: *mut u8,
    pub numD:   f64,
}

// ── lo3_val ──────────────────────────────────────────────────────────────────
#[repr(C)]
pub struct lo3_val {
    pub r#type:     i32,
    pub chooseType: i32,
    pub value:      lo3_value,
}

impl lo3_val {
    pub fn num(n: i32) -> Self {
        lo3_val { r#type: TYPE_NUM, chooseType: 0, value: lo3_value { num: n } }
    }
    pub fn string(s: *mut u8) -> Self {
        lo3_val { r#type: TYPE_STRING, chooseType: 3, value: lo3_value { string: s } }
    }
}

// ── lo3_label ────────────────────────────────────────────────────────────────
#[repr(C)]
pub struct lo3_label {
    pub names:       [*mut i8; ARRAY_SIZE],
    pub pos:         [i32;     ARRAY_SIZE],
    pub nextFreePos: i32,
}

// ── opaque lo3_var ───────────────────────────────────────────────────────────
pub enum lo3_var {}

// ── extern C ─────────────────────────────────────────────────────────────────
extern "C" {
    // globals
    pub static mut currentLine:  i32;
    pub static mut openFile:     *mut libc::FILE;
    pub static mut cf:           lo3_label;
    pub static mut rush:         i8;
    pub static mut isWarped:     i8;
    pub static mut rush_target:  *mut i8;

    // var.c
    pub fn var_init();
    pub fn var_freeAll();
    pub fn var_find(name: *const i8) -> i32;
    pub fn var_create(name: *const i8, r#type: i32);
    pub fn var_get(name: *const i8) -> *mut lo3_var;
    pub fn var_free(name: *const i8);
    pub fn var_setNum(name: *const i8, num: i32);
    pub fn var_setString(name: *const i8, string: *mut i8);
    pub fn var_getNum(var: *const lo3_var) -> i32;
    pub fn var_getString(var: *const lo3_var) -> *const i8;
    pub fn var_getType(var: *const lo3_var) -> i32;
    pub fn var_getValue(var: *const lo3_var) -> lo3_value;

    // global.c
    pub fn g_set(index: i32, val: lo3_val);
    pub fn g_get(index: i32) -> lo3_val;
    pub fn g_isSet(index: i32) -> i32;
    pub fn g_getNum(index: i32) -> i32;
    pub fn g_getType(index: i32) -> i32;
    pub fn g_setType(index: i32, r#type: lo3_val) -> i32;
    pub fn g_getValue(index: i32) -> lo3_val;
    pub fn g_fasterInit(line: *mut i8);

    // control-flow.c
    pub fn cf_findLabel(name: *const i8) -> i32;
    pub fn cf_addLabel(name: *const i8, pos: i32) -> i32;
    pub fn cf_getPos(name: *const i8) -> i32;
    pub fn cf_getCursorPos() -> i32;
    pub fn cf_jumpToLabel(name: *const i8) -> i32;

    // execute.c
    pub fn exec_new(a1: lo3_val, a2: lo3_val, array: *mut i8);
    pub fn exec_free(a1: lo3_val, a2: lo3_val, array: *mut i8);
    pub fn exec_asn(a1: lo3_val, a2: lo3_val, array: *mut i8);
    pub fn exec_add(a1: lo3_val, a2: lo3_val, array: *mut i8);
    pub fn exec_sub(a1: lo3_val, a2: lo3_val, array: *mut i8);
    pub fn exec_mul(a1: lo3_val, a2: lo3_val, array: *mut i8);
    pub fn exec_div(a1: lo3_val, a2: lo3_val, array: *mut i8);
    pub fn exec_label(a1: lo3_val, a2: lo3_val, array: *mut i8);
    pub fn exec_out(a1: lo3_val, a2: lo3_val, array: *mut i8);

    // warnings.c
    pub fn lo3_warn(msg: *const i8, context: *const i8);
    pub fn lo3_error(msg: *const i8, context: *const i8);
}

// ── shared helpers ────────────────────────────────────────────────────────────

/// Heap-duplicate a Rust str into a C string (malloc-compatible, safe to free() by C).
pub unsafe fn c_strdup(s: &str) -> *mut i8 {
    let cstr = std::ffi::CString::new(s).unwrap();
    libc::strdup(cstr.as_ptr())
}

/// Reset control-flow globals between tests.
pub unsafe fn reset_cf() {
    std::ptr::write_bytes(&raw mut cf, 0, 1);
    rush        = FALSE;
    isWarped    = FALSE;
    rush_target = std::ptr::null_mut();
}

// ── test modules ─────────────────────────────────────────────────────────────
#[cfg(test)] mod var_tests;
#[cfg(test)] mod global_tests;
#[cfg(test)] mod control_flow_tests;
#[cfg(test)] mod execute_tests;
#[cfg(test)] mod warnings_tests;
