import contextlib
import argparse
import io
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

import compare
import compare_branches


MESH_TYPES = ("trimesh", "trimeshf", "trimesh64", "trimesh64f")
DEFAULT_BINARY = (
    compare_branches.REPO_ROOT
    / compare_branches.BUILD_SUBDIR
    / compare_branches.BENCH_BIN_SUBDIR
    / (compare_branches.BENCH_TARGET + (".exe" if os.name == "nt" else ""))
)
BINARY = Path(os.environ.get("VCLIB_MESH_BENCHMARK", DEFAULT_BINARY)).resolve()


class ComparisonTests(unittest.TestCase):
    def test_mesh_type_precedence(self):
        parser = argparse.ArgumentParser()
        compare.add_mesh_type_options(parser)
        config = {
            "mesh_type": "common-config",
            "mesh_type_a": "a-config",
            "mesh_type_b": "b-config",
        }
        for argv, expected in (
            ([], ("a-config", "b-config")),
            (["--mesh-type", "common-cli"], ("common-cli", "common-cli")),
            (["--mesh-type-a", "a-cli"], ("a-cli", "b-config")),
            (["--mesh-type-b", "b-cli"], ("a-config", "b-cli")),
            (["--mesh-type", "common-cli", "--mesh-type-a", "a-cli"],
             ("a-cli", "common-cli")),
        ):
            with self.subTest(argv=argv):
                self.assertEqual(
                    compare.resolve_mesh_types(parser.parse_args(argv), config),
                    expected,
                )
        args = parser.parse_args([])
        self.assertEqual(compare.resolve_mesh_types(args, {}), (None, None))
        self.assertEqual(
            compare.resolve_mesh_types(args, {"mesh_type": "common"}),
            ("common", "common"),
        )
        self.assertEqual(
            compare.resolve_mesh_types(args, config, "forwarded"),
            ("forwarded", "forwarded"),
        )
        for invalid in ([], {"mesh_type_a": None}, {"mesh_type": ""}):
            with self.subTest(config=invalid), self.assertRaises(ValueError):
                compare.resolve_mesh_types(args, invalid)

    def test_require_mesh_type(self):
        compare.require_mesh_type(
            {"metadata": {"mesh_type": "trimeshf"}}, "trimeshf", "A"
        )
        for report in ({}, {"metadata": {"mesh_type": "trimesh"}}):
            with self.subTest(report=report), self.assertRaisesRegex(
                SystemExit, "Mesh type mismatch for A"
            ):
                compare.require_mesh_type(report, "trimeshf", "A")

    def test_comparison_displays_both_mesh_types(self):
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            compare.compare_reports(
                {"metadata": {"mesh_type": "trimesh"}},
                {"metadata": {"mesh_type": "trimeshf"}},
                label_a="baseline",
                label_b="current",
                use_color=False,
            )
        self.assertIn("Metadata (baseline): mesh_type: trimesh", output.getvalue())
        self.assertIn("Metadata (current): mesh_type: trimeshf", output.getvalue())

    def test_legacy_comparison(self):
        with contextlib.redirect_stdout(io.StringIO()):
            compare.compare_reports({}, {}, use_color=False)

    def test_coverage_warning(self):
        output = io.StringIO()
        entry = {"name": "shared", "time_median_ms": 1}
        with contextlib.redirect_stdout(output):
            compare.compare_reports(
                {"benchmarks": [entry, {**entry, "name": "unported"}]},
                {"benchmarks": [entry]},
                label_a="v1", label_b="v2", use_color=False,
            )
        self.assertIn("1 shared, 1 v1 only, 0 v2 only", output.getvalue())
        self.assertIn("across 1 benchmarks", output.getvalue())

    def test_comparison_rejects_ignored_mesh_type(self):
        def write_legacy_report(command, **kwargs):
            report_path = command[command.index("--json") + 1]
            Path(report_path).write_text(json.dumps({"metadata": {}}))

        with (
            mock.patch.object(sys, "argv", [
                "compare.py", "--bin-a", "old", "--bin-b", "new",
                "--mesh-type", "trimeshf",
            ]),
            mock.patch.object(
                compare.subprocess, "run", side_effect=write_legacy_report
            ),
            contextlib.redirect_stdout(io.StringIO()),
            self.assertRaisesRegex(SystemExit, "reported 'missing'"),
        ):
            compare.main()

    def test_comparison_checks_side_b_independently(self):
        def write_report(command, **kwargs):
            report_path = command[command.index("--json") + 1]
            actual_type = "trimesh-indexed" if command[0] == "v1" else "trimesh"
            Path(report_path).write_text(json.dumps({
                "metadata": {"mesh_type": actual_type}
            }))

        with (
            mock.patch.object(sys, "argv", [
                "compare.py", "--bin-a", "v1", "--bin-b", "v2",
                "--mesh-type-a", "trimesh-indexed", "--mesh-type-b", "trimesh64",
            ]),
            mock.patch.object(compare.subprocess, "run", side_effect=write_report),
            contextlib.redirect_stdout(io.StringIO()),
            self.assertRaisesRegex(SystemExit, "Mesh type mismatch for B"),
        ):
            compare.main()

    def test_branches_forward_cli_and_config_mesh_types(self):
        for override in (None, "trimesh-indexed"):
            with self.subTest(override=override), tempfile.TemporaryDirectory() as tmp:
                root = Path(tmp)
                config = root / "config.json"
                config.write_text(json.dumps({
                    "mesh_type": "trimeshf", "grid": [4, 5]
                }))
                argv = [
                    "compare_branches.py", "baseline", "--no-build",
                    "--config", str(config),
                ]
                if override is not None:
                    argv += ["--mesh-type", override]
                with (
                    mock.patch.object(sys, "argv", argv),
                    mock.patch.object(compare_branches, "RESULTS_DIR", root),
                    mock.patch.object(
                        compare_branches, "current_branch_name", return_value="current"
                    ),
                    mock.patch.object(
                        compare_branches, "prepare_worktree", return_value=root
                    ),
                    mock.patch.object(
                        compare_branches, "find_binary", return_value=BINARY
                    ),
                    mock.patch.object(compare_branches, "git", return_value=""),
                    mock.patch.object(compare_branches, "run"),
                    mock.patch.object(compare_branches, "record_provenance"),
                    mock.patch.object(compare_branches, "run_benchmark") as run,
                    contextlib.redirect_stdout(io.StringIO()),
                ):
                    compare_branches.main()
                self.assertEqual(run.call_count, 2)
                for call in run.call_args_list:
                    self.assertEqual(call.args[5], override or "trimeshf")
                    forwarded = call.args[1]
                    self.assertIn("--config", forwarded)
                    if override is not None:
                        index = forwarded.index("--mesh-type")
                        self.assertEqual(forwarded[index + 1], override)

    def test_branches_forward_different_types(self):
        pairs = (
            ("trimesh-indexed", "trimesh"),
            ("trimesh-indexedf", "trimeshf"),
            ("trimesh", "trimesh64"),
            ("trimeshf", "trimesh64f"),
        )
        for type_a, type_b in pairs:
            for use_config in (False, True):
                with self.subTest(pair=(type_a, type_b), config=use_config):
                    with tempfile.TemporaryDirectory() as tmp:
                        root = Path(tmp)
                        config = root / "config.json"
                        config.write_text(json.dumps({
                            "mesh_type_a": type_a, "mesh_type_b": type_b,
                            "grid": [4, 5],
                        }))
                        argv = ["compare_branches.py", "baseline", "--no-build"]
                        if use_config:
                            argv += ["--config", str(config)]
                        else:
                            argv += ["--mesh-type-a", type_a, "--mesh-type-b", type_b]
                        with (
                            mock.patch.object(sys, "argv", argv),
                            mock.patch.object(compare_branches, "RESULTS_DIR", root),
                            mock.patch.object(
                                compare_branches, "current_branch_name",
                                return_value="current",
                            ),
                            mock.patch.object(
                                compare_branches, "prepare_worktree", return_value=root
                            ),
                            mock.patch.object(
                                compare_branches, "find_binary", return_value=BINARY
                            ),
                            mock.patch.object(compare_branches, "git", return_value=""),
                            mock.patch.object(compare_branches, "run"),
                            mock.patch.object(compare_branches, "record_provenance"),
                            mock.patch.object(compare_branches, "run_benchmark") as run,
                            contextlib.redirect_stdout(io.StringIO()),
                        ):
                            compare_branches.main()
                        self.assertEqual(run.call_count, 2)
                        for call, expected in zip(run.call_args_list, (type_a, type_b)):
                            self.assertEqual(call.args[5], expected)
                            forwarded = call.args[1]
                            self.assertEqual(forwarded[-2:], ["--mesh-type", expected])

    def test_provenance_is_persisted(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            cache = root / compare_branches.BUILD_SUBDIR / "CMakeCache.txt"
            cache.parent.mkdir(parents=True)
            cache.write_text(
                "// Comment\nCMAKE_BUILD_TYPE:STRING=Release\n"
                "CMAKE_CXX_FLAGS:STRING=-march=native\n"
                "VCLIB_EXCLUDE_BENCHMARKS_TARGETS:BOOL=OFF\n"
            )
            binary = root / "binary"
            binary.write_bytes(b"test-binary")
            report_path = root / "report.json"
            report_path.write_text(json.dumps({"benchmarks": []}))
            config_path = root / "config.json"
            config_path.write_text(json.dumps({"mesh_type_b": "trimesh64"}))
            with mock.patch.object(
                compare_branches, "git", side_effect=["abc123", " M source.cpp"]
            ):
                compare_branches.record_provenance(
                    report_path, root, binary, "main", True,
                    ["--config", config_path, "--mesh-type", "trimesh64"], 1, None,
                )
            report = json.loads(report_path.read_text())
            provenance = report["provenance"]
            self.assertEqual(provenance["source_commit"], "abc123")
            self.assertTrue(provenance["source_dirty"])
            self.assertTrue(provenance["reused_binary"])
            self.assertEqual(len(provenance["binary_sha256"]), 64)
            self.assertEqual(provenance["config"], {"mesh_type_b": "trimesh64"})
            self.assertEqual(
                provenance["build_cache"]["CMAKE_CXX_FLAGS"], "-march=native"
            )
            self.assertEqual(report["benchmarks"], [])

    def test_branches_reject_ignored_mesh_type(self):
        with tempfile.TemporaryDirectory() as tmp:
            report = Path(tmp) / "report.json"
            report.write_text(json.dumps({"metadata": {}}))
            with (
                mock.patch.object(compare_branches, "run"),
                self.assertRaisesRegex(SystemExit, "reported 'missing'"),
            ):
                compare_branches.run_benchmark(
                    BINARY, ["--mesh-type", "trimeshf"], 1, report, None, "trimeshf"
                )


@unittest.skipUnless(BINARY.is_file(), "Build the mesh benchmark first")
class MeshCliTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.report = self.root / "report.json"
        self.config = self.root / "config.json"
        self.mesh = self.root / "tetra.obj"
        self.mesh.write_text(
            "v 0 0 0\nv 1 0 0\nv 0 1 0\nv 0 0 1\n"
            "f 1 3 2\nf 1 2 4\nf 1 4 3\nf 2 3 4\n"
        )

    def invoke(self, *args):
        return subprocess.run(
            [str(BINARY), *(str(arg) for arg in args)],
            capture_output=True, text=True, check=False,
        )

    def run_report(self, *args):
        result = self.invoke(
            *args, "--iterations", 1, "--no-warmup", "--json", self.report
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return json.loads(self.report.read_text())

    def assert_metadata(self, report, mesh_type, vertices, faces):
        self.assertEqual(report["version"], "v2")
        metadata = dict(report["metadata"])
        self.assertIsInstance(metadata.pop("seed"), int)
        self.assertEqual(metadata, {
            "mesh_type": mesh_type, "mesh_generation": "v2",
            "vertices": vertices, "faces": faces,
            "scalar_type": "float" if mesh_type.endswith("f") else "double",
            "scalar_bits": 32 if mesh_type.endswith("f") else 64,
            "reference_kind": "index",
            "reference_bits": 64 if "64" in mesh_type else 32,
        })

    def test_all_types_have_same_registry_and_run_all_file_benchmarks(self):
        baseline = None
        for mesh_type in MESH_TYPES:
            with self.subTest(mesh_type=mesh_type):
                result = self.invoke("--mesh-type", mesh_type, "--list", "--include-slow")
                self.assertEqual(result.returncode, 0, result.stderr)
                names = {
                    line.removesuffix(" (slow)")
                    for line in result.stdout.splitlines()
                }
                self.assertTrue(names)
                if baseline is None:
                    baseline = names
                self.assertEqual(names, baseline)
                # The positional file must also work after the type option.
                result = self.invoke(
                    "--mesh-type", mesh_type, self.mesh, "--include-slow",
                    "--iterations", 1, "--no-warmup", "--json", self.report
                )
                self.assertEqual(
                    result.returncode, 0, result.stdout + result.stderr
                )
                report = json.loads(self.report.read_text())
                self.assert_metadata(report, mesh_type, 4, 4)
                executed = {entry["name"] for entry in report["benchmarks"]}
                # Benchmarks that need components missing from the dataset
                # mesh are skipped and reported on stdout.
                skipped = {
                    line.split()[1]
                    for line in result.stdout.splitlines()
                    if line.startswith("Skipping ")
                }
                self.assertTrue(executed)
                self.assertFalse(executed & skipped)
                self.assertEqual(executed | skipped, names)

    def test_all_types_on_grid(self):
        for mesh_type in MESH_TYPES:
            with self.subTest(mesh_type=mesh_type):
                report = self.run_report("--grid", 4, 5, "--mesh-type", mesh_type)
                self.assert_metadata(report, mesh_type, 20, 24)
                self.assertTrue(report["benchmarks"])

    def test_default_and_config_precedence(self):
        report = self.run_report("--grid", 4, 5, "--filter", "mesh/copy")
        self.assertEqual(report["metadata"]["mesh_type"], "trimesh")
        self.config.write_text(json.dumps({
            "mesh_type": "trimeshf", "grid": [4, 5], "include": ["mesh/copy"]
        }))
        report = self.run_report("--config", self.config)
        self.assertEqual(report["metadata"]["mesh_type"], "trimeshf")
        report = self.run_report(
            "--config", self.config, "--mesh-type", "trimesh64f"
        )
        self.assertEqual(report["metadata"]["mesh_type"], "trimesh64f")

    def test_invalid_type_and_missing_value_fail(self):
        for args, message in (
            (("--mesh-type", "invalid", "--list"), "Unknown mesh type"),
            (("--mesh-type",), "--mesh-type requires"),
            (("--mesh-type", "--list"), "--mesh-type requires"),
            (("--mesh-type", "", "--list"), "Unknown mesh type"),
        ):
            with self.subTest(args=args):
                result = self.invoke(*args)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(message, result.stderr)
        self.config.write_text(json.dumps({"mesh_type": "invalid"}))
        result = self.invoke("--config", self.config, "--list")
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("Unknown mesh type", result.stderr)

    def test_empty_dataset_is_rejected(self):
        empty_mesh = self.root / "empty.obj"
        empty_mesh.write_text("# No vertices or faces\n")
        result = self.invoke(empty_mesh, "--filter", "mesh/copy")
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("dataset has no vertices or faces", result.stderr)

    def test_help_lists_mesh_types(self):
        result = self.invoke("--help")
        self.assertEqual(result.returncode, 0, result.stderr)
        for mesh_type in MESH_TYPES:
            self.assertIn(mesh_type, result.stdout)
        self.assertNotIn("trimesh-indexed", result.stdout)

    def test_comparison_script_forwards_mesh_type(self):
        self.config.write_text(json.dumps({
            "mesh_type": "trimeshf", "grid": [4, 5], "include": ["mesh/copy"]
        }))
        for type_args in (
            ["--mesh-type", "trimeshf", "--args=--grid 4 5 --filter mesh/copy"],
            ["--args=--grid 4 5 --filter mesh/copy --mesh-type trimeshf"],
            [f"--args=--config '{self.config}'"],
            [f"--args=--config '{self.config}' --mesh-type trimesh",
             "--mesh-type", "trimeshf"],
        ):
            with self.subTest(type_args=type_args):
                result = subprocess.run(
                    [
                        sys.executable, str(Path(compare.__file__)),
                        "--bin-a", str(BINARY), "--bin-b", str(BINARY),
                        "--iterations", "1", "--no-color", *type_args,
                    ],
                    capture_output=True, text=True, check=False,
                )
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn("mesh_type: trimeshf", result.stdout)

    def test_comparison_script_different_types(self):
        self.config.write_text(json.dumps({
            "mesh_type_a": "trimeshf", "mesh_type_b": "trimesh64f",
            "grid": [4, 5], "include": ["mesh/copy"],
        }))
        for options in (
            ["--mesh-type-a", "trimeshf", "--mesh-type-b", "trimesh64f",
             "--args=--grid 4 5 --filter mesh/copy"],
            [f"--args=--config '{self.config}'"],
            ["--mesh-type", "trimesh", "--mesh-type-a", "trimeshf",
             "--mesh-type-b", "trimesh64f", f"--args=--config '{self.config}'"],
        ):
            with self.subTest(options=options):
                result = subprocess.run(
                    [sys.executable, str(Path(compare.__file__)),
                     "--bin-a", str(BINARY), "--bin-b", str(BINARY),
                     "--iterations", "1", "--no-color", *options],
                    capture_output=True, text=True, check=False,
                )
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn("Metadata (A):", result.stdout)
                self.assertIn("Metadata (B):", result.stdout)
                self.assertIn("mesh_type: trimeshf", result.stdout)
                self.assertIn("mesh_type: trimesh64f", result.stdout)


if __name__ == "__main__":
    unittest.main()
