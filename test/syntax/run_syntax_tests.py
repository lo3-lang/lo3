#!/usr/bin/env python3
# Copyright (c) 2026 seesee010
# Syntax test runner: runs each .lo3 file and compares stdout to .expected

import os
import subprocess
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.dirname(os.path.dirname(SCRIPT_DIR))
BIN = os.path.join(REPO_ROOT, "bin", "lo3")

def run_syntax_tests():
    if not os.path.isfile(BIN):
        print(f"ERROR: binary not found at {BIN}")
        print("Build it first: cmake -B build && cmake --build build")
        return False

    tests = sorted(f for f in os.listdir(SCRIPT_DIR) if f.endswith(".lo3"))
    if not tests:
        print("No .lo3 test files found.")
        return True

    passed = 0
    failed = 0
    errors = []

    for lo3_name in tests:
        base = lo3_name[:-4]
        lo3_path = os.path.join(SCRIPT_DIR, lo3_name)
        exp_path = os.path.join(SCRIPT_DIR, base + ".expected")

        if not os.path.isfile(exp_path):
            print(f"[SKIP] {base}  (no .expected file)")
            continue

        with open(exp_path) as f:
            expected = f.read()

        result = subprocess.run([BIN, lo3_path], capture_output=True, timeout=10)
        actual = result.stdout.decode("utf-8")

        if actual == expected and result.returncode == 0:
            print(f"[PASS] {base}")
            passed += 1
        else:
            print(f"[FAIL] {base}")
            if result.returncode != 0:
                print(f"       exit code: {result.returncode}")
            if actual != expected:
                exp_lines = expected.splitlines()
                act_lines = actual.splitlines()
                for i, (e, a) in enumerate(zip(exp_lines, act_lines)):
                    if e != a:
                        print(f"       line {i+1}: expected {repr(e)}")
                        print(f"                 actual   {repr(a)}")
                        break
                if len(exp_lines) != len(act_lines):
                    print(f"       expected {len(exp_lines)} lines, got {len(act_lines)}")
            failed += 1
            errors.append(base)

    total = passed + failed
    print()
    print(f"Syntax tests: {passed}/{total} passed", end="")
    if failed:
        print(f"  ({failed} failed: {', '.join(errors)})")
    else:
        print()
    return failed == 0


if __name__ == "__main__":
    ok = run_syntax_tests()
    sys.exit(0 if ok else 1)
