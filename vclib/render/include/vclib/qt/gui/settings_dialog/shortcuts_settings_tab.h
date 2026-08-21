// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_SHORTCUTS_SETTINGS_TAB_H
#define VCL_QT_SHORTCUTS_SETTINGS_TAB_H

#include <vclib/qt/gui/settings_dialog/settings_dialog_tab.h>
#include <vclib/render/input/action_map_group.h>

#include <QIcon>
#include <QString>
#include <QWidget>

#include <functional>

namespace vcl::qt {

class InputBindingsWidget;

/**
 * @brief The SettingsDialogTab that lets the user browse and customize all
 * the input bindings (shortcuts) exposed by the viewer and its active
 * editors.
 *
 * The available ActionMapGroup%s are obtained on demand from mProvider (e.g.
 * a callback into the viewer), so the tab always reflects the editors that
 * are currently pushed into the viewer.
 */
class ShortcutsSettingsTab : public SettingsDialogTab
{
    std::function<std::vector<ActionMapGroup>()>              mProvider;
    // one InputBindingsWidget per action map, paired with its owning group
    // name, used by checkConflicts() to scope conflict detection
    std::vector<std::pair<InputBindingsWidget*, std::string>> mWidgets;

public:
    explicit ShortcutsSettingsTab(
        std::function<std::vector<ActionMapGroup>()> provider) :
            mProvider(std::move(provider))
    {
    }

    ~ShortcutsSettingsTab() override = default;

    QString category() const override;

    QString name() const override;

    QWidget* createWidget(QWidget* parent) override;

    void applySettings() override;

    void saveSettings(nlohmann::json& j) const override;

    void updateToolbarFrames(QToolBar* /*toolbar*/) override {}

private:
    void checkConflicts();
};

} // namespace vcl::qt

#endif // VCL_QT_SHORTCUTS_SETTINGS_TAB_H
