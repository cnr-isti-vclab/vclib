#!/usr/bin/env python3
"""
Format all CMake files in the repository using gersemi.

Usage:
    python scripts/format_cmake_with_gersemi.py
    python scripts/format_cmake_with_gersemi.py --dry-run
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

# Directories that should not be traversed while searching files to format.
SKIP_DIR_NAMES = {
    ".git",
    ".idea",
    ".qtcreator",
    ".vscode",
    "__pycache__",
    "build",
    "cmake-build-debug",
    "cmake-build-release",
    "cmake-build-relwithdebinfo",
}


def is_cmake_file(path: Path) -> bool:
    """Return True when the path is a CMake source file."""
    if path.name == "CMakeLists.txt":
        return True

    name_lower = path.name.lower()

    # Explicitly include CMake template files.
    if name_lower.endswith(".cmake.in"):
        return True

    # Include regular CMake modules.
    if name_lower.endswith(".cmake"):
        return True

    if name_lower == "cmakelists.txt.in":
        return True

    return False


def find_cmake_files(root: Path) -> list[Path]:
    """Collect CMake files recursively from root."""
    files: list[Path] = []

    for candidate in root.rglob("*"):
        if candidate.is_dir() and candidate.name in SKIP_DIR_NAMES:
            continue

        if not candidate.is_file():
            continue

        if any(part in SKIP_DIR_NAMES for part in candidate.parts):
            continue

        if is_cmake_file(candidate):
            files.append(candidate)

    return sorted(files)


def format_with_gersemi(files: list[Path]) -> int:
    """Format files in place and return the number of failures."""
    failures = 0

    for file_path in files:
        cmd = ["gersemi", "--in-place", str(file_path)]
        result = subprocess.run(cmd, check=False)
        if result.returncode != 0:
            failures += 1
            print(f"[ERROR] gersemi failed on: {file_path}", file=sys.stderr)
        else:
            print(f"[OK] {file_path}")

    return failures


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Format all CMake files in the repository with gersemi"
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print the files that would be formatted without changing them",
    )

    args = parser.parse_args()

    if shutil.which("gersemi") is None:
        print(
            "Error: 'gersemi' is not available in PATH. "
            "Install it first, e.g.: pip install gersemi",
            file=sys.stderr,
        )
        return 1

    script_dir = Path(__file__).resolve().parent
    repo_root = script_dir.parent

    files = find_cmake_files(repo_root)

    if not files:
        print("No CMake files found.")
        return 0

    print(f"Repository root: {repo_root}")
    print(f"Found {len(files)} CMake files")

    if args.dry_run:
        for file_path in files:
            print(file_path)
        return 0

    failures = format_with_gersemi(files)

    if failures > 0:
        print(f"Completed with {failures} formatting errors.", file=sys.stderr)
        return 1

    print("Formatting completed successfully.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
