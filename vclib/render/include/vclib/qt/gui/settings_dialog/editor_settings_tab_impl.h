// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SETTINGS_DIALOG_EDITOR_SETTINGS_TAB_IMPL_H
#define VCL_QT_GUI_SETTINGS_DIALOG_EDITOR_SETTINGS_TAB_IMPL_H

#include "settings_dialog_tab.h"

#include <vclib/render/concepts/settings.h>

#include <QString>
#include <QToolBar>
#include <QWidget>

#include <memory>

namespace vcl::qt {

/**
 * @brief The EditorSettingsTabImpl class implements SettingsDialogTab for
 * specific editor types.
 *
 * @tparam EditorType: The type of the editor.
 * @tparam SettingsFrameType: The type of the GUI frame used to edit the
 * settings.
 */
template<typename EditorType, typename SettingsFrameType>
class EditorSettingsTabImpl : public SettingsDialogTab
{
    std::shared_ptr<EditorType> mEditor;
    QString                     mName;
    using SettingsType = std::remove_reference_t<
        decltype(std::declval<EditorType>().settings())>;
    std::unique_ptr<SettingsType> mTempSettings;

public:
    EditorSettingsTabImpl(
        std::shared_ptr<EditorType> editor,
        const QString&              name) : mEditor(editor), mName(name)
    {
    }

    QString category() const override { return "Editors"; }

    QString name() const override { return mName; }

    QWidget* createWidget(QWidget* parent) override
    {
        mTempSettings = std::make_unique<SettingsType>(mEditor->settings());
        return new SettingsFrameType(*mTempSettings, parent);
    }

    void applySettings() override
    {
        if (mTempSettings) {
            mEditor->settings() = *mTempSettings;
            mEditor->refreshSettings();
        }
    }

    void saveSettings(nlohmann::json& j) const override
    {
        if (mTempSettings) {
            if constexpr (vcl::HasSettings<SettingsType>) {
                mTempSettings->saveSettings(j);
            }
        }
    }

    void updateToolbarFrames(QToolBar* toolbar) override
    {
        for (auto* f : toolbar->findChildren<SettingsFrameType*>()) {
            f->updateGUI();
        }
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_SETTINGS_DIALOG_EDITOR_SETTINGS_TAB_IMPL_H
