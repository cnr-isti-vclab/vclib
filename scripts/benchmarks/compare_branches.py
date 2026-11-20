#!/usr/bin/env python3
"""
VCLib Branch Benchmark Comparison

Builds the core mesh benchmark on the current working tree and on another
branch (checked out in a dedicated git worktree), runs both on the same input,
saves the two JSON reports in scripts/benchmarks/results/ and compares them
with compare.py.

The other branch is the baseline (A), the current one is the compared
version (B): a speedup > 1 means that the current branch is faster.

Examples:
    python scripts/benchmarks/compare_branches.py main
    python scripts/benchmarks/compare_branches.py main --mesh bunny.obj
    python scripts/benchmarks/compare_branches.py main --grid 2000 2000 -n 20
    python scripts/benchmarks/compare_branches.py main --filter 'stat/*'
    python scripts/benchmarks/compare_branches.py main --config my_bench.json
    python scripts/benchmarks/compare_branches.py main --mesh-type-a trimesh-indexed --mesh-type-b trimesh

Mesh type names are native to each executable; there is no implicit mapping.
--mesh-type sets the same name for both sides; --mesh-type-a/--mesh-type-b
select the baseline/current types independently. Config keys: mesh_type,
mesh_type_a, mesh_type_b. CLI options override the config.
Both reports must confirm the requested type; executables that silently ignore
the option are rejected. Provenance is saved alongside the benchmark results.
"""

import argparse
import datetime
import hashlib
import json
import os
import random
import re
import shutil
import subprocess
import sys
from pathlib import Path

from compare import add_mesh_type_options, require_mesh_type, resolve_mesh_types

SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parent.parent
RESULTS_DIR = SCRIPT_DIR / "results"
WORKTREES_DIR = REPO_ROOT / "out" / "worktrees"

BENCH_TARGET = "vclib-core-benchmark-mesh"
BUILD_SUBDIR = Path("out") / "build" / "vclib-core-benchmarks"
BENCH_BIN_SUBDIR = Path("benchmarks") / "core" / "mesh"

# Keep in sync with the "vclib-core-benchmarks" configure preset in
# CMakePresets.json.
CMAKE_DEFINES = {
    "CMAKE_BUILD_TYPE": "Release",
    "VCLIB_EXCLUDE_BENCHMARKS_TARGETS": "OFF",
    "VCLIB_COMPILE_WARNINGS_AS_ERRORS": "OFF",
}
if sys.platform == "win32":
    # /EHsc is an MSVC-specific flag. Do not pass it if compiling with GCC/MinGW or Clang++.
    cxx_is_gnu = any(
        tool in os.environ.get("CXX", "").lower() for tool in ("g++", "gcc", "clang++")
    ) or (not shutil.which("cl") and shutil.which("g++"))
    if not cxx_is_gnu:
        CMAKE_DEFINES["CMAKE_CXX_FLAGS"] = "/EHsc"
    else:
        # MinGW GCC implements C++23 <print> (std::print, std::println) terminal routines in libstdc++exp.a
        CMAKE_DEFINES["CMAKE_CXX_STANDARD_LIBRARIES"] = (
            "-lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -lstdc++exp"
        )


def run(cmd, cwd=None, capture=False):
    print(f"$ {' '.join(str(c) for c in cmd)}", flush=True)
    res = subprocess.run(
        [str(c) for c in cmd],
        cwd=cwd,
        check=True,
        text=True,
        stdout=subprocess.PIPE if capture else None,
    )
    return res.stdout.strip() if capture else None


def git(*args, cwd=REPO_ROOT, capture=True):
    return run(["git", *args], cwd=cwd, capture=capture)


def safe_name(name: str) -> str:
    return re.sub(r"[^A-Za-z0-9._-]+", "_", name)


def current_branch_name() -> str:
    name = git("branch", "--show-current")
    return name or "detached_" + git("rev-parse", "--short", "HEAD")


def prepare_worktree(ref: str) -> Path:
    try:
        git("worktree", "prune")
    except subprocess.CalledProcessError:
        pass

    commit = None
    for candidate in [ref, f"origin/{ref}"]:
        try:
            commit = git("rev-parse", "--verify", "--quiet", f"{candidate}^{{commit}}")
            break
        except subprocess.CalledProcessError:
            continue
    if not commit:
        sys.exit(f"Unknown branch or git ref: {ref}")
    path = WORKTREES_DIR / safe_name(ref)

    if (path / ".git").exists():
        git("checkout", "--detach", "-f", commit, cwd=path, capture=False)
    else:
        WORKTREES_DIR.mkdir(parents=True, exist_ok=True)
        try:
            git("worktree", "add", "--detach", "-f", path, commit, capture=False)
        except subprocess.CalledProcessError:
            git("worktree", "prune")
            git("worktree", "add", "--detach", "-f", path, commit, capture=False)
    return path


