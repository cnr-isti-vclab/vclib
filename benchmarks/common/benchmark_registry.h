// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BENCHMARKS_BENCHMARK_REGISTRY_H
#define VCL_BENCHMARKS_BENCHMARK_REGISTRY_H

#include "cli_options.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace bench {

/**
 * @brief Matches a text against a pattern that can contain the wildcards `*`
 * (any sequence of characters, '/' included) and `?` (any single character).
 */
inline bool globMatch(std::string_view pattern, std::string_view text)
{
    std::size_t p = 0, t = 0;
    std::size_t star = std::string_view::npos, mark = 0;

    while (t < text.size()) {
        if (p < pattern.size() && (pattern[p] == '?' || pattern[p] == text[t])) {
            ++p;
            ++t;
        }
        else if (p < pattern.size() && pattern[p] == '*') {
            star = p++;
            mark = t;
        }
        else if (star != std::string_view::npos) {
            p = star + 1;
            t = ++mark;
        }
        else {
            return false;
        }
    }
    while (p < pattern.size() && pattern[p] == '*')
        ++p;
    return p == pattern.size();
}

/**
 * @brief Selects benchmarks by name.
 *
 * A pattern matches a benchmark if:
 * - it contains wildcards and matches the whole name (see globMatch);
 * - otherwise, it is equal to the name or it is a group of the name (i.e. the
 *   name starts with the pattern followed by '/').
 *
 * A benchmark is selected if it matches at least one include pattern (or there
 * are no include patterns) and it does not match any exclude pattern. The
 * benchmarks marked as slow are selected only if includeSlow is true or if
 * there is at least one include pattern.
 */
struct BenchmarkFilter
{
    std::vector<std::string> includes;
    std::vector<std::string> excludes;
    bool                     includeSlow = false;

    static bool patternMatches(const std::string& pattern, const std::string& name)
    {
        if (pattern.find_first_of("*?") != std::string::npos)
            return globMatch(pattern, name);
        std::string_view p = pattern;
        while (p.ends_with('/'))
            p.remove_suffix(1);
        return name == p ||
               (name.size() > p.size() && name.starts_with(p) &&
                name[p.size()] == '/');
    }

    bool selects(const std::string& name, bool slow) const
    {
        auto matches = [&](const std::string& pattern) {
            return patternMatches(pattern, name);
        };

        if (std::ranges::any_of(excludes, matches))
            return false;
        if (!includes.empty() && !std::ranges::any_of(includes, matches))
            return false;
        return !slow || includeSlow || !includes.empty();
    }
};

/**
 * @brief Registry of the benchmarks of an executable.
 *
 * Each benchmark is a function that receives a State (defined by the
 * executable, that contains the dataset and the runner) and runs one or more
 * measurements. The benchmarks register themselves at static initialization
 * time (see the Registrar class), allowing to organize them in several
 * translation units.
 */
template<typename State>
class BenchmarkRegistry
{
public:
    using Func = std::function<void(State&)>;

    struct Entry
    {
        std::string name;
        Func        func;
        bool        slow = false;
    };

    static BenchmarkRegistry& instance()
    {
        static BenchmarkRegistry registry;
        return registry;
    }

    void add(std::string name, Func func, bool slow = false)
    {
        for (const Entry& e : mEntries) {
            if (e.name == name) {
                std::fprintf(
                    stderr,
                    "Error: benchmark '%s' registered twice\n",
                    name.c_str());
                std::abort();
            }
        }
        mEntries.push_back({std::move(name), std::move(func), slow});
    }

    /**
     * @brief Returns the benchmarks selected by the filter, sorted by name.
     */
    std::vector<const Entry*> select(const BenchmarkFilter& filter) const
    {
        std::vector<const Entry*> res;
        for (const Entry& e : mEntries) {
            if (filter.selects(e.name, e.slow))
                res.push_back(&e);
        }
        std::ranges::sort(res, [](const Entry* a, const Entry* b) {
            return a->name < b->name;
        });
        return res;
    }

private:
    std::vector<Entry> mEntries;
};

/**
 * @brief Loads the optional JSON configuration file given with --config and
 * applies it to the options. The values given in the command line have the
 * precedence over the ones of the file.
 *
 * Supported keys: "include" and "exclude" (arrays of patterns),
 * "include_slow" (bool) and "iterations" (int). The other keys (e.g. the ones
 * that describe the dataset) are returned to the caller.
 *
 * @return The content of the configuration file (an empty object if the file
 * has not been given).
 */
inline nlohmann::json loadConfig(CommonOptions& opts)
{
    nlohmann::json config = nlohmann::json::object();
    if (opts.configPath.empty())
        return config;

    std::ifstream file(opts.configPath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open config file " + opts.configPath);
    config = nlohmann::json::parse(file);

    if (opts.includes.empty() && config.contains("include"))
        opts.includes = config["include"].get<std::vector<std::string>>();
    if (opts.excludes.empty() && config.contains("exclude"))
        opts.excludes = config["exclude"].get<std::vector<std::string>>();
    if (!opts.includeSlow && config.contains("include_slow"))
        opts.includeSlow = config["include_slow"].get<bool>();
    if (!opts.iterationsSet && config.contains("iterations"))
        opts.iterations = config["iterations"].get<int>();

    return config;
}

inline BenchmarkFilter makeFilter(const CommonOptions& opts)
{
    return BenchmarkFilter {opts.includes, opts.excludes, opts.includeSlow};
}

} // namespace bench

#endif // VCL_BENCHMARKS_BENCHMARK_REGISTRY_H
