# Coding Guidelines

This document defines the coding standard for lo3-core.
All contributions must follow these rules.

---

## C Standard

Use **C11** exclusively.
Do not upgrade or downgrade the standard.
Do not use compiler-specific extensions.
`sizeof()` is allowed.

---

## Indentation and Spacing

- Use **tabs**, not spaces.
- One tab equals 8 spaces visually.
- Use spaces around operators:

```c
int a = b;       /* correct */
int a=b;         /* wrong   */
```

---

## Braces

Always use braces for `if`, `else`, `for`, `while`, etc. — even for a single statement.
The opening brace goes on its own line:

```c
if (abc)
{
	/* ... */
}
else
{
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

Avoid empty `if`/`else` blocks.
If you must acknowledge a condition without acting, write a `todo` comment inside the block (see [Todo Comments](#todo-comments)).

If an empty block is truly intentional and no action is needed, write it inline:

```c
if (abc) { }
```

No blank lines are required before or after an empty inline block (though exceptions exist).

Stay consistent within a file: either `{}` or `{ }` — do not mix.

---

## Code Grouping and Blank Lines

Group related lines together.
Add a blank line before and after `if`/loop blocks as a general rule.

```c
b = 10;
a = 10;

if (a)
{
	/* ... */
}

if (b)
{
	/* ... */
}
```

Short, dense assignment groups are an exception — they may appear without blank lines between them:

```c
b = 10;
a = 10; /* ok, not much code to separate */
```

If an `if` immediately follows code on the very next line with no logical gap, a blank line is still preferred but not always required — use judgement and keep it readable.

---

## Return Values

Cast the return value to `(void)` when you intentionally do not check it, but **only** when you are sure you do not need to:

```c
(void)printf("hello\n");
(void)fclose(fp);
```

I/O operations (stdio, file operations) generally do not need their return value checked — cast them.

Heap allocations (`malloc`, `calloc`, `realloc`) **must** be checked:

```c
buf = malloc(size);
if (buf == NULL)
{
	lo3_error("allocation failed", "");
	return -1;
}
```

---

## Error Handling

- Return `-1` when a function has been called incorrectly or encounters a fatal condition it can report.
- Not all return types allow `-1` (e.g. `char`-returning functions) — use judgement.
- Always check for errors at system boundaries.
- Use the project error methods:
  - `lo3_error()` — fatal/hard error
  - `lo3_warn()` — non-fatal warning
  - `PRINTLI` — debug output from the interpreter (not yet implemented)

---

## Nesting

**Maximum nesting depth: 4 levels** inside a function body.
Each `{}` block counts as one level.
A `switch` body is one level; `case` bodies inside the switch do not add an extra level.

```c
void example(void)
{
	/* level 1 */
	if (a)
	{
		/* level 2 */
		for (int i = 0; i < n; i++)
		{
			/* level 3 */
			if (b)
			{
				/* level 4 — maximum */
			}
		}
	}
}
```

If you exceed 4 levels, restructure: extract helper functions or invert conditions to reduce depth.

---

## Goto

Use `goto` only to jump **forward** (downward) within the same function.
The primary use is cleanup before returning:

```c
void sampleFunc(void)
{
	if (bad)
		goto err;

	/* ... */

err:
	/* cleanup */
}
```

A "public" cleanup label that jumps out of a function is allowed only with a clear reason.
Never jump backwards or into another function.

---

## Naming

### Files
Use kebab-case: `lo3-getLine.c`, `control-flow.c`.

### Macros / Defines
ALL_CAPS: `LO3_MAX_SIZE`, `TRUE`, `FALSE`.

### Constants
First letter uppercase, then camelCase: `MaxBufferSize`.

### Variables
camelCase: `currentLine`, `lastLineOffset`.

One-character names are allowed, but use them only when the meaning is obvious in context.
Never use random or meaningless names (`sjdi`, `abc123`, etc.).

Well-known abbreviations are allowed:
- `buf` — buffer
- `fp` — file pointer
- `stack` — custom stack structure
- etc.

### Functions
Prefix with the semantic group (usually derived from the file name), then an underscore, then camelCase:

```
<group>_<actionName>()
```

Examples: `lo3_parse()`, `var_create()`, `pars_resv()`.

---

## Comments

Single-line comments use `//`:

```c
int x = 0; /* wrong for single line */
int x = 0; // correct
```

Multi-line comments use `/* */` with a `*` on every body line.
Do **not** write text on the opening `/*` line:

```c
/*
* First line of comment.
* Second line of comment.
*/
```

Use only as many `*` lines as needed.

---

## Todo Comments

```c
// todo: <short description>
// ///// More Informations /////
// <longer explanation, and if needed a step-by-step guide>
```

This is the only case where a multi-line block is written with `//` instead of `/* */`.
Always use this exact format for todos — do not deviate.

---

## Header Guards

Prefer `#pragma once` for all headers.
Traditional `#ifndef` guards are allowed but should not be used by default.

---

## Section Dividers in Headers

General-purpose header files that are used by multiple source files must divide their sections with:

```c
///// <file name or section name> /////
```

This is required for multi-file-purpose headers only.

---

## Boolean Values

Use `TRUE` and `FALSE` from `cf-define.h` where semantically appropriate (flag variables, boolean conditions).
Integer `1` and `0` are acceptable for arithmetic or non-boolean contexts.

---

## Assembly

For a small number of low-level lines, use `asm()` inline.
For larger register-heavy routines, bind a separate `.S` file instead of writing it in `.c`.
Assembly files follow the same kebab-case naming convention.

---

## Preprocessor

Use the C preprocessor freely.
Conditional compilation, macros, and include guards are all encouraged where appropriate.
