// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_SURFACE_FRAME_H
#define VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_SURFACE_FRAME_H

#include "generic_mesh_render_settings_frame.h"

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class SurfaceFrame;
} // namespace Ui

class SurfaceFrame : public GenericMeshRenderSettingsFrame
{
    Q_OBJECT

    Ui::SurfaceFrame* mUI;

public:
    explicit SurfaceFrame(
        MeshRenderSettings& settings,
        QWidget*            parent = nullptr);
    ~SurfaceFrame();

    void updateFrameFromSettings() override;

private:
    enum SURF_COLOR {
        SC_VERT = 0,
        SC_FACE,
        SC_MESH,
        SC_VERT_TEX,
        SC_WEDG_TEX,
        SC_USER
    };

    void uptateShadingRadioButtonsFromSettings();
    void updateColorComboBoxFromSettings();

private slots:
    void onVisibilityChanged(Qt::CheckState arg1);
    void onShadingSmoothToggled(bool checked);
    void onShadingFlatToggled(bool checked);
    void onShadingNoneToggled(bool checked);
    void onColorComboBoxChanged(int index);
    void onUserColorChanged(const QColor& c);
    void onSelectionVisibilityChanged(Qt::CheckState arg1);
    void onSelectionColorChanged(const QColor& c);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_SURFACE_FRAME_H
