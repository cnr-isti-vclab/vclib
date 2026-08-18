// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SETTINGS_DIALOG_TAB_H
#define VCL_QT_GUI_SETTINGS_DIALOG_TAB_H

#include <vclib/render/concepts/settings.h>

#include <nlohmann/json.hpp>

#include <QString>
#include <QToolBar>
#include <QWidget>

namespace vcl::qt {

/**
 * @brief The SettingsDialogTab class is an interface for a settings tab inside
 * the SettingsDialog.
 */
class SettingsDialogTab
{
public:
    virtual ~SettingsDialogTab()                            = default;
    virtual QString  category() const                       = 0;
    virtual QString  name() const                           = 0;
    virtual QWidget* createWidget(QWidget* parent)          = 0;
    virtual void     applySettings()                        = 0;
    virtual void     saveSettings(nlohmann::json& j) const  = 0;
    virtual void     updateToolbarFrames(QToolBar* toolbar) = 0;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_SETTINGS_DIALOG_TAB_H