def build(source_dir: Path, jobs) -> Path:
    build_dir = source_dir / BUILD_SUBDIR

    configure = ["cmake", "-S", source_dir, "-B", build_dir]
    if shutil.which("ninja"):
        configure += ["-G", "Ninja"]
    configure += [f"-D{k}={v}" for k, v in CMAKE_DEFINES.items()]
    run(configure)

    compile_cmd = [
        "cmake",
        "--build",
        build_dir,
        "--config",
        "Release",
        "--target",
        BENCH_TARGET,
    ]
    if jobs:
        compile_cmd += ["-j", str(jobs)]
    run(compile_cmd)
    return build_dir


def find_binary(source_dir: Path) -> Path:
    base = source_dir / BUILD_SUBDIR / BENCH_BIN_SUBDIR
    for name in (
        BENCH_TARGET,
        BENCH_TARGET + ".exe",
        f"Release/{BENCH_TARGET}.exe",
        f"Release/{BENCH_TARGET}",
    ):
        p = base / name
        if p.exists():
            return p
    sys.exit(f"Benchmark executable not found in {base}")


def run_benchmark(
    binary: Path, dataset_args, iterations, json_path, cpu, mesh_type=None
):
    cmd = [binary, *dataset_args, "--json", json_path]
    if iterations is not None:
        cmd += ["--iterations", iterations]
    if cpu is not None:
        if shutil.which("taskset"):
            cmd = ["taskset", "-c", cpu, *cmd]
        else:
            print("Warning: taskset not found, ignoring --cpu")
    try:
        run(cmd)
    except subprocess.CalledProcessError:
        sys.exit(
            f"\nThe benchmark failed. If it printed 'No benchmark selected', "
            f"the selected benchmarks are not available in this branch: check "
            f"the filters and that their sources are enabled in "
            f"benchmarks/core/mesh/CMakeLists.txt."
        )
    if mesh_type is not None:
        with open(json_path) as f:
            require_mesh_type(json.load(f), mesh_type, str(binary))


def record_provenance(
    json_path: Path, source_dir: Path, binary: Path, ref: str,
    reused_binary: bool, dataset_args, iterations, cpu,
):
    cache_path = source_dir / BUILD_SUBDIR / "CMakeCache.txt"
    build_config = {}
    if cache_path.is_file():
        for line in cache_path.read_text().splitlines():
            if not line or line.startswith(("//", "#")) or "=" not in line:
                continue
            key, value = line.split("=", 1)
            key = key.split(":", 1)[0]
            if key.startswith(("CMAKE_", "VCLIB_")):
                build_config[key] = value
    else:
        print(f"Warning: build cache not found: {cache_path}")
    with open(binary, "rb") as f:
        digest = hashlib.sha256()
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            digest.update(chunk)
        binary_hash = digest.hexdigest()
    with open(json_path) as f:
        report = json.load(f)
    report["provenance"] = {
        "ref": ref,
        "source_commit": git("rev-parse", "HEAD", cwd=source_dir),
        "source_dirty": bool(git("status", "--porcelain", cwd=source_dir)),
        "binary_sha256": binary_hash,
        "reused_binary": reused_binary,
        "build_cache": build_config,
        "benchmark_args": [str(arg) for arg in dataset_args],
        "iterations_override": iterations,
        "cpu_requested": cpu,
        "config": None,
    }
    if "--config" in dataset_args:
        config_path = dataset_args[dataset_args.index("--config") + 1]
        with open(config_path) as f:
            report["provenance"]["config"] = json.load(f)
    with open(json_path, "w") as f:
        json.dump(report, f, indent=2)
        f.write("\n")


