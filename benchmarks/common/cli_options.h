// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BENCHMARKS_CLI_OPTIONS_H
#define VCL_BENCHMARKS_CLI_OPTIONS_H

#include <print>
#include <string>
#include <vector>

namespace bench {

struct CommonOptions
{
    std::string              jsonPath;
    int                      iterations    = 5;
    bool                     warmup        = true;
    bool                     helpRequested = false;
    std::vector<std::string> positionalArgs;
    std::vector<std::string> extraArgs;
};

inline CommonOptions parseCommonOptions(int argc, char** argv)
{
    CommonOptions opts;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--json" && i + 1 < argc) {
            opts.jsonPath = argv[++i];
        }
        else if (arg == "--iterations" && i + 1 < argc) {
            opts.iterations = std::stoi(argv[++i]);
        }
        else if (arg == "--no-warmup") {
            opts.warmup = false;
        }
        else if (arg == "-h" || arg == "--help") {
            opts.helpRequested = true;
        }
        else if (!arg.starts_with("-")) {
            opts.positionalArgs.push_back(arg);
        }
        else {
            opts.extraArgs.push_back(arg);
            if (i + 1 < argc && !std::string(argv[i + 1]).starts_with("-")) {
                opts.extraArgs.push_back(argv[++i]);
            }
        }
    }

    return opts;
}

inline void printCommonHelp(
    const char*        prog,
    const std::string& description,
    const std::string& extraUsage = "")
{
    std::println("{}\n", description);
    std::println("Usage: {} [options] {}", prog, extraUsage);
    std::println("Standard Benchmark Options:");
    std::println(
        "  --json <path>         Output benchmark results to a JSON report");
    std::println(
        "  --iterations <N>      Number of measurement iterations (default: "
        "5)");
    std::println(
        "  --no-warmup           Disable the initial warmup execution");
    std::println("  -h, --help            Display this help message");
}

} // namespace bench

#endif // VCL_BENCHMARKS_CLI_OPTIONS_H
