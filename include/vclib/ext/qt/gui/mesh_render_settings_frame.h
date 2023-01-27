/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
#define VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H

#include <QFrame>

#include <vclib/render/mesh_render_settings.h>

class QPushButton;

namespace vcl {

namespace Ui {
class MeshRenderSettingsFrame;
}

class MeshRenderSettingsFrame : public QFrame
{
	Q_OBJECT

public:
	explicit MeshRenderSettingsFrame(QWidget* parent = nullptr);
	~MeshRenderSettingsFrame();

	const MeshRenderSettings& meshRenderSettings() const;
	void setMeshRenderSettings(const MeshRenderSettings& settings);

signals:
	void settingsUpdated();

private slots:
	void on_pointVisibilityCheckBox_stateChanged(int arg1);
	void on_pointColorComboBox_currentIndexChanged(int index);
	void on_pointColorDialogPushButton_clicked();
	void on_pointSizeSlider_valueChanged(int value);

	void on_surfaceVisibilityCheckBox_stateChanged(int arg1);
	void on_surfaceShadingSmoothRadioButton_toggled(bool checked);
	void on_surfaceShadingFlatRadioButton_toggled(bool checked);
	void on_surfaceColorComboBox_currentIndexChanged(int index);
	void on_surfaceColorDialogPushButton_clicked();

	void on_wireframeVisibilityCheckBox_stateChanged(int arg1);
	void on_wireframeColorComboBox_currentIndexChanged(int index);
	void on_wireframeColorDialogPushButton_clicked();
	void on_wireframeSizelSlider_valueChanged(int value);

private:
	enum POINT_COLOR {P_VERT = 0, P_MESH, P_USER};
	enum SURF_COLOR {SC_VERT = 0, SC_FACE, SC_MESH, SC_VERT_TEX, SC_WEDG_TEX, SC_USER};
	enum WIRE_COLOR {W_MESH = 0, W_USER};

	Ui::MeshRenderSettingsFrame *ui;
	MeshRenderSettings mrs;

	void updateGuiFromSettings();
	void updatePointsTabFromSettings();
	void updatePointsColorComboBoxFromSettings();
	void updateSurfaceTabFromSettings();
	void uptateSurfaceShadingRadioButtonsFromSettings();
	void updateSurfaceColorComboBoxFromSettings();
	void updateWireframeTabFromSettings();
	void updateWireframeComboBoxFromSettings();

	void setButtonBackGround(QPushButton* b, const QColor &c);
	QColor getButtonBackGround(QPushButton* b);
};

} // namespace vcl

#endif // VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
