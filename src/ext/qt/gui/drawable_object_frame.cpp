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

#include "ui_drawable_object_frame.h"
#include <vclib/ext/qt/gui/drawable_object_frame.h>

namespace vcl::qt {

DrawableObjectFrame::DrawableObjectFrame(
    DrawableObjectI* obj,
    QWidget*         parent) :
        QFrame(parent),
        ui(new Ui::DrawableObjectFrame), obj(obj)
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

} // namespace vcl::qt
