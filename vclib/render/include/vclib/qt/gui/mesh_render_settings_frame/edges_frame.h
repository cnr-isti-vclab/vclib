// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_EDGES_FRAME_H
#define VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_EDGES_FRAME_H

#include "generic_mesh_render_settings_frame.h"

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class EdgesFrame;
} // namespace Ui

class EdgesFrame : public GenericMeshRenderSettingsFrame
{
    Q_OBJECT

    Ui::EdgesFrame* mUI;

public:
    explicit EdgesFrame(
        MeshRenderSettings& settings,
        QWidget*            parent = nullptr);
    ~EdgesFrame();

    void updateFrameFromSettings() override;

private:
    enum EDGES_COLOR { E_VERTEX = 0, E_EDGES, E_MESH, E_USER };

    void updateColorComboBoxFromSettings();

private slots:
    void onVisibilityChanged(Qt::CheckState arg1);
    void onShadingSmoothToggled(bool checked);
    void onShadingFlatToggled(bool checked);
    void onShadingNoneToggled(bool checked);
    void onColorComboBoxChanged(int index);
    void onUserColorChanged(const QColor& c);
    void onSizeChanged(int value);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_EDGES_FRAME_H
