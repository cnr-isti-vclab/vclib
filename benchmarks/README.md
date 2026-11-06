# VCLib benchmarks

Run the following commands from the repository root to build the Release mesh
benchmark executable:

```sh
cmake --preset vclib-core-benchmarks
cmake --build out/build/vclib-core-benchmarks --target vclib-core-benchmark-mesh
```

## Selecting a mesh type

Use `--mesh-type` to choose the representation at runtime:

```sh
out/build/vclib-core-benchmarks/benchmarks/core/mesh/vclib-core-benchmark-mesh \
    --mesh-type trimesh-indexed --grid 1000 1000 --json report.json
```

| CLI name | C++ type |
|----------|----------|
| `trimesh` (default) | `vcl::TriMesh` |
| `trimeshf` | `vcl::TriMeshf` |
| `trimesh-indexed` | `vcl::TriMeshIndexed` |
| `trimesh-indexedf` | `vcl::TriMeshIndexedf` |

The same option applies when passing a mesh file instead of `--grid`.
All four types are compiled into one executable; selection happens outside the
measured operations. An unsupported mesh type is an error.

The `--config` JSON file also accepts a `mesh_type` key:

```json
{
    "mesh_type": "trimesh-indexed",
    "grid": [1000, 1000],
    "include": ["stat/*"],
    "iterations": 10
}
```

The command line overrides the configuration; without either, `trimesh` is used.
The selected type is printed and saved in the report's `metadata.mesh_type`.
Use `--help` for all options and `--list` to inspect the selected benchmarks
without loading a dataset.

## Adding benchmarks

The [mesh benchmarks](core/mesh) follow the library's algorithm groups.
Register a generic lambda with `BENCH`, or `BENCH_SLOW` for a benchmark excluded
by default:

```cpp
BENCH("stat/bounding_box/bounding_box",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() { return vcl::boundingBox(m); });
    });
```

[State and Context](core/mesh/benchmark.h) are templated on the mesh type.
Registration instantiates each benchmark for every type listed in
[types.h](core/mesh/types.h). Keep helpers generic too, and use `s.skip(reason)`
when a dataset does not satisfy a benchmark's requirements.

See the [comparison scripts](../scripts/benchmarks/README.md) for comparing
reports, binaries, or branches and running regression tests.
