/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "mesh_render_settings_frame.h"
#include "ui_mesh_render_settings_frame.h"

#include <QColorDialog>

namespace vcl {

MeshRenderSettingsFrame::MeshRenderSettingsFrame(QWidget *parent) :
		QFrame(parent),
		ui(new Ui::MeshRenderSettingsFrame)
{
	ui->setupUi(this);
}

MeshRenderSettingsFrame::~MeshRenderSettingsFrame()
{
	delete ui;
}

const MeshRenderSettings& MeshRenderSettingsFrame::meshRenderSettings() const
{
	return mrs;
}

void MeshRenderSettingsFrame::setMeshRenderSettings(const MeshRenderSettings &settings)
{
	mrs = settings;
	updateGuiFromSettings();
}

void MeshRenderSettingsFrame::on_pointColorDialogPushButton_clicked()
{
	QColor color = QColorDialog::getColor();

	if (color.isValid()) {
		QString s("background: #"
				  + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
				  + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
				  + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
		ui->pointColorDialogPushButton->setStyleSheet(s);
		ui->pointColorDialogPushButton->update();

		//mrs.setPointCloudUserDefinedColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	}
}


void MeshRenderSettingsFrame::on_surfaceColorDialogPushButton_clicked()
{
	QColor color = QColorDialog::getColor();

	if (color.isValid()) {
		QString s("background: #"
				  + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
				  + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
				  + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
		ui->surfaceColorDialogPushButton->setStyleSheet(s);
		ui->surfaceColorDialogPushButton->update();

		//mrs.setSurfaceUserDefinedColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	}
}

void MeshRenderSettingsFrame::on_wireframeColorPushButton_clicked()
{
	QColor color = QColorDialog::getColor();

	if (color.isValid()) {
		QString s("background: #"
				  + QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
				  + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
				  + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
		ui->wireframeColorDialogPushButton->setStyleSheet(s);
		ui->wireframeColorDialogPushButton->update();

		mrs.setWireframeColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	}
}

void MeshRenderSettingsFrame::updateGuiFromSettings()
{

}

} // namespace vcl

