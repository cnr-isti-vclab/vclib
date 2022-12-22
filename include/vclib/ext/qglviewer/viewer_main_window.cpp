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

#include "viewer_main_window.h"
#include "ui_viewer_main_window.h"

#include <vclib/render/generic_drawable_mesh.h>

namespace vcl {

ViewerMainWindow::ViewerMainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::ViewerMainWindow)
{
	ui->setupUi(this);

	drawVector = std::make_shared<vcl::DrawableObjectVector>();
	ui->glArea->setDrawableObjectVector(drawVector);

	connect(ui->renderSettingsFrame, SIGNAL(settingsUpdated()), this, SLOT(renderSettingsUpdated()));
	ui->renderSettingsFrame->setVisible(false);
}

ViewerMainWindow::~ViewerMainWindow()
{
	delete ui;
}

void ViewerMainWindow::setDrawableObjectVector(std::shared_ptr<vcl::DrawableObjectVector> v)
{
	drawVector = v;
	if (drawVector->size() > 0) {
		try {
			GenericDrawableMesh& m = dynamic_cast<GenericDrawableMesh&>(drawVector->at(0));
			ui->renderSettingsFrame->setMeshRenderSettings(m.renderSettings());
			ui->renderSettingsFrame->setVisible(true);
		}
		catch(std::bad_cast exp) {
			ui->renderSettingsFrame->setVisible(false);
		}
	}
	else {
		ui->renderSettingsFrame->setVisible(false);
	}
	ui->glArea->setDrawableObjectVector(v);
	ui->glArea->fitScene();
}

/**
 * @brief Slot called every time that the MeshRenderSettingsFrame emits 'settingsUpdated()'
 */
void ViewerMainWindow::renderSettingsUpdated()
{
	if (drawVector->size() > 0) {
		// todo - get the selected mesh instead of the first
		GenericDrawableMesh& m = dynamic_cast<GenericDrawableMesh&>(drawVector->at(0));
		m.setRenderSettings(ui->renderSettingsFrame->meshRenderSettings());
		ui->glArea->update();
	}
}

} // namespace vcl
