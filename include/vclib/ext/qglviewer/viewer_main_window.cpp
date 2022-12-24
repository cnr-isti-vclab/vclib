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
	ui->drawVectorFrame->setDrawableObjectVector(drawVector);

	connect(
		ui->renderSettingsFrame, SIGNAL(settingsUpdated()),
		this, SLOT(renderSettingsUpdated()));
	connect(
		ui->drawVectorFrame, SIGNAL(drawableObjectVisibilityChanged()),
		ui->glArea, SLOT(update()));
	connect(ui->drawVectorFrame, SIGNAL(drawableObjectSelectionChanged(uint)),
			this, SLOT(selectedDrawableObjectChanged(uint)));
	ui->rightArea->setVisible(false);
}

ViewerMainWindow::~ViewerMainWindow()
{
	delete ui;
}

void ViewerMainWindow::setDrawableObjectVector(std::shared_ptr<vcl::DrawableObjectVector> v)
{
	drawVector = v;

	// order here is important: drawVectorFrame must have the drawVector before the
	// renderSettingsFrame!
	ui->glArea->setDrawableObjectVector(drawVector);
	ui->drawVectorFrame->setDrawableObjectVector(drawVector);
	if (drawVector->size() > 0) {
		try {
			GenericDrawableMesh& m = dynamic_cast<GenericDrawableMesh&>(drawVector->at(0));
			ui->renderSettingsFrame->setMeshRenderSettings(m.renderSettings());
			ui->rightArea->setVisible(true);
		}
		catch(std::bad_cast exp) {
			ui->rightArea->setVisible(false);
		}
	}
	else {
		ui->rightArea->setVisible(false);
	}
	ui->glArea->fitScene();
}

void ViewerMainWindow::selectedDrawableObjectChanged(uint i)
{
	try {
		GenericDrawableMesh& m = dynamic_cast<GenericDrawableMesh&>(drawVector->at(i));
		ui->renderSettingsFrame->setMeshRenderSettings(m.renderSettings());
		ui->rightArea->setVisible(true);
	}
	catch(std::bad_cast exp) {
		ui->rightArea->setVisible(false);
	}
}

/**
 * @brief Slot called every time that the MeshRenderSettingsFrame emits 'settingsUpdated()',
 * that is when the user changes render settings of a GeneriDrawableMesh.
 *
 * We need to get the selected GeneriDrawableMesh first, and then update the settings to it.
 */
void ViewerMainWindow::renderSettingsUpdated()
{
	uint i = ui->drawVectorFrame->selectedDrawableObject();
	if (drawVector->size() > 0) {
		// todo - get the selected mesh instead of the first
		GenericDrawableMesh& m = dynamic_cast<GenericDrawableMesh&>(drawVector->at(i));
		m.setRenderSettings(ui->renderSettingsFrame->meshRenderSettings());
		ui->glArea->update();
	}
}

} // namespace vcl
