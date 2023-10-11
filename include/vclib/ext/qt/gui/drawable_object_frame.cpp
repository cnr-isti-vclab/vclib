/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#include "drawable_object_frame.h"
#include "ui_drawable_object_frame.h"

namespace vcl {

DrawableObjectFrame::DrawableObjectFrame(DrawableObject *obj, QWidget *parent) :
        QFrame(parent),
        ui(new Ui::DrawableObjectFrame),
        obj(obj)
{
    ui->setupUi(this);
    assert(obj);
    ui->objNameLabel->setText(QString::fromStdString(obj->name()));
    ui->visibilityCheckBox->setChecked(obj->isVisible());
}

DrawableObjectFrame::~DrawableObjectFrame()
{
    delete ui;
}


void DrawableObjectFrame::on_visibilityCheckBox_stateChanged(int arg1)
{
    obj->setVisibility(arg1 == Qt::Checked);
    emit visibilityChanged();
}

} // namespace vcl
