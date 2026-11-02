import contextlib
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


MESH_TYPES = ("trimesh", "trimeshf", "trimesh-indexed", "trimesh-indexedf")
DEFAULT_BINARY = (
    compare_branches.REPO_ROOT
    / compare_branches.BUILD_SUBDIR
    / compare_branches.BENCH_BIN_SUBDIR
    / (compare_branches.BENCH_TARGET + (".exe" if os.name == "nt" else ""))
)
BINARY = Path(os.environ.get("VCLIB_MESH_BENCHMARK", DEFAULT_BINARY)).resolve()


class ComparisonTests(unittest.TestCase):
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
        self.mesh = self.root / "tetra.off"
        self.mesh.write_text(
            "OFF\n4 4 0\n0 0 0\n1 0 0\n0 1 0\n0 0 1\n"
            "3 0 2 1\n3 0 1 3\n3 0 3 2\n3 1 2 3\n"
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
                report = self.run_report(
                    "--mesh-type", mesh_type, self.mesh, "--include-slow"
                )
                self.assertEqual(report["metadata"], {
                    "mesh_type": mesh_type, "vertices": 4, "faces": 4,
                })
                executed = {entry["name"] for entry in report["benchmarks"]}
                self.assertEqual(executed, names - {
                    "stat/topology/vertices_to_duplicate_by_wedge_tex_coords_count"
                })

    def test_all_types_on_grid(self):
        for mesh_type in MESH_TYPES:
            with self.subTest(mesh_type=mesh_type):
                report = self.run_report("--grid", 4, 5, "--mesh-type", mesh_type)
                self.assertEqual(report["metadata"], {
                    "mesh_type": mesh_type, "vertices": 20, "faces": 24,
                })
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
            "--config", self.config, "--mesh-type", "trimesh-indexedf"
        )
        self.assertEqual(report["metadata"]["mesh_type"], "trimesh-indexedf")

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

    def test_help_lists_mesh_types(self):
        result = self.invoke("--help")
        self.assertEqual(result.returncode, 0, result.stderr)
        for mesh_type in MESH_TYPES:
            self.assertIn(mesh_type, result.stdout)

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


if __name__ == "__main__":
    unittest.main()
