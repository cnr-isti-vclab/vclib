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

#include "drawable_object_vector_frame.h"
#include "ui_drawable_object_vector_frame.h"

#include <QStandardItemModel>

namespace vcl {

DrawableObjectVectorFrame::DrawableObjectVectorFrame(QWidget* parent) :
		QFrame(parent),
		ui(new Ui::DrawableObjectVectorFrame)
{
	ui->setupUi(this);
}

DrawableObjectVectorFrame::DrawableObjectVectorFrame(
	std::shared_ptr<DrawableObjectVector> v,
	QWidget*                              parent) :
		DrawableObjectVectorFrame(parent)
{
	drawList = v;
	updateDrawableVectorWidget();

}

DrawableObjectVectorFrame::~DrawableObjectVectorFrame()
{
	delete ui;
}

void DrawableObjectVectorFrame::setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v)
{
	drawList = v;
	updateDrawableVectorWidget();
}

void DrawableObjectVectorFrame::updateDrawableVectorWidget()
{

}

} // namespace vcl
