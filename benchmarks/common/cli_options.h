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
    bool                     iterationsSet = false;
    bool                     warmup        = true;
    bool                     helpRequested = false;
    bool                     listOnly      = false;
    bool                     includeSlow   = false;
    std::string              configPath;
    std::vector<std::string> includes; // --filter
    std::vector<std::string> excludes; // --exclude
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
            opts.iterations    = std::stoi(argv[++i]);
            opts.iterationsSet = true;
        }
        else if (arg == "--filter" && i + 1 < argc) {
            opts.includes.push_back(argv[++i]);
        }
        else if (arg == "--exclude" && i + 1 < argc) {
            opts.excludes.push_back(argv[++i]);
        }
        else if (arg == "--config" && i + 1 < argc) {
            opts.configPath = argv[++i];
        }
        else if (arg == "--include-slow") {
            opts.includeSlow = true;
        }
        else if (arg == "--list") {
            opts.listOnly = true;
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
            while (i + 1 < argc && !std::string(argv[i + 1]).starts_with("-")) {
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
    std::println("Benchmark Selection Options:");
    std::println(
        "  --filter <pattern>    Run only the benchmarks matching the pattern "
        "(repeatable).");
    std::println(
        "                        '*' and '?' are wildcards; a pattern without "
        "wildcards");
    std::println(
        "                        matches the exact name or a whole group "
        "(e.g. stat/topology)");
    std::println(
        "  --exclude <pattern>   Do not run the benchmarks matching the "
        "pattern (repeatable)");
    std::println(
        "  --include-slow        Run also the benchmarks marked as slow (they "
        "are skipped by");
    std::println(
        "                        default unless selected by --filter)");
    std::println(
        "  --list                List the selected benchmarks and exit");
    std::println(
        "  --config <path>       JSON file with the keys \"include\", "
        "\"exclude\" (arrays of patterns),");
    std::println(
        "                        \"include_slow\", \"iterations\" and the "
        "dataset keys \"mesh\" or \"grid\".");
    std::println(
        "                        The command line options override the "
        "config file");
    std::println("  -h, --help            Display this help message");
}

} // namespace bench

#endif // VCL_BENCHMARKS_CLI_OPTIONS_H
