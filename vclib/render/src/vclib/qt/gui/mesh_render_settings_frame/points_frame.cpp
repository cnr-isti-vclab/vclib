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

#include <vclib/qt/gui/mesh_render_settings_frame/points_frame.h>

#include "ui_points_frame.h"

#include <QColorDialog>
#include <QStandardItemModel>

namespace vcl::qt {

using enum MeshRenderInfo::Points;

PointsFrame::PointsFrame(MeshRenderSettings& settings, QWidget* parent) :
        GenericMeshRenderSettingsFrame(settings, parent),
        mUI(new Ui::PointsFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->visibilityCheckBox,
        SIGNAL(checkStateChanged(Qt::CheckState)),
        this,
        SLOT(onVisibilityChanged(Qt::CheckState)));

    connect(
        mUI->shapeCircleRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onShapeCircleToggled(bool)));

    connect(
        mUI->shapePixelRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onShapePixelToggled(bool)));

    connect(
        mUI->shadingVertexRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onShadingVertexToggled(bool)));

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

PointsFrame::~PointsFrame()
{
    delete mUI;
}

void PointsFrame::updateFrameFromSettings()
{
    if (mMRS.canPoints(VISIBLE)) {
        this->setEnabled(true);
        mUI->visibilityCheckBox->setEnabled(true);
        mUI->visibilityCheckBox->setChecked(mMRS.isPoints(VISIBLE));

        mUI->shadingVertexRadioButton->setEnabled(mMRS.canPoints(SHADING_VERT));
        mUI->shadingVertexRadioButton->setChecked(mMRS.isPoints(SHADING_VERT));
        mUI->shadingNoneRadioButton->setChecked(mMRS.isPoints(SHADING_NONE));

        // todo
        mUI->shapePixelRadioButton->setChecked(true);
        mUI->shapeCircleRadioButton->setEnabled(false);

        updateColorComboBoxFromSettings();
        mUI->sizeSlider->setValue((uint) mMRS.pointWidth());
    }
    else {
        this->setEnabled(false);
        mUI->visibilityCheckBox->setChecked(false);
    }
}

void PointsFrame::updateColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->colorComboBox->model());
    assert(model != nullptr);

    // color per vertex
    QStandardItem* item = model->item(P_VERT);
    if (mMRS.canPoints(COLOR_VERTEX)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    // color per mesh
    item = model->item(P_MESH);
    if (mMRS.canPoints(COLOR_MESH)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    if (mMRS.isPoints(COLOR_VERTEX))
        mUI->colorComboBox->setCurrentIndex(P_VERT);
    if (mMRS.isPoints(COLOR_MESH))
        mUI->colorComboBox->setCurrentIndex(P_MESH);
    if (mMRS.isPoints(COLOR_USER))
        mUI->colorComboBox->setCurrentIndex(P_USER);

    mUI->userColorFrame->setEnabled(mMRS.isPoints(COLOR_USER));
    vcl::Color vc = mMRS.pointUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->colorDialogPushButton, c);
}

void PointsFrame::onVisibilityChanged(Qt::CheckState arg1)
{
    mMRS.setPoints(VISIBLE, arg1 == Qt::CheckState::Checked);
    emit settingsUpdated();
}

void PointsFrame::onShapeCircleToggled(bool checked)
{
    // todo
}

void PointsFrame::onShapePixelToggled(bool checked)
{
    // todo
}

void PointsFrame::onShadingVertexToggled(bool checked)
{
    if (checked) {
        mMRS.setPoints(SHADING_VERT);
        emit settingsUpdated();
    }
}

void PointsFrame::onShadingNoneToggled(bool checked)
{
    if (checked) {
        mMRS.setPoints(SHADING_NONE);
        emit settingsUpdated();
    }
}

void PointsFrame::onColorComboBoxChanged(int index)
{
    switch (index) {
    case P_VERT: mMRS.setPoints(COLOR_VERTEX); break;
    case P_MESH: mMRS.setPoints(COLOR_MESH); break;
    case P_USER: mMRS.setPoints(COLOR_USER); break;
    }
    mUI->userColorFrame->setEnabled(index == P_USER);
    emit settingsUpdated();
}

void PointsFrame::onColorDialogButtonClicked()
{
    QColor color =
        QColorDialog::getColor(getButtonBackGround(mUI->colorDialogPushButton));

    if (color.isValid()) {
        setButtonBackGround(mUI->colorDialogPushButton, color);

        mMRS.setPointsUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void PointsFrame::onSizeChanged(int value)
{
    mMRS.setPointsWidth(value);
    emit settingsUpdated();
}

} // namespace vcl::qt
