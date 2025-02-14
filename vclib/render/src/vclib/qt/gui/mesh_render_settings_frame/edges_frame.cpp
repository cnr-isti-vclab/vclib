/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/qt/gui/mesh_render_settings_frame/edges_frame.h>

#include "ui_edges_frame.h"

#include <QColorDialog>
#include <QStandardItemModel>

namespace vcl::qt {

EdgesFrame::EdgesFrame(MeshRenderSettings& settings, QWidget* parent) :
        GenericMeshRenderSettingsFrame(settings, parent),
        mUI(new Ui::EdgesFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->visibilityCheckBox,
        SIGNAL(stateChanged(int)),
        this,
        SLOT(onVisibilityChanged(int)));

    connect(
        mUI->shadingSmoothRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onShadingSmoothToggled(bool)));

    connect(
        mUI->shadingFlatRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onShadingFlatToggled(bool)));

    connect(
        mUI->shadingNoneRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onShadingNoneToggled(bool)));

    connect(
        mUI->colorComboBox,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(onColorComboBoxChanged(int)));

    connect(
        mUI->colorDialogPushButton,
        SIGNAL(clicked()),
        this,
        SLOT(onColorDialogButtonClicked()));

    connect(
        mUI->sizeSlider,
        SIGNAL(valueChanged(int)),
        this,
        SLOT(onSizeChanged(int)));
}

EdgesFrame::~EdgesFrame()
{
    delete mUI;
}

void EdgesFrame::updateFrameFromSettings()
{
    if (mMRS.canEdges(VISIBLE)) {
        this->setEnabled(true);
        mUI->visibilityCheckBox->setEnabled(true);
        mUI->visibilityCheckBox->setChecked(mMRS.isEdges(VISIBLE));
        mUI->shadingSmoothRadioButton->setEnabled(
            mMRS.canEdges(SHADING_SMOOTH));
        mUI->shadingSmoothRadioButton->setChecked(mMRS.isEdges(SHADING_SMOOTH));
        mUI->shadingFlatRadioButton->setEnabled(mMRS.canEdges(SHADING_FLAT));
        mUI->shadingFlatRadioButton->setChecked(mMRS.isEdges(SHADING_FLAT));
        mUI->shadingNoneRadioButton->setChecked(mMRS.isEdges(SHADING_NONE));

        updateColorComboBoxFromSettings();
        mUI->sizeSlider->setValue(mMRS.edgesWidth());
    }
    else {
        this->setEnabled(false);
        mUI->visibilityCheckBox->setChecked(false);
    }
}

void EdgesFrame::updateColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->colorComboBox->model());
    assert(model != nullptr);
    QStandardItem* item = model->item(E_VERTEX);
    if (mMRS.canEdges(COLOR_VERTEX)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    item = model->item(E_EDGES);
    if (mMRS.canEdges(COLOR_EDGE)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    item = model->item(E_MESH);
    if (mMRS.canEdges(COLOR_MESH)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    if (mMRS.isEdges(COLOR_VERTEX))
        mUI->colorComboBox->setCurrentIndex(E_VERTEX);
    if (mMRS.isEdges(COLOR_EDGE))
        mUI->colorComboBox->setCurrentIndex(E_EDGES);
    if (mMRS.isEdges(COLOR_MESH))
        mUI->colorComboBox->setCurrentIndex(E_MESH);
    if (mMRS.isEdges(COLOR_USER))
        mUI->colorComboBox->setCurrentIndex(E_USER);

    mUI->userColorFrame->setEnabled(mMRS.isEdges(COLOR_USER));
    vcl::Color vc = mMRS.edgesUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->colorDialogPushButton, c);
}

void EdgesFrame::onVisibilityChanged(int arg1)
{
    mMRS.setEdges(VISIBLE, arg1 == Qt::Checked);
    emit settingsUpdated();
}

void EdgesFrame::onShadingSmoothToggled(bool checked)
{
    if (checked) {
        mMRS.setEdges(SHADING_SMOOTH);
        emit settingsUpdated();
    }
}

void EdgesFrame::onShadingFlatToggled(bool checked)
{
    if (checked) {
        mMRS.setEdges(SHADING_FLAT);
        emit settingsUpdated();
    }
}

void EdgesFrame::onShadingNoneToggled(bool checked)
{
    if (checked) {
        mMRS.setEdges(SHADING_NONE);
        emit settingsUpdated();
    }
}

void EdgesFrame::onColorComboBoxChanged(int index)
{
    switch (index) {
    case E_VERTEX: mMRS.setEdges(COLOR_VERTEX); break;
    case E_EDGES: mMRS.setEdges(COLOR_EDGE); break;
    case E_MESH: mMRS.setEdges(COLOR_MESH); break;
    case E_USER: mMRS.setEdges(COLOR_USER); break;
    }
    mUI->userColorFrame->setEnabled(index == E_USER);
    emit settingsUpdated();
}

void EdgesFrame::onColorDialogButtonClicked()
{
    QColor color =
        QColorDialog::getColor(getButtonBackGround(mUI->colorDialogPushButton));
    if (color.isValid()) {
        setButtonBackGround(mUI->colorDialogPushButton, color);

        mMRS.setEdgesUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void EdgesFrame::onSizeChanged(int value)
{
    mMRS.setEdgesWidth(value);
    emit settingsUpdated();
}

} // namespace vcl::qt