def main():
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("branch", help="Branch (or any git ref) to compare against")
    parser.add_argument(
        "--mesh", help="Mesh file used by the benchmarks (default: synthetic grid)"
    )
    parser.add_argument(
        "--grid",
        nargs=2,
        type=int,
        metavar=("ROWS", "COLS"),
        help="Size of the synthetic grid used when --mesh is not given "
        "(default: 1000 1000)",
    )
    add_mesh_type_options(parser)
    parser.add_argument(
        "--filter",
        action="append",
        default=[],
        metavar="PATTERN",
        help="Run only the benchmarks matching the pattern (repeatable)",
    )
    parser.add_argument(
        "--exclude",
        action="append",
        default=[],
        metavar="PATTERN",
        help="Do not run the benchmarks matching the pattern (repeatable)",
    )
    parser.add_argument(
        "--include-slow",
        action="store_true",
        help="Run also the benchmarks marked as slow",
    )
    parser.add_argument(
        "--config",
        help="JSON configuration file of the benchmarks (selection, dataset, "
        "iterations)",
    )
    parser.add_argument(
        "-n",
        "--iterations",
        type=int,
        help="Iterations (default: the value of the config file, or 10)",
    )
    parser.add_argument(
        "--seed",
        type=int,
        help="Random seed for benchmarks (default: random int, shared between branches)",
    )
    parser.add_argument("-j", "--jobs", type=int, help="Parallel build jobs")
    parser.add_argument("--cpu", help="Pin the benchmarks to this CPU (taskset)")
    parser.add_argument(
        "--no-build",
        action="store_true",
        help="Do not build, reuse the existing binaries",
    )
    parser.add_argument(
        "--remove-worktree",
        action="store_true",
        help="Remove the worktree of the other branch at the end "
        "(it is kept by default to speed up the next runs)",
    )
    parser.add_argument(
        "--markdown",
        "--md",
        dest="markdown",
        help="Path to write the markdown comparison report (default: scripts/benchmarks/results/<stamp>_<ref_a>_vs_<ref_b>.md)",
    )
    parser.add_argument("--no-color", action="store_true", help="Disable colors")
    args = parser.parse_args()

    config_has_dataset = False
    config_path = None
    config = {}
    if args.config:
        config_path = Path(args.config).resolve()
        if not config_path.is_file():
            sys.exit(f"Config file not found: {config_path}")
        with open(config_path) as f:
            config = json.load(f)
    try:
        mesh_type_a, mesh_type_b = resolve_mesh_types(args, config)
    except ValueError as e:
        parser.error(str(e))
    config_has_dataset = "mesh" in config or "grid" in config
    if args.iterations is None and "iterations" not in config:
        args.iterations = 10

    if args.mesh:
        mesh = Path(args.mesh).resolve()
        if not mesh.is_file():
            sys.exit(f"Mesh file not found: {mesh}")
        dataset_args = [mesh]
    elif args.grid:
        dataset_args = ["--grid", *args.grid]
    elif config_has_dataset:
        dataset_args = []  # the dataset is described by the config file
    else:
        dataset_args = ["--grid", 1000, 1000]

    for pattern in args.filter:
        dataset_args += ["--filter", pattern]
    for pattern in args.exclude:
        dataset_args += ["--exclude", pattern]
    if args.include_slow:
        dataset_args.append("--include-slow")
    if config_path:
        dataset_args += ["--config", config_path]

    seed = args.seed
    if seed is None and config_path:
        with open(config_path) as f:
            cfg = json.load(f)
            seed = cfg.get("seed")
    if seed is None:
        seed = random.randint(1, 2**31 - 1)

    dataset_args += ["--seed", str(seed)]
    print(f"Random seed: {seed}")

    this_name = current_branch_name()
    other_name = args.branch
    if safe_name(other_name) == safe_name(this_name):
        sys.exit("The branch to compare against must differ from the current one.")

    if git("status", "--porcelain"):
        print("Note: the current working tree has uncommitted changes (included).")

    other_dir = prepare_worktree(other_name)

    if not args.no_build:
        for label, src in ((other_name, other_dir), (this_name, REPO_ROOT)):
            print(f"\n>>> Building [{label}]")
            build(src, args.jobs)
    else:
        print(
            "Warning: --no-build reuses existing binaries; the recorded source "
            "commit and build cache do not prove which sources built them."
        )

    bin_other = find_binary(other_dir)
    bin_this = find_binary(REPO_ROOT)

    RESULTS_DIR.mkdir(parents=True, exist_ok=True)
    stamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    json_other = RESULTS_DIR / f"{stamp}_{safe_name(other_name)}.json"
    json_this = RESULTS_DIR / f"{stamp}_{safe_name(this_name)}.json"
    md_report = (
        Path(args.markdown).resolve()
        if args.markdown
        else RESULTS_DIR / f"{stamp}_{safe_name(other_name)}_vs_{safe_name(this_name)}.md"
    )

    for label, binary, json_path, source_dir, mesh_type in (
        (other_name, bin_other, json_other, other_dir, mesh_type_a),
        (this_name, bin_this, json_this, REPO_ROOT, mesh_type_b),
    ):
        print(f"\n>>> Running benchmark [{label}]")
        side_args = list(dataset_args)
        if mesh_type is not None:
            side_args += ["--mesh-type", mesh_type]
        run_benchmark(
            binary, side_args, args.iterations, json_path, args.cpu, mesh_type
        )
        record_provenance(
            json_path, source_dir, binary, label, args.no_build,
            side_args, args.iterations, args.cpu,
        )

    print("\n>>> Comparison\n")
    compare_cmd = [
        sys.executable,
        SCRIPT_DIR / "compare.py",
        json_other,
        json_this,
        "--label-a",
        other_name,
        "--label-b",
        this_name,
        "--markdown",
        str(md_report),
    ]
    if args.no_color or not sys.stdout.isatty():
        compare_cmd.append("--no-color")
    run(compare_cmd)

    print(f"\nReports saved in:\n  {json_other}\n  {json_this}\n  {md_report}")

    if args.remove_worktree:
        git("worktree", "remove", "--force", other_dir, capture=False)


if __name__ == "__main__":
    main()
