// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_SHORTCUTS_SETTINGS_TAB_H
#define VCL_QT_SHORTCUTS_SETTINGS_TAB_H

#include <vclib/qt/gui/settings_dialog/settings_dialog_tab.h>

#include <QIcon>
#include <QString>
#include <QWidget>

namespace vcl::qt {

class ShortcutsSettingsTab : public SettingsDialogTab
{
public:
    ShortcutsSettingsTab()           = default;
    ~ShortcutsSettingsTab() override = default;

    QString category() const override;
    QString name() const override;

    QWidget* createWidget(QWidget* parent) override;

    void applySettings() override {}

    void saveSettings(nlohmann::json& /*j*/) const override {}

    void updateToolbarFrames(QToolBar* /*toolbar*/) override {}
};

} // namespace vcl::qt

#endif // VCL_QT_SHORTCUTS_SETTINGS_TAB_H
