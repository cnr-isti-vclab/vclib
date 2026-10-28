// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <benchmark_runner.h>
#include <cli_options.h>

#include "load_matrices.h"
#include "test_bounding_box.h"
#include "test_building.h"
#include "test_compact.h"
#include "test_copy.h"
#include "test_normals.h"
#include "test_transform.h"
#include "test_traversal.h"
#include "types.h"

#include <filesystem>
#include <iostream>
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

    // Parse mesh-specific args from extraArgs
    int gridRows = 0;
    int gridCols = 0;
    for (size_t i = 0; i < opts.extraArgs.size(); ++i) {
        if (opts.extraArgs[i] == "--grid" && i + 2 < opts.extraArgs.size()) {
            gridRows = std::stoi(opts.extraArgs[i + 1]);
            gridCols = std::stoi(opts.extraArgs[i + 2]);
            i += 2;
        }
    }

    std::string meshPath;
    if (!opts.positionalArgs.empty()) {
        meshPath = opts.positionalArgs[0];
    }
    else if (gridRows == 0) {
#ifdef VCLIB_EXAMPLE_MESHES_PATH
        meshPath = VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj";
#else
        gridRows = 500;
        gridCols = 500;
#endif
    }

    std::string      datasetName;
    Eigen::MatrixX3d V;
    Eigen::MatrixX3i F;

    try {
        if (gridRows > 0) {
            datasetName = std::format("grid_{}x{}", gridRows, gridCols);
            std::println(
                "Generating synthetic grid {}x{}...", gridRows, gridCols);
            auto grid = bench::makeGridMatrices(gridRows, gridCols);
            V         = std::move(grid.vertices);
            F         = std::move(grid.faces);
        }
        else {
            datasetName = fs::path(meshPath).filename().string();
            std::println("Loading mesh matrices from: {}", meshPath);
            auto mats = bench::loadMeshMatrices(meshPath);
            V         = std::move(mats.vertices);
            F         = std::move(mats.faces);
        }
    }
    catch (const std::exception& e) {
        std::println(stderr, "Error loading dataset: {}", e.what());
        return 1;
    }

    nlohmann::json metadata = {
        {"vertices", V.rows()},
        {"faces",    F.rows()}
    };

    std::println("==================================================");
    std::println("VCLib Mesh Benchmark [Version: V1 (Baseline)]");
    std::println(
        "Dataset: {} ({} vertices, {} faces)", datasetName, V.rows(), F.rows());
    std::println("Iterations: {}, Warmup: {}", opts.iterations, opts.warmup);
    std::println("==================================================");

    bench::BenchmarkRunner runner(
        "v1", datasetName, metadata, opts.iterations, opts.warmup);
    bench::Mesh mesh;

    // Building
    bench::benchmarkBuilding(runner, V, F, mesh, opts.iterations);

    // High-level library algorithms
    bench::benchmarkBoundingBox(runner, mesh, opts.iterations);
    bench::benchmarkTraversal(runner, mesh, opts.iterations);
    bench::benchmarkNormals(runner, mesh, opts.iterations);
    bench::benchmarkTransform(runner, mesh, opts.iterations);

    // Element management & containers
    bench::benchmarkCopy(runner, mesh, opts.iterations);
    bench::benchmarkDeleteAndCompact(runner, mesh, opts.iterations);

    std::println("==================================================");
    std::println("Benchmark completed successfully.");

    if (!opts.jsonPath.empty()) {
        runner.saveJson(opts.jsonPath);
    }

    return 0;
}
