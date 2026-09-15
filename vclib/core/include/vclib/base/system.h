// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_SYSTEM_H
#define VCL_BASE_SYSTEM_H

#include <cstdlib>
#include <filesystem>
#include <string>

namespace vcl {

/**
 * @brief Returns the default system-specific directory for application
 * configuration and settings.
 *
 * This function resolves the appropriate directory to store application
 * settings based on the current operating system:
 * - **Windows**: `%APPDATA%\<appName>`
 * - **macOS**: `~/Library/Application Support/<appName>`
 * - **Linux/Unix**: `$XDG_CONFIG_HOME/<appName>` or `~/.config/<appName>`
 *
 * @param appName The name of the application, used to create a dedicated
 * sub-directory.
 * @return The absolute path to the configuration directory.
 */
inline std::filesystem::path appConfigDirectory(const std::string& appName)
{
#if defined(_WIN32)
    const char* appData = std::getenv("APPDATA");
    return std::filesystem::path(appData ? appData : "") / appName;
#elif defined(__APPLE__)
    const char* home = std::getenv("HOME");
    return std::filesystem::path(home ? home : "") / "Library" /
           "Application Support" / appName;
#else
    // Linux / Unix
    const char* xdgConfig = std::getenv("XDG_CONFIG_HOME");
    if (xdgConfig && *xdgConfig) {
        return std::filesystem::path(xdgConfig) / appName;
    }
    const char* home = std::getenv("HOME");
    return std::filesystem::path(home ? home : "") / ".config" / appName;
#endif
}

} // namespace vcl

#endif // VCL_BASE_SYSTEM_H
