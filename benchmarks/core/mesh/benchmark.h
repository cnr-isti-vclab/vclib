// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BENCH_BENCHMARK_H
#define BENCH_BENCHMARK_H

#include "compat.h"
#include "types.h"

#include <benchmark_registry.h>
#include <benchmark_runner.h>

#include <vclib/algorithms/mesh/update/topology.h>

#include <memory>
#include <print>
#include <string>

/**
 * Benchmarks organization
 *
 * The benchmark sources follow the tree of the algorithms of the library: for
 * each header `vclib/algorithms/mesh/<group>/<header>.h` there is a source file
 * `<group>/<header>.cpp` that registers the benchmarks of the functions of that
 * header. The name of each benchmark is
 * `<group>/<header>/<function_name_in_snake_case>`, e.g.
 * `stat/topology/is_water_tight`. Benchmarks that do not belong to the
 * algorithms (loading, copy...) are in the other directories.
 *
 * A benchmark is registered with the BENCH macro, that gets the name and a
 * function that receives a State:
 *
 * @code{.cpp}
 * BENCH("stat/bounding_box/bounding_box", [](bench::State& s) {
 *     const bench::Mesh& m = s.mesh();
 *     s.run([&]() { return vcl::boundingBox(m); });
 * });
 * @endcode
 *
 * The State gives access to the dataset and to some prepared meshes (that are
 * not part of the measurement), and measures the function given to run().
 */

namespace bench {

/**
 * @brief The dataset of the benchmarks and some meshes derived from it, that
 * are created only when requested.
 */
class Context
{
    Mesh        mMesh;
    std::string mDatasetName;
    std::string mMeshPath;

    std::unique_ptr<Mesh> mMeshWithTopology;

public:
    Context(Mesh mesh, std::string datasetName, std::string meshPath = "") :
            mMesh(std::move(mesh)), mDatasetName(std::move(datasetName)),
            mMeshPath(std::move(meshPath))
    {
    }

    /**
     * @brief The input mesh. It must not be modified by the benchmarks.
     */
    const Mesh& mesh() const { return mMesh; }

    const std::string& datasetName() const { return mDatasetName; }

    /**
     * @brief The path of the file from which the mesh has been loaded. It is
     * empty if the mesh has been generated.
     */
    const std::string& meshPath() const { return mMeshPath; }

    /**
     * @brief A copy of the input mesh, that the benchmark is free to modify.
     */
    Mesh copyMesh() const { return mMesh; }

    /**
     * @brief The input mesh with the topology (adjacencies) enabled and
     * updated. It is created at the first call and shared among the
     * benchmarks, therefore it must not be modified.
     */
    const Mesh& meshWithTopology()
    {
        if (!mMeshWithTopology) {
            mMeshWithTopology = std::make_unique<Mesh>(mMesh);
            enableTopologyComponents(*mMeshWithTopology);
            vcl::updatePerVertexAdjacentFaces(*mMeshWithTopology);
            vcl::updatePerVertexAdjacentVertices(*mMeshWithTopology);
            vcl::updatePerFaceAdjacentFaces(*mMeshWithTopology);
        }
        return *mMeshWithTopology;
    }
};

/**
 * @brief The state of a running benchmark: it allows to access the dataset and
 * to measure the function to benchmark.
 */
class State
{
    BenchmarkRunner& mRunner;
    Context&         mContext;
    std::string      mName;
    int              mIterations;

public:
    State(
        BenchmarkRunner& runner,
        Context&         context,
        std::string      name,
        int              iterations) :
            mRunner(runner), mContext(context), mName(std::move(name)),
            mIterations(iterations)
    {
    }

    const std::string& name() const { return mName; }

    const Mesh& mesh() const { return mContext.mesh(); }

    Mesh copyMesh() const { return mContext.copyMesh(); }

    const Mesh& meshWithTopology() { return mContext.meshWithTopology(); }

    const std::string& meshPath() const { return mContext.meshPath(); }

    /**
     * @brief Measures the given function. If it returns a value, the value is
     * kept alive to avoid that the call is optimized away.
     */
    template<typename Func>
    void run(Func&& func)
    {
        mRunner.run(mName, func, mIterations);
    }

    /**
     * @brief Measures the given function, calling the (not measured) setup
     * function before each execution. Useful for functions that modify their
     * input, to restore it.
     */
    template<typename Setup, typename Func>
    void run(Setup&& setup, Func&& func)
    {
        mRunner.runWithSetup(mName, setup, func, mIterations);
    }

    /**
     * @brief Notifies that the benchmark cannot be executed on this dataset.
     * No result is saved for the benchmark.
     */
    void skip(const std::string& reason) const
    {
        std::println("Skipping {:<45} ({})", mName, reason);
    }
};

using Registry = BenchmarkRegistry<State>;

class Registrar
{
public:
    Registrar(std::string name, Registry::Func func, bool slow = false)
    {
        Registry::instance().add(std::move(name), std::move(func), slow);
    }
};

} // namespace bench

#define BENCH_CONCAT_IMPL(a, b) a##b
#define BENCH_CONCAT(a, b)      BENCH_CONCAT_IMPL(a, b)

/// Registers a benchmark: BENCH(name, [](bench::State& s) { ... });
#define BENCH(NAME, ...)                                    \
    static const bench::Registrar BENCH_CONCAT(             \
        benchRegistrar,                                     \
        __LINE__)(NAME, __VA_ARGS__)

/// Registers a benchmark that runs only if requested (see --include-slow).
#define BENCH_SLOW(NAME, ...)                               \
    static const bench::Registrar BENCH_CONCAT(             \
        benchRegistrar,                                     \
        __LINE__)(NAME, __VA_ARGS__, true)

#endif // BENCH_BENCHMARK_H
