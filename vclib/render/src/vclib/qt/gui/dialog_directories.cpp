// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/dialog_directories.h>

#include <vclib/base.h>
#include <vclib/render/settings.h>

#include <nlohmann/json.hpp>

#include <QDir>
#include <filesystem>
#include <fstream>

namespace vcl::qt {

static std::string resolveSettingsPath(const std::string& settingsFilePath)
{
    if (!settingsFilePath.empty()) {
        return settingsFilePath;
    }
    std::filesystem::path configDir = vcl::appConfigDirectory("vclib");
    return (configDir / vcl::RENDER_SETTINGS_FILE_NAME).string();
}

QString dialogDirectory(
    const std::string& dialogId,
    const std::string& settingsFilePath)
{
    std::string filePath = resolveSettingsPath(settingsFilePath);

    std::ifstream in(filePath);
    if (in.is_open()) {
        nlohmann::json j;
        try {
            in >> j;
            if (j.contains("DialogDirectories") &&
                j["DialogDirectories"].contains(dialogId)) {
                std::string dir =
                    j["DialogDirectories"][dialogId].get<std::string>();
                if (!dir.empty()) {
                    return QString::fromStdString(dir);
                }
            }
        }
        catch (...) {
            // Ignore parse errors
        }
    }

    // Default to the user's home directory
    return QDir::homePath();
}

void setDialogDirectory(
    const std::string& dialogId,
    const QString&     dir,
    const std::string& settingsFilePath)
{
    if (dir.isEmpty())
        return;

    std::string filePath = resolveSettingsPath(settingsFilePath);

    nlohmann::json j;
    std::ifstream  in(filePath);
    if (in.is_open()) {
        try {
            in >> j;
        }
        catch (...) {
            j = nlohmann::json::object();
        }
        in.close();
    }

    j["DialogDirectories"][dialogId] = dir.toStdString();

    // Ensure directory exists
    QDir qdir;
    qdir.mkpath(
        QString::fromStdString(
            std::filesystem::path(filePath).parent_path().string()));

    std::ofstream out(filePath);
    if (out.is_open()) {
        out << j.dump(4);
    }
}

} // namespace vcl::qt
