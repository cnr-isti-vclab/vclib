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

#ifndef VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
#define VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H

#include <QFrame>

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include "mesh_render_settings_frame/generic_mesh_render_settings_frame.h"

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
    void setMeshRenderSettings(const MeshRenderSettings& settings);

signals:
    void settingsUpdated();

private slots:
    void on_surfaceVisibilityCheckBox_stateChanged(int arg1);
    void on_surfaceShadingNoneRadioButton_toggled(bool checked);
    void on_surfaceShadingSmoothRadioButton_toggled(bool checked);
    void on_surfaceShadingFlatRadioButton_toggled(bool checked);
    void on_surfaceColorComboBox_currentIndexChanged(int index);
    void on_surfaceColorDialogPushButton_clicked();

    void on_wireframeVisibilityCheckBox_stateChanged(int arg1);
    void on_wireframeShadingNoneRadioButton_toggled(bool checked);
    void on_wireframeShadingVertexRadioButton_toggled(bool checked);
    void on_wireframeColorComboBox_currentIndexChanged(int index);
    void on_wireframeColorDialogPushButton_clicked();
    void on_wireframeSizeSlider_valueChanged(int value);

    void on_edgesVisibilityCheckBox_stateChanged(int arg1);
    void on_edgesShadingSmoothRadioButton_toggled(bool checked);
    void on_edgesShadingFlatRadioButton_toggled(bool checked);
    void on_edgesShadingNoneRadioButton_toggled(bool checked);
    void on_edgesColorComboBox_currentIndexChanged(int index);
    void on_edgesColorDialogPushButton_clicked();
    void on_edgesSizeSlider_valueChanged(int value);

private:
    enum SURF_COLOR {
        SC_VERT = 0,
        SC_FACE,
        SC_MESH,
        SC_VERT_TEX,
        SC_WEDG_TEX,
        SC_USER
    };

    enum WIRE_COLOR { W_VERTEX = 0, W_MESH, W_USER };

    enum EDGES_COLOR { E_VERTEX = 0, E_EDGES, E_MESH, E_USER };

    void updateGuiFromSettings();
    void updateSurfaceTabFromSettings();
    void uptateSurfaceShadingRadioButtonsFromSettings();
    void updateSurfaceColorComboBoxFromSettings();
    void updateWireframeTabFromSettings();
    void updateWireframeComboBoxFromSettings();
    void updateEdgesTabFromSettings();
    void updateEdgesComboBoxFromSettings();

    void   setButtonBackGround(QPushButton* b, const QColor& c);
    QColor getButtonBackGround(QPushButton* b);
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
