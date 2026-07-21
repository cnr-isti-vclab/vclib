// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
