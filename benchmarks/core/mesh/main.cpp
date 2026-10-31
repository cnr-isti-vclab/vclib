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
#include <string>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    auto opts = bench::parseCommonOptions(argc, argv);

    if (opts.helpRequested) {
        bench::printCommonHelp(
            argv[0],
            "VCLib Core Mesh Benchmark",
            "[mesh_path] [--grid <rows> <cols>]");
        std::println("Mesh Specific Options:");
        std::println(
            "  --grid <rows> <cols>  Generate a synthetic triangulated grid");
        return 0;
    }

    nlohmann::json config;
    try {
        config = bench::loadConfig(opts);
    }
    catch (const std::exception& e) {
        std::println(stderr, "Error loading the configuration: {}", e.what());
        return 1;
    }

    const auto& registry = bench::Registry::instance();
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
    bench::Mesh mesh;

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
            mesh = vcl::loadMesh<bench::Mesh>(meshPath);
        }
    }
    catch (const std::exception& e) {
        std::println(stderr, "Error loading dataset: {}", e.what());
        return 1;
    }

    nlohmann::json metadata = {
        {"vertices", mesh.vertexCount()},
        {"faces",    mesh.faceCount()  }
    };

    std::println("==================================================");
    std::println("VCLib Mesh Benchmark [Version: V1 (Baseline)]");
    std::println(
        "Dataset: {} ({} vertices, {} faces)",
        datasetName,
        mesh.vertexCount(),
        mesh.faceCount());
    std::println(
        "Benchmarks: {}, Iterations: {}, Warmup: {}",
        selected.size(),
        opts.iterations,
        opts.warmup);
    std::println("==================================================");

    bench::BenchmarkRunner runner(
        "v1", datasetName, metadata, opts.iterations, opts.warmup);
    bench::Context context(std::move(mesh), datasetName, meshPath);

    for (const auto* entry : selected) {
        bench::State state(runner, context, entry->name, opts.iterations);
        entry->func(state);
    }

    std::println("==================================================");
    std::println("Benchmark completed successfully.");

    if (!opts.jsonPath.empty()) {
        runner.saveJson(opts.jsonPath);
    }

    return 0;
}
