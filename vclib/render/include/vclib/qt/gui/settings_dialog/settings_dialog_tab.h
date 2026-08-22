// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SETTINGS_DIALOG_SETTINGS_DIALOG_TAB_H
#define VCL_QT_GUI_SETTINGS_DIALOG_SETTINGS_DIALOG_TAB_H

#include <vclib/render/concepts/settings.h>

#include <nlohmann/json.hpp>

#include <QString>
#include <QToolBar>
#include <QWidget>

#include <functional>
#include <vector>

namespace vcl {
class AbstractInputActionMap;
} // namespace vcl

namespace vcl::qt {

/**
 * @brief The SettingsDialogTab class is an interface for a settings tab inside
 * the SettingsDialog.
 */
class SettingsDialogTab
{
public:
    virtual ~SettingsDialogTab() = default;

    /**
     * @brief Returns the category name under which this tab should be grouped
     * (e.g., "Editors", "Viewer").
     * @return The category name as a QString.
     */
    virtual QString category() const = 0;

    /**
     * @brief Returns the display name of this specific tab
     * (e.g., "Bounding Box", "General").
     * @return The tab name as a QString.
     */
    virtual QString name() const = 0;

    /**
     * @brief Creates and returns the UI widget for this settings tab.
     * @param[in] parent: The parent widget for the created UI.
     * @return A pointer to the newly created QWidget.
     */
    virtual QWidget* createWidget(QWidget* parent) = 0;

    /**
     * @brief Applies the current settings modified in the UI to the actual
     * application objects (e.g., editors, viewers).
     */
    virtual void applySettings() = 0;

    /**
     * @brief Saves the current settings to the provided JSON object.
     * @param[out] j: The JSON object where the settings will be stored.
     */
    virtual void saveSettings(nlohmann::json& j) const = 0;

    /**
     * @brief Hook to update any external GUI elements (like toolbars or dock
     * widgets) after settings have been applied.
     * @param[in] toolbar: Pointer to the main QToolBar.
     */
    virtual void updateToolbarFrames(QToolBar* toolbar) = 0;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_SETTINGS_DIALOG_SETTINGS_DIALOG_TAB_H
