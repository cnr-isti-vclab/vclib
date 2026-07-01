// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
#define VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H

#include "mesh_render_settings_frame/cross_section_settings_frame.h"
#include "mesh_render_settings_frame/generic_mesh_render_settings_frame.h"

#include <vclib/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/render/settings/cross_section_settings.h>

#include <QFrame>

class QPushButton;

namespace vcl::qt {

namespace Ui {
class MeshRenderSettingsFrame;
} // namespace Ui

class MeshRenderSettingsFrame : public QFrame
{
    Q_OBJECT

    Ui::MeshRenderSettingsFrame* mUI;
    MeshRenderSettings           mMRS;

    std::vector<GenericMeshRenderSettingsFrame*> mFrames;
    CrossSectionSettingsFrame*                   mCrossSectionFrame = nullptr;

public:
    explicit MeshRenderSettingsFrame(QWidget* parent = nullptr);
    ~MeshRenderSettingsFrame();

    const MeshRenderSettings& meshRenderSettings() const;

    const CrossSectionSettings& crossSectionSettings() const;

    void setMeshRenderSettings(
        const MeshRenderSettings& settings,
        bool                      changeCurrentTab = false);

    void setCrossSectionSettings(const CrossSectionSettings& settings);

signals:
    void meshRenderSettingsUpdated();

    void crossSectionSettingsUpdated();

private:
    enum { POINTS_FRAME = 0, SURFACE_FRAME, WIREFRAME_FRAME, EDGES_FRAME };

    void updateGuiFromSettings(bool changeCurrentTab);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
