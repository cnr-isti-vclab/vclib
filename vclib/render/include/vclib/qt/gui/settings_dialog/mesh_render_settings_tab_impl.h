// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SETTINGS_DIALOG_MESH_RENDER_SETTINGS_TAB_IMPL_H
#define VCL_QT_GUI_SETTINGS_DIALOG_MESH_RENDER_SETTINGS_TAB_IMPL_H

#include "settings_dialog_data.h"
#include "settings_dialog_tab.h"

#include <vclib/qt/gui/mesh_render_settings_frame/edges_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/generic_mesh_render_settings_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/points_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/surface_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/wireframe_frame.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <QCheckBox>
#include <QPushButton>
#include <QString>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include <functional>
#include <memory>

namespace vcl::qt {

/**
 * @brief The MeshRenderSettingsTabImpl class implements SettingsDialogTab for
 * global Mesh Render Settings defaults.
 */
class MeshRenderSettingsTabImpl : public SettingsDialogTab
{
    MeshRenderSettings&                 mGlobalSettings;
    std::shared_ptr<MeshRenderSettings> mTempSettings;
    QString                             mName;
    std::function<
        GenericMeshRenderSettingsFrame*(MeshRenderSettings&, QWidget*)>
                                             mFactory;
    std::function<void(MeshRenderSettings&)> mResetFunc;

public:
    MeshRenderSettingsTabImpl(
        MeshRenderSettings&                 globalSettings,
        std::shared_ptr<MeshRenderSettings> tempSettings,
        const QString&                      name,
        std::function<
            GenericMeshRenderSettingsFrame*(MeshRenderSettings&, QWidget*)>
                                                 factory,
        std::function<void(MeshRenderSettings&)> resetFunc) :
            mGlobalSettings(globalSettings), mTempSettings(tempSettings),
            mName(name), mFactory(factory), mResetFunc(resetFunc)
    {
    }

    QString category() const override { return "Mesh Render Defaults"; }

    QString name() const override { return mName; }

    QWidget* createWidget(QWidget* parent) override
    {
        QWidget*     page   = new QWidget(parent);
        QVBoxLayout* layout = new QVBoxLayout(page);
        layout->setContentsMargins(0, 0, 0, 0);

        GenericMeshRenderSettingsFrame* frame = mFactory(*mTempSettings, page);
        frame->updateFrameFromSettings();
        if (auto cb = frame->visibilityCheckBox()) {
            cb->setText(QString("Enable %1").arg(mName));
        }

        QPushButton* resetBtn = new QPushButton("Reset to Defaults", page);
        resetBtn->setObjectName("resetDefaultButton");
        QObject::connect(resetBtn, &QPushButton::clicked, [this, frame]() {
            mResetFunc(*mTempSettings);
            frame->updateFrameFromSettings();
        });

        layout->addWidget(frame);
        layout->addWidget(resetBtn);
        // Note: Stretch is automatically added by SettingsDialog.cpp, but
        // adding one here doesn't hurt.

        return page;
    }

    void applySettings() override { mGlobalSettings = *mTempSettings; }

    void saveSettings(nlohmann::json& j) const override
    {
        if (mTempSettings) {
            j["MeshRenderSettings"] = *mTempSettings;
        }
    }

    void updateToolbarFrames(QToolBar*) override {}
};

inline void setupMeshRenderSettingsTabs(
    SettingsDialogData& data,
    MeshRenderSettings& defaults)
{
    auto tempMeshSettings = std::make_shared<MeshRenderSettings>(defaults);

    data.addTab(
        std::make_shared<MeshRenderSettingsTabImpl>(
            defaults,
            tempMeshSettings,
            "Points",
            [](MeshRenderSettings& s, QWidget* p) {
                return new PointsFrame(s, p);
            },
            [](MeshRenderSettings& s) {
                s.setDefaultPointSettingsFromCapability();
            }));

    data.addTab(
        std::make_shared<MeshRenderSettingsTabImpl>(
            defaults,
            tempMeshSettings,
            "Surface",
            [](MeshRenderSettings& s, QWidget* p) {
                return new SurfaceFrame(s, p);
            },
            [](MeshRenderSettings& s) {
                s.setDefaultSurfaceSettingsFromCapability();
            }));

    data.addTab(
        std::make_shared<MeshRenderSettingsTabImpl>(
            defaults,
            tempMeshSettings,
            "Wireframe",
            [](MeshRenderSettings& s, QWidget* p) {
                return new WireframeFrame(s, p);
            },
            [](MeshRenderSettings& s) {
                s.setDefaultWireframeSettingsFromCapability();
            }));

    data.addTab(
        std::make_shared<MeshRenderSettingsTabImpl>(
            defaults,
            tempMeshSettings,
            "Edges",
            [](MeshRenderSettings& s, QWidget* p) {
                return new EdgesFrame(s, p);
            },
            [](MeshRenderSettings& s) {
                s.setDefaultEdgeSettingsFromCapability();
            }));
}

} // namespace vcl::qt

#endif // VCL_QT_GUI_SETTINGS_DIALOG_MESH_RENDER_SETTINGS_TAB_IMPL_H
