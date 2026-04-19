#!/usr/bin/env python3
# Copyright (c) 2026 seesee010
# Master test runner: unit tests (CMake/CTest) + syntax tests.

import os
import subprocess
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.dirname(SCRIPT_DIR)
BUILD_DIR = os.path.join(REPO_ROOT, "build_test")
SYNTAX_RUNNER = os.path.join(SCRIPT_DIR, "syntax", "run_syntax_tests.py")


def section(title):
    print(f"\n{'='*60}\n  {title}\n{'='*60}")


def run(cmd, **kwargs):
    print(f"  $ {' '.join(cmd)}")
    return subprocess.run(cmd, **kwargs)


def run_unit_tests():
    section("UNIT TESTS (CMake / CTest)")

    r = run(["cmake", "-B", BUILD_DIR, "-S", REPO_ROOT, "-DCMAKE_BUILD_TYPE=Debug"])
    if r.returncode != 0:
        print("ERROR: cmake configure failed")
        return False

    r = run(["cmake", "--build", BUILD_DIR, "--parallel"])
    if r.returncode != 0:
        print("ERROR: cmake build failed")
        return False

    r = run(["ctest", "--output-on-failure", "--test-dir", BUILD_DIR])
    return r.returncode == 0


def run_syntax_tests():
    section("SYNTAX TESTS")
    r = subprocess.run([sys.executable, SYNTAX_RUNNER])
    return r.returncode == 0


def main():
    unit_ok = run_unit_tests()
    syntax_ok = run_syntax_tests()

    section("SUMMARY")
    print(f"  Unit tests:   {'PASS' if unit_ok   else 'FAIL'}")
    print(f"  Syntax tests: {'PASS' if syntax_ok else 'FAIL'}")
    print()

    if unit_ok and syntax_ok:
        print("All tests passed.")
        return 0
    print("Some tests FAILED.")
    return 1


if __name__ == "__main__":
    sys.exit(main())
