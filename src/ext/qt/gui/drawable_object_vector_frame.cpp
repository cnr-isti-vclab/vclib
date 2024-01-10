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

#include <vclib/ext/qt/gui/drawable_object_vector_frame.h>
#include "ui_drawable_object_vector_frame.h"

#include <QStandardItemModel>

namespace vcl::qt {

DrawableObjectVectorFrame::DrawableObjectVectorFrame(QWidget* parent) :
        QFrame(parent), ui(new Ui::DrawableObjectVectorFrame)
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

void DrawableObjectVectorFrame::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    drawList = v;
    updateDrawableVectorWidget();
}

uint DrawableObjectVectorFrame::selectedDrawableObject() const
{
    auto item = ui->listWidget->selectedItems().first();
    return ui->listWidget->row(item);
}

void DrawableObjectVectorFrame::on_listWidget_itemSelectionChanged()
{
    if (ui->listWidget->selectedItems().size() > 0) {
        emit drawableObjectSelectionChanged(selectedDrawableObject());
    }
    else {
        ui->listWidget->item(0)->setSelected(true);
    }
}

void DrawableObjectVectorFrame::updateDrawableVectorWidget()
{
    ui->listWidget->clear();
    for (auto* d : *drawList) {
        QListWidgetItem*     item  = new QListWidgetItem(ui->listWidget);
        DrawableObjectFrame* frame = new DrawableObjectFrame(d, ui->listWidget);

        item->setSizeHint(frame->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, frame);
        connect(
            frame,
            SIGNAL(visibilityChanged()),
            this,
            SIGNAL(drawableObjectVisibilityChanged()));
    }
    if (drawList->size() > 0) {
        ui->listWidget->item(0)->setSelected(true);
    }
}

} // namespace vcl::qt
