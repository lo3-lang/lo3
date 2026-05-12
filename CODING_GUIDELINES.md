# Coding Guidelines

This document defines the coding standard for this project.
All contributions must follow these rules.

---

## Standard

- **C11** only. Do not upgrade or downgrade.
- Do **not** use compiler-specific extensions or features.
  - Exception: `sizeof()` is allowed.
  - Exception: `asm()` is allowed for inline assembly (see Assembly section).
- The C preprocessor is allowed and encouraged where appropriate.

---

## Indentation

- Use **tabs**, not spaces.
- One tab = 8 spaces visual width.

---

## Spacing

Always put spaces around operators and after keywords:

```c
int a = b;       /* correct */
int a=b;         /* wrong   */
```

---

## Braces

Always use braces — never omit them, even for single-statement bodies.
Opening brace goes on the **same line** as the statement, closing brace on its own line:

```c
if (abc) {
	/* ... */
}
else {
	/* ... */
}
```

Functions follow the same rule:

```c
int main(void) {
	return 0;
}
```

Empty blocks use `{ }` or `{}` — pick one and stay consistent within a file:

```c
if (abc) { }
else { }
```

> **Note:** Prefer a `todo` comment over an intentionally empty block.
> Only use an empty block when you know exactly why it is empty.

---

## Blank Lines and Code Grouping

Put a blank line **before and after** every `if`, loop, or similar control-flow block.
There are exceptions:

- Short sequential assignments can be grouped without blank lines:

```c
b = 10;
a = 10;  /* ok — not much code, no need to separate */
```

- Related assignments followed by related ifs can be grouped together:

```c
b = 10;
a = 10;

if (a) { }
if (b) { }
```

- Empty `{ }` blocks do not need surrounding blank lines (though exceptions exist).

---

## Nesting

- Maximum nesting depth: **4 levels**.
- A pair of `{}` counts as one level.
- A `switch` body counts as one level; individual `case` bodies inside it do **not** add a level.
- If you exceed 4 levels, restructure — extract a helper function.

---

## Names

### Files
Kebab-case: `lo3-getLine.c`, `control-flow.c`.

### Defines / Macros
ALL_CAPS: `MAX_SIZE`, `TRUE`, `FALSE`.

### Variables
camelCase: `varName`, `filePointer`.

Single-character names are allowed but carry risk — the reader may not understand them.
Avoid random or meaningless names (`sjdi`, `xqz`).

Common short names that are always acceptable:
- `buf` — a buffer
- `fp` — file pointer
- `stack` — a manually managed stack
- Any other well-known abbreviation that is unambiguous in context

### Types
When you need a custom type, use the `_t` suffix: `myType_t`.

### Functions
`<module>_camelCaseName()`, where the module prefix matches the file name shortened to its semantic group:

```c
lo3_parse()    /* from lo3 / parsing */
var_create()   /* from variable.c    */
```

---

## `const` vs `#define`

Prefer `#define` over `const`.
Use `const` only when it is genuinely necessary (e.g., a pointer that must carry a type-qualified contract).

---

## Type Qualifiers

Use `restrict` when two pointer parameters can **never** alias, but only when it is truly the case:

```c
void copy(char *restrict dst, const char *restrict src);
```

Do not add `restrict` speculatively.

---

## Floating-Point Types

Prefer `double` over `float`.

Exception: when memory is critical — low-level or embedded code where every byte matters.
In those cases `float` (or even smaller types) is appropriate.

---

## `(void)` Cast

Cast the return value to `(void)` when you intentionally skip checking it,
but **only** when you are sure it is safe to ignore:

```c
(void)printf("hello\n");
(void)fclose(fp);
(void)fseek(fp, 0, SEEK_SET);
```

Rules:
- I/O and OS operations that return something: cast to `(void)` (if `printf` fails you likely cannot do I/O at all — it is not your problem).
- Heap allocations (`malloc`, `calloc`, `realloc`): **must** be checked — never cast to `(void)`.
- `free()` is already `void` — no cast needed.

---

## Error Handling

- Return `-1` when a function has been called incorrectly or when propagating an error makes sense.
- Do not always return `-1` — for functions returning `char` or other narrow types, use a type-appropriate sentinel.
- Use the project error helpers:
  - `lo3_error()` — fatal / hard errors
  - `lo3_warn()` — non-fatal warnings
  - `PRINTLI` — debug print (planned; outputs interpreter state for debugging)

---

## `goto`

Use `goto` for cleanup and early-exit patterns.
Rules:
- Only jump **forward** (down) in the function — never upward.
- "Public" cleanup at the end of a function is allowed when it provides a clear benefit.

```c
void sampleFunc(void) {
	if (TRUE) {
		goto err;
	}

	/* normal work */

err:
	/* cleanup */
}
```

Jumping forward to a return point at the bottom of a function is acceptable,
but only when there is a clear reason to do it that way.

---

## Comments

Single-line comments use `//` only. `/*` and `*/` are **not** allowed for single-line comments:

```c
// this is correct
/* this is wrong for a single-line comment */
```

Multi-line — use `/*` / `*/` with a leading `*` on every body line.
Write only as many lines as needed:

```c
/*
 * first line of explanation
 * second line of explanation
 */
```

Do **not** write on the `/*` opening line or the `*/` closing line.

### `todo` comments

```c
// todo: <short description>
// ///// More Informations /////
// <longer explanation and, if needed, a step-by-step guide>
```

This is the **only** case where a multi-line `//` comment block is used.
Always use this exact format for todos.

---

## Assembly

- A few lines of inline assembly: use `asm()`.
- Larger routines that need many registers or complex control flow: write a separate `.S` file and bind it.
- Do not mix sizeable assembly logic into `.c` files.

---

## Header Guards

Prefer `#pragma once`:

```c
#pragma once
```

You may use traditional `#ifndef` guards, but `#pragma once` is the preferred style.

---

## Boolean Values

Use `TRUE` and `FALSE` from `cf-define.h` wherever the context is boolean.
`1` and `0` are acceptable for purely numeric comparisons.

---

## Section Dividers in Headers

Multi-purpose header files (shared across several `.c` files) must divide their sections with:

```c
///// <section name> /////
```

Example:

```c
///// from var.c /////
```

This rule applies **only** to multi-purpose headers — single-purpose headers do not need dividers.
