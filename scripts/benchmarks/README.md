# Benchmark comparison scripts

Run the following commands from the repository root. See the
[benchmark documentation](../../benchmarks/README.md) for building the executable
and configuring datasets and mesh types.

## Compare branches

[compare_branches.py](compare_branches.py) builds the baseline branch in a
dedicated worktree and the current working tree, runs both, saves JSON reports
under `scripts/benchmarks/results/`, and compares them:

```sh
python scripts/benchmarks/compare_branches.py main \
    --mesh-type trimeshf --grid 1000 1000
```

The mesh types are `trimesh` (default), `trimeshf`, `trimesh-indexed`, and
`trimesh-indexedf`. Both executables receive the same type.
With `--config config.json`, the configuration's `mesh_type` is used unless
overridden by `--mesh-type`. Benchmark filters, exclusions, and dataset options
are forwarded as well. Use `--help` for all options.

When a type is explicitly requested through CLI or configuration, both reports
must confirm it in `metadata.mesh_type`. An older executable that ignores the
option or omits this metadata is rejected rather than silently benchmarking the
wrong representation.

## Compare binaries or reports

[compare.py](compare.py) can run two existing binaries:

```sh
python scripts/benchmarks/compare.py --bin-a ./baseline --bin-b ./current \
    --mesh-type trimesh-indexed --args="--grid 1000 1000" --iterations 10
```

`--mesh-type` is forwarded to both binaries and checked against both reports.
Passing it inside `--args` is also supported and verified.
When `--args` contains `--config`, its `mesh_type` is verified too, unless
overridden by a CLI mesh type.

Alternatively, compare saved reports:

```sh
python scripts/benchmarks/compare.py baseline.json current.json \
    --label-a baseline --label-b current
```

Comparisons without an explicit mesh type remain compatible with older reports.
When the reports have different metadata, including different mesh types, both
sets of metadata are displayed.

## Regression tests

After building the benchmark executable:

```sh
python -m unittest discover -s scripts/benchmarks -p 'test_*.py'
```

The tests cover all four mesh types on generated grids and a mesh file, registry
consistency, CLI/config precedence, invalid options, metadata, and the comparison
scripts. CLI tests use the executable built by the benchmark preset; set
`VCLIB_MESH_BENCHMARK` to use another path. CLI tests are skipped if the executable
does not exist.
