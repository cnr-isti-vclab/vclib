// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_POINTS_FRAME_H
#define VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_POINTS_FRAME_H

#include "generic_mesh_render_settings_frame.h"

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class PointsFrame;
} // namespace Ui

class PointsFrame : public GenericMeshRenderSettingsFrame
{
    Q_OBJECT

    Ui::PointsFrame* mUI;

public:
    explicit PointsFrame(
        MeshRenderSettings& settings,
        QWidget*            parent = nullptr);
    ~PointsFrame();

    void updateFrameFromSettings() override;

private:
    enum POINT_COLOR { P_VERT = 0, P_MESH, P_USER };

    void updateColorComboBoxFromSettings();

private slots:
    void onVisibilityChanged(Qt::CheckState arg1);
    void onShapeCircleToggled(bool checked);
    void onShapePixelToggled(bool checked);
    void onShadingVertexToggled(bool checked);
    void onShadingNoneToggled(bool checked);
    void onColorComboBoxChanged(int index);
    void onUserColorChanged(const QColor& c);
    void onSizeChanged(int value);
    void onSelectionVisibilityChanged(Qt::CheckState arg1);
    void onSelectionColorChanged(const QColor& c);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_POINTS_FRAME_H
