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

#include <vclib/qt/gui/drawable_object_frame.h>

#include "ui_drawable_object_frame.h"

#include <vclib/render/interfaces/drawable_mesh_i.h>

namespace vcl::qt {

DrawableObjectFrame::DrawableObjectFrame(
    const std::shared_ptr<DrawableObjectI>& obj,
    QWidget*                                parent) :
        QFrame(parent), mUI(new Ui::DrawableObjectFrame), mObj(obj)
{
    mUI->setupUi(this);
    assert(obj);
    mUI->objNameLabel->setText(QString::fromStdString(obj->name()));
    mUI->visibilityCheckBox->setChecked(obj->isVisible());

    mUI->infoFrame->setVisible(false);
    mUI->showInfoToolButton->setVisible(false);

    // info management
    mUI->infoLabel->setText(QString::fromStdString(obj->info()));
    if (obj->info().empty()) {
        mUI->showInfoToolButton->setVisible(false);
        mUI->showInfoToolButton->setChecked(false);
        mUI->showInfoToolButton->setEnabled(false);
        mUI->infoFrame->setVisible(false);
    }
    else {
        mUI->showInfoToolButton->setVisible(true);
        mUI->showInfoToolButton->setEnabled(true);
        mUI->showInfoToolButton->setChecked(false);
        mUI->infoFrame->setVisible(false);
    }

    // connects
    connect(
        mUI->showInfoToolButton,
        &QToolButton::toggled,
        this,
        &DrawableObjectFrame::showInfoToolButtonChecked);

    connect(
        mUI->visibilityCheckBox,
        &QCheckBox::stateChanged,
        this,
        &DrawableObjectFrame::visibilityCheckBoxStateChanged);
}

DrawableObjectFrame::~DrawableObjectFrame()
{
    delete mUI;
}

void DrawableObjectFrame::visibilityCheckBoxStateChanged(int arg1)
{
    mObj->setVisibility(arg1 == Qt::Checked);
    emit visibilityChanged();
}

void DrawableObjectFrame::showInfoToolButtonChecked(bool checked)
{
    if (checked) {
        mUI->showInfoToolButton->setArrowType(Qt::DownArrow);
    }
    else {
        mUI->showInfoToolButton->setArrowType(Qt::RightArrow);
    }
    mUI->infoFrame->setVisible(checked);
    emit resized();
}

} // namespace vcl::qt
