# Coding Guidelines

This document defines the coding standard for this project.
All contributions must follow these rules.

---

## Standard

- **C11** — do not upgrade or downgrade.
- No compiler-specific extensions. `sizeof()` is allowed.
- The C preprocessor is allowed and encouraged.

---

## Indentation and Spacing

- Use **tabs**, not spaces. Tabs represent **8 spaces** visually.
- Always put spaces around operators:

```c
int a = b;       /* correct */
int a=b;         /* wrong   */
```

---

## Brace Style

Always use braces, even for single-statement bodies.
The opening brace goes on the **same line** as the statement.
`else` goes on its **own line** after the closing brace:

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
int main(void)
{
	return 0;
}
```

---

## Empty Blocks

Do **not** write empty `if`/`else` blocks without reason.
If a block is intentionally empty, create a `todo` comment explaining why instead.

If you genuinely need an empty block (rare), use `{ }` or `{}` — be consistent within a file:

```c
if (abc) { }
```

Empty blocks do **not** need the blank lines that normal `if` blocks require.

---

## Code Grouping and Blank Lines

Group related code together. Put a blank line before **and** after `if` blocks and loops.

```c
b = 10;
a = 10;

if (a) {
	/* ... */
}

if (b) {
	/* ... */
}
```

Short, clearly related assignments may be grouped without a blank line between them:

```c
b = 10;
a = 10; /* ok — related, small group */
if (a) {
	/* ... */
}
```

If an `if` immediately follows a short group of assignments, the blank line before the `if` may be omitted when the intent is obvious.

---

## Nesting

Maximum nesting depth is **4 levels**.

Counting rules:

- Each `{ }` block = 1 level.
- A `switch` body = 1 level.
- `case` bodies inside a `switch` do **not** add a level.

If you exceed 4 levels, restructure the code (helper functions, early returns via `goto`, etc.).

---

## `(void)` Cast

Cast the return value to `(void)` when you intentionally ignore it — but only when you are sure ignoring it is safe:

```c
(void)printf("hello\n");
(void)fclose(fp);
```

**I/O operations** (stdio, file operations) should always be cast to `(void)` when unchecked, because if I/O fails you usually cannot recover anyway.

**Heap allocations** (`malloc`, `calloc`, `realloc`) must **always** be checked — never cast them to `(void)`.

`free()` is already `void`; no cast is needed.

---

## Error Handling

- Always check for errors.
- Return `-1` when a function has been used wrongly or when propagating failure makes sense.
- Return type determines what error value to use — you cannot always return `-1` (e.g., a function returning `char`).
- Use the project's error/warning utilities:
  - `lo3_error()` — fatal error
  - `lo3_warn()` — non-fatal warning
  - `PRINTLI` — debug print for the interpreter (not yet implemented)
- `void` functions use early `return` on error; whether to convert to `int`-returning depends on whether the caller can act on the error.

---

## `goto`

Use `goto` for **cleanup** within a function. You may only jump **down**, never up or sideways.

```c
void sampleFunc(void)
{
	if (TRUE) {
		goto err;
	}

	/* ... normal path ... */

err:
	/* cleanup */
}
```

Jumping forward to a return point (exiting the function early) is allowed when there is a clear reason.

---

## Comments

Use `//` for single-line comments:

```c
int x = 0; /* wrong — use // for single-line */
// correct
```

Use `/* */` for multi-line comments, with a `*` on every body line:

```c
/*
 * First line of explanation.
 * Second line of explanation.
 */
```

Only use as many lines as you need.

### TODO comments

The **only** exception to the single-line rule is the `todo` format:

```c
// todo: <short description>
// ///// More Informations /////
// <longer explanation and step-by-step guide if needed>
```

Always use this exact signature for todos.

---

## Header Files

- Prefer `#pragma once` over traditional `#ifndef` guards.
  `#ifndef` guards are allowed but not preferred.

---

## `TRUE` / `FALSE`

`TRUE` and `FALSE` are defined in `cf-define.h`.
Use them when a variable or expression is semantically boolean.
`1` and `0` are fine for integer arithmetic.

---

## Section Dividers

Use the `///// name /////` divider to separate sections in **multi-purpose header files** (headers included by many different source files):

```c
///// var.c /////
/* declarations for var.c */

///// parsing.c /////
/* declarations for parsing.c */
```

Do not use this in regular `.c` files or single-purpose headers.

---

## Naming Conventions

| Kind | Style | Example |
|---|---|---|
| Files | kebab-case | `lo3-getLine.c`, `control-flow.c` |
| Macros / defines | ALL_CAPS | `MAX_SIZE`, `TRUE` |
| Constants | PascalCase | `DefaultBuffer` |
| Variables | camelCase | `lineCount`, `isWarped` |
| Functions | `prefix_camelCase` | `lo3_parse()`, `var_create()` |

The function prefix is derived from the source file name, shortened to a semantic group
(e.g., functions in `variable.c` use `var_`, functions for lo3 features use `lo3_`).

### Variable names

- Use clear, descriptive names.
- Single-character names are allowed but carry a readability risk — use them only when the scope is tiny and the meaning is obvious.
- Common short names that are always acceptable: `buf` (buffer), `fp` (file pointer), `stack`.
- **Never** use random characters or meaningless names like `sjdi`, `abc123`, `qqq`.

---

## Type Conventions

### Prefer `#define` over `const`

Use `#define` for constants. Use `const` only when it is genuinely necessary
(e.g., when a pointer to a `const`-qualified type is required by an API).

### Use official `_t` types

Prefer the standard integer types from `<stdint.h>` and `<stddef.h>` over plain `int`, `long`, etc.:

```c
#include <stdint.h>

uint8_t  byte;
int32_t  value;
size_t   length;
```

Use the plain types only when the standard demands them (e.g., `int main(void)`).

### `restrict`

Use the `restrict` qualifier when two pointers in a function signature **cannot** alias and the compiler can benefit from knowing that.
Only use it when it is genuinely true — do not add it speculatively.

### `double` over `float`

Prefer `double` for floating-point values.
Use `float` only when memory is critical (low-level, performance-sensitive code where every byte matters).

---

## Assembly

- A few lines of inline assembly: use `asm()`.
- A larger routine that still fits in a `.c` file: consider a separate `.S` file bound to the build.
- Heavy register usage or a whole routine in assembly: write a `.S` file instead of a `.c` file.

---

## Optimizations

Apply optimizations where they matter.
Bit-shifts and other low-level tricks are appropriate in low-level, performance-critical code.
Do not optimize prematurely in high-level logic.
