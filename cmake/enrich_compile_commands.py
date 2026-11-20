#!/usr/bin/env python3
#
# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

"""
Enriches compile_commands.json with explicit entries for header files
under vclib/bindings/include/.

CMake does not emit compilation commands for header files, which causes
clangd to fall back on heuristic guessing for header files that are not yet
cached in-memory, often picking unrelated translation units (e.g. benchmarks).
This script copies the compile flags of the corresponding binding module
(.cpp) to each header, providing clangd with exact compiler flags (Python,
pybind11, include directories, defines) portably across all platforms.
"""

import glob
import json
import os
import sys


def enrich_file(cc_path, repo_root):
    if os.path.isdir(cc_path):
        cc_path = os.path.join(cc_path, "compile_commands.json")

    if not os.path.isfile(cc_path):
        return

    try:
        with open(cc_path, "r", encoding="utf-8") as f:
            cmds = json.load(f)
    except Exception as e:
        print(f"[enrich_compile_commands] Warning: Could not read {cc_path}: {e}", file=sys.stderr)
        return

    existing_files = {entry.get("file") for entry in cmds}

    # Find reference commands for binding modules
    modules = ["core", "render", "external"]
    ref_cmds = {}
    for entry in cmds:
        f = entry.get("file", "")
        for m in modules:
            if m not in ref_cmds and (
                f"/vclib/bindings/src/vclib/bindings/{m}/" in f or
                f"/vclib/bindings/src/vclib/bindings/{m}.cpp" in f
            ):
                ref_cmds[m] = entry

    if not ref_cmds:
        return

    default_ref = ref_cmds.get("core") or next(iter(ref_cmds.values()))

    bindings_include = os.path.join(repo_root, "vclib", "bindings", "include")
    if not os.path.isdir(bindings_include):
        return

    headers = glob.glob(os.path.join(bindings_include, "**", "*.h"), recursive=True)
    headers.extend(glob.glob(os.path.join(bindings_include, "**", "*.hpp"), recursive=True))

    added = 0
    for h in headers:
        abs_h = os.path.abspath(h)
        if abs_h in existing_files:
            continue

        mod_ref = default_ref
        for m in modules:
            if f"/{m}/" in abs_h and m in ref_cmds:
                mod_ref = ref_cmds[m]
                break

        entry = dict(mod_ref)
        entry["file"] = abs_h
        ref_file = mod_ref.get("file", "")

        if "command" in entry and ref_file:
            entry["command"] = entry["command"].replace(ref_file, abs_h)
        if "arguments" in entry and ref_file:
            entry["arguments"] = [abs_h if arg == ref_file else arg for arg in entry["arguments"]]

        cmds.append(entry)
        existing_files.add(abs_h)
        added += 1

    if added > 0:
        try:
            with open(cc_path, "w", encoding="utf-8") as f:
                json.dump(cmds, f, indent=2)
            print(f"[enrich_compile_commands] Added {added} header entries to {cc_path}")
        except Exception as e:
            print(f"[enrich_compile_commands] Warning: Could not write {cc_path}: {e}", file=sys.stderr)


def main():
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    target_paths = sys.argv[1:]

    if not target_paths:
        target_paths = [os.path.join(repo_root, "compile_commands.json")]

    for path in target_paths:
        enrich_file(path, repo_root)


if __name__ == "__main__":
    main()
