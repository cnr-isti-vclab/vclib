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

PointsFrame::PointsFrame(MeshRenderSettings& settings, QWidget* parent) :
        GenericMeshRenderSettingsFrame(settings, parent),
        mUI(new Ui::PointsFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->visibilityCheckBox,
        SIGNAL(stateChanged(int)),
        this,
        SLOT(onVisibilityChanged(int)));

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
    if (mMRS.canPointCloudBeVisible()) {
        this->setEnabled(true);
        mUI->visibilityCheckBox->setEnabled(true);
        mUI->visibilityCheckBox->setChecked(mMRS.isPointCloudVisible());

        mUI->shadingVertexRadioButton->setEnabled(
            mMRS.canPointCloudShadingBePerVertex());
        mUI->shadingVertexRadioButton->setChecked(
            mMRS.isPointCloudShadingPerVertex());
        mUI->shadingNoneRadioButton->setChecked(mMRS.isPointCloudShadingNone());

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
    if (mMRS.canPointCloudColorBePerVertex()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    // color per mesh
    item = model->item(P_MESH);
    if (mMRS.canPointCloudColorBePerMesh()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    if (mMRS.isPointCloudColorPerVertex())
        mUI->colorComboBox->setCurrentIndex(P_VERT);
    if (mMRS.isPointCloudColorPerMesh())
        mUI->colorComboBox->setCurrentIndex(P_MESH);
    if (mMRS.isPointCloudColorUserDefined())
        mUI->colorComboBox->setCurrentIndex(P_USER);

    mUI->userColorFrame->setEnabled(mMRS.isPointCloudColorUserDefined());
    vcl::Color vc = mMRS.pointCloudUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->colorDialogPushButton, c);
}

void PointsFrame::onVisibilityChanged(int arg1)
{
    mMRS.setPointCloudVisibility(arg1 == Qt::Checked);
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
        mMRS.setPointCloudShadingPerVertex();
        emit settingsUpdated();
    }
}

void PointsFrame::onShadingNoneToggled(bool checked)
{
    if (checked) {
        mMRS.setPointCloudShadingNone();
        emit settingsUpdated();
    }
}

void PointsFrame::onColorComboBoxChanged(int index)
{
    switch (index) {
    case P_VERT: mMRS.setPointCloudColorPerVertex(); break;
    case P_MESH: mMRS.setPointCloudColorPerMesh(); break;
    case P_USER: mMRS.setPointCloudColorUserDefined(); break;
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

        mMRS.setPointCloudUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void PointsFrame::onSizeChanged(int value)
{
    mMRS.setPointWidth(value);
    emit settingsUpdated();
}

} // namespace vcl::qt
