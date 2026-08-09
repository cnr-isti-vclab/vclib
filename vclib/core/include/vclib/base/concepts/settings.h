// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_CONCEPTS_SETTINGS_H
#define VCL_BASE_CONCEPTS_SETTINGS_H

#ifdef VCLIB_WITH_JSON
#include <nlohmann/json.hpp>
#endif

namespace vcl {

#ifdef VCLIB_WITH_JSON

/**
 * @brief Concept that checks if a type T provides settings serialization methods.
 * 
 * A type T satisfies `HasSettings` if it provides both `loadSettings` and `saveSettings`
 * methods that take `nlohmann::json` objects.
 */
template <typename T>
concept HasSettings = requires(T t, nlohmann::json& j, const nlohmann::json& cj) {
    { t.loadSettings(cj) };
    { t.saveSettings(j) };
};

#endif // VCLIB_WITH_JSON

} // namespace vcl

#endif // VCL_BASE_CONCEPTS_SETTINGS_H
