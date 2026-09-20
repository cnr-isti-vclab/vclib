// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_DIALOG_DIRECTORIES_H
#define VCL_QT_GUI_DIALOG_DIRECTORIES_H

#include <QString>
#include <string>

namespace vcl::qt {

/**
 * @brief Gets the last used directory for the specified dialog ID.
 *
 * If no directory was previously saved, it returns the user's home directory.
 *
 * @param[in] dialogId: Identifier for the dialog (e.g., "LoadPanorama",
 * "SaveScreenshot").
 * @param[in] settingsFilePath: Path to the settings JSON file. If empty, the
 * default vclib configuration path is used.
 * @return QString The directory path.
 */
QString dialogDirectory(
    const std::string& dialogId,
    const std::string& settingsFilePath = "");

/**
 * @brief Saves the used directory for the specified dialog ID.
 *
 * @param[in] dialogId: Identifier for the dialog.
 * @param[in] dir: The directory path to save.
 * @param[in] settingsFilePath: Path to the settings JSON file. If empty, the
 * default vclib configuration path is used.
 */
void setDialogDirectory(
    const std::string& dialogId,
    const QString&     dir,
    const std::string& settingsFilePath = "");

} // namespace vcl::qt

#endif // VCL_QT_GUI_DIALOG_DIRECTORIES_H
