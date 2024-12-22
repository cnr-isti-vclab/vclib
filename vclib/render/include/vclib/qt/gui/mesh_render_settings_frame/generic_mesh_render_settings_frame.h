/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_GENERIC_MESH_RENDER_SETTINGS_FRAME_H
#define VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_GENERIC_MESH_RENDER_SETTINGS_FRAME_H

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <QFrame>
#include <QPushButton>

namespace vcl::qt {

class GenericMeshRenderSettingsFrame : public QFrame
{
    Q_OBJECT

protected:
    vcl::MeshRenderSettings& mMRS;

public:
    explicit GenericMeshRenderSettingsFrame(
        vcl::MeshRenderSettings& settings,
        QWidget*                 parent = nullptr);

    ~GenericMeshRenderSettingsFrame();

    virtual void updateFrameFromSettings() = 0;

signals:
    void settingsUpdated();

protected:
    static void   setButtonBackGround(QPushButton* b, const QColor& c);
    static QColor getButtonBackGround(const QPushButton* b);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_GENERIC_MESH_RENDER_SETTINGS_FRAME_H
