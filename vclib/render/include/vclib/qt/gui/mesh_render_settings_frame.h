/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
#define VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H

#include "mesh_render_settings_frame/generic_mesh_render_settings_frame.h"

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

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

    std::vector<GenericMeshRenderSettingsFrame*> frames;

public:
    explicit MeshRenderSettingsFrame(QWidget* parent = nullptr);
    ~MeshRenderSettingsFrame();

    const MeshRenderSettings& meshRenderSettings() const;

    void setMeshRenderSettings(
        const MeshRenderSettings& settings,
        bool                      changeCurrentTab = false);

signals:
    void settingsUpdated();

private:
    enum { POINTS_FRAME = 0, SURFACE_FRAME, WIREFRAME_FRAME, EDGES_FRAME };

    void updateGuiFromSettings(bool changeCurrentTab);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
