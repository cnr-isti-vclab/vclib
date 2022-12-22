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

namespace vcl {

ViewerMainWindow::ViewerMainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::ViewerMainWindow)
{
	ui->setupUi(this);

	drawVector = std::make_shared<vcl::DrawableObjectVector>();
	ui->glArea->setDrawableObjectVector(drawVector);
}

ViewerMainWindow::~ViewerMainWindow()
{
	delete ui;
}

void ViewerMainWindow::setDrawableObjectVector(std::shared_ptr<vcl::DrawableObjectVector> v)
{
	drawVector = v;
	ui->glArea->setDrawableObjectVector(v);
	ui->glArea->fitScene();
}

} // namespace vcl
