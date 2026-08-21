// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SETTINGS_DIALOG_VIEWER_SETTINGS_TAB_IMPL_H
#define VCL_QT_GUI_SETTINGS_DIALOG_VIEWER_SETTINGS_TAB_IMPL_H

#include "settings_dialog_tab.h"

#include <vclib/qt/gui/viewer_settings_frame.h>
#include <vclib/qt/mesh_viewer.h>

namespace vcl::qt {

/**
 * @brief The ViewerSettingsTabImpl class implements SettingsDialogTab for the
 * viewer settings.
 */
class ViewerSettingsTabImpl : public SettingsDialogTab
{
    MeshViewer*          mMeshViewer;
    ViewerSettings       mTempSettings;
    ViewerSettingsFrame* mFrame = nullptr;

public:
    ViewerSettingsTabImpl(MeshViewer* meshViewer) : mMeshViewer(meshViewer) {}

    QString category() const override { return "Viewer"; }

    QString name() const override { return "General"; }

    QWidget* createWidget(QWidget* parent) override
    {
        mTempSettings = mMeshViewer->viewerSettings();

        mFrame = new ViewerSettingsFrame(parent);
        mFrame->setViewerSettings(mTempSettings);

        QObject::connect(
            mFrame,
            &ViewerSettingsFrame::settingsChanged,
            [this](const ViewerSettings& sts) {
                mTempSettings = sts;
            });

        return mFrame;
    }

    void applySettings() override
    {
        mMeshViewer->setViewerSettings(mTempSettings);
    }

    void saveSettings(nlohmann::json& j) const override
    {
        mTempSettings.saveSettings(j);
    }

    void updateToolbarFrames(QToolBar* /*toolbar*/) override
    {
        // Settings are already applied via applySettings,
        // which updates the MeshViewer and its DockWidget frame.
        // No specific toolbar frames need to be manually updated here.
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_SETTINGS_DIALOG_VIEWER_SETTINGS_TAB_IMPL_H
