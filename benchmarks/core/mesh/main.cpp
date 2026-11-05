// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "benchmark.h"
#include "compat.h"
#include "types.h"

#include <benchmark_registry.h>
#include <benchmark_runner.h>
#include <cli_options.h>

#include <vclib/io.h>

#include <filesystem>
#include <print>
#include <random>
#include <string>

namespace fs = std::filesystem;

template<typename Mesh>
int runBenchmarks(
    const bench::CommonOptions& opts,
    const nlohmann::json&       config,
    std::string_view            meshType)
{
    const auto& registry = bench::Registry<Mesh>::instance();
    const auto  selected = registry.select(bench::makeFilter(opts));

    if (opts.listOnly) {
        for (const auto* entry : selected) {
            std::println("{}{}", entry->name, entry->slow ? " (slow)" : "");
        }
        return 0;
    }

    if (selected.empty()) {
        std::println(stderr, "No benchmark selected.");
        return 1;
    }

    // Dataset: the command line has the precedence over the config file
    int         gridRows = 0;
    int         gridCols = 0;
    std::string meshPath;

    for (size_t i = 0; i < opts.extraArgs.size(); ++i) {
        if (opts.extraArgs[i] == "--grid" && i + 2 < opts.extraArgs.size()) {
            gridRows = std::stoi(opts.extraArgs[i + 1]);
            gridCols = std::stoi(opts.extraArgs[i + 2]);
            i += 2;
        }
    }

    if (!opts.positionalArgs.empty()) {
        meshPath = opts.positionalArgs[0];
    }
    else if (gridRows == 0) {
        if (config.contains("mesh")) {
            meshPath = config["mesh"].get<std::string>();
        }
        else if (config.contains("grid")) {
            gridRows = config["grid"].at(0).get<int>();
            gridCols = config["grid"].at(1).get<int>();
        }
        else {
#ifdef VCLIB_EXAMPLE_MESHES_PATH
            meshPath = VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj";
#else
            gridRows = 500;
            gridCols = 500;
#endif
        }
    }

    std::string datasetName;
    Mesh        mesh;

    try {
        if (meshPath.empty()) {
            datasetName = std::format("grid_{}x{}", gridRows, gridCols);
            std::println(
                "Generating synthetic grid {}x{}...", gridRows, gridCols);
            bench::buildGridMesh(mesh, gridRows, gridCols);
        }
        else {
            datasetName = fs::path(meshPath).filename().string();
            std::println("Loading mesh from: {}", meshPath);
            mesh = vcl::loadMesh<Mesh>(meshPath);
        }
    }
    catch (const std::exception& e) {
        std::println(stderr, "Error loading dataset: {}", e.what());
        return 1;
    }

    const uint32_t seed = opts.seed.value_or(std::random_device{}());

    nlohmann::json metadata = {
        {"mesh_type", meshType          },
        {"vertices",  mesh.vertexCount()},
        {"faces",     mesh.faceCount()  },
        {"seed",      seed              }
    };

    std::println("==================================================");
    std::println("VCLib Mesh Benchmark [Version: V1 (Baseline)]");
    std::println("Mesh type: {}", meshType);
    std::println(
        "Dataset: {} ({} vertices, {} faces)",
        datasetName,
        mesh.vertexCount(),
        mesh.faceCount());
    std::println(
        "Benchmarks: {}, Iterations: {}, Warmup: {}, Seed: {}",
        selected.size(),
        opts.iterations,
        opts.warmup,
        seed);
    std::println("==================================================");

    bench::BenchmarkRunner runner(
        "v1", datasetName, metadata, opts.iterations, opts.warmup);
    bench::Context<Mesh> context(std::move(mesh), datasetName, meshPath, seed);

    for (const auto* entry : selected) {
        bench::State<Mesh> state(runner, context, entry->name, opts.iterations);
        entry->func(state);
    }

    std::println("==================================================");
    std::println("Benchmark completed successfully.");

    if (!opts.jsonPath.empty()) {
        runner.saveJson(opts.jsonPath);
    }

    return 0;
}

int main(int argc, char** argv)
{
    try {
        auto opts = bench::parseCommonOptions(
            argc,
            argv,
            {
                {"--grid",      2},
                {"--mesh-type", 1}
        });

        std::string supportedTypes;
        bench::forEachMeshType([&](auto, std::string_view name) {
            if (!supportedTypes.empty())
                supportedTypes += ", ";
            supportedTypes += name;
        });

        if (opts.helpRequested) {
            bench::printCommonHelp(
                argv[0],
                "VCLib Core Mesh Benchmark",
                "[mesh_path] [--grid <rows> <cols>] [--mesh-type <type>]");
            std::println("Mesh Specific Options:");
            std::println(
                "  --grid <rows> <cols>  Generate a synthetic triangulated "
                "grid");
            std::println(
                "  --mesh-type <type>   Mesh representation (default: "
                "trimesh)");
            std::println(
                "                       Supported: {}", supportedTypes);
            std::println(
                "                       Config key: \"mesh_type\"; CLI takes "
                "precedence");
            return 0;
        }

        const auto  config = bench::loadConfig(opts);
        std::string meshType;
        bool        meshTypeSet = false;
        for (size_t i = 0; i < opts.extraArgs.size(); ++i) {
            if (opts.extraArgs[i] == "--mesh-type") {
                meshType    = opts.extraArgs[++i];
                meshTypeSet = true;
            }
        }
        if (!meshTypeSet)
            meshType = config.value("mesh_type", std::string("trimesh"));

        bool matched = false;
        int  result  = 1;
        bench::forEachMeshType([&](auto type, std::string_view name) {
            if (meshType == name) {
                matched = true;
                result  = runBenchmarks<typename decltype(type)::type>(
                    opts, config, name);
            }
        });
        if (!matched) {
            std::println(
                stderr,
                "Unknown mesh type '{}'. Supported types: {}",
                meshType,
                supportedTypes);
        }
        return result;
    }
    catch (const std::exception& e) {
        std::println(stderr, "Error: {}", e.what());
        return 1;
    }
}
