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

#include <vclib/qt/gui/mesh_render_settings_frame/wireframe_frame.h>

#include "ui_wireframe_frame.h"

#include <QColorDialog>
#include <QStandardItemModel>

namespace vcl::qt {

WireframeFrame::WireframeFrame(MeshRenderSettings& settings, QWidget* parent) :
        GenericMeshRenderSettingsFrame(settings, parent),
        mUI(new Ui::WireframeFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->visibilityCheckBox,
        SIGNAL(stateChanged(int)),
        this,
        SLOT(onVisibilityChanged(int)));

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

WireframeFrame::~WireframeFrame()
{
    delete mUI;
}

void WireframeFrame::updateFrameFromSettings()
{
    if (mMRS.canSurfaceBeVisible()) {
        this->setEnabled(true);
        mUI->visibilityCheckBox->setEnabled(true);
        mUI->visibilityCheckBox->setChecked(mMRS.isWireframeVisible());
        mUI->shadingVertexRadioButton->setEnabled(
            mMRS.canWireframeShadingBePerVertex());
        mUI->shadingVertexRadioButton->setChecked(
            mMRS.isWireframeShadingPerVertex());
        mUI->shadingNoneRadioButton->setChecked(mMRS.isWireframeShadingNone());

        updateColorComboBoxFromSettings();
        mUI->sizeSlider->setValue(mMRS.wireframeWidth());
    }
    else {
        this->setEnabled(false);
        mUI->visibilityCheckBox->setChecked(false);
    }
}

void WireframeFrame::updateColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->colorComboBox->model());
    assert(model != nullptr);
    QStandardItem* item = model->item(W_VERTEX);
    if (mMRS.canWireframeColorBePerVertex()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    item = model->item(W_MESH);
    if (mMRS.canWireframeColorBePerMesh()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    if (mMRS.isWireframeColorPerVertex())
        mUI->colorComboBox->setCurrentIndex(W_VERTEX);
    if (mMRS.isWireframeColorPerMesh())
        mUI->colorComboBox->setCurrentIndex(W_MESH);
    if (mMRS.isWireframeColorUserDefined())
        mUI->colorComboBox->setCurrentIndex(W_USER);

    mUI->userColorFrame->setEnabled(mMRS.isWireframeColorUserDefined());
    vcl::Color vc = mMRS.wireframeUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->colorDialogPushButton, c);
}

void WireframeFrame::onVisibilityChanged(int arg1)
{
    mMRS.setWireframeVisibility(arg1 == Qt::Checked);
    emit settingsUpdated();
}

void WireframeFrame::onShadingVertexToggled(bool checked)
{
    if (checked) {
        mMRS.setWireframeShadingPerVertex();
        emit settingsUpdated();
    }
}

void WireframeFrame::onShadingNoneToggled(bool checked)
{
    if (checked) {
        mMRS.setWireframeShadingNone();
        emit settingsUpdated();
    }
}

void WireframeFrame::onColorComboBoxChanged(int index)
{
    switch (index) {
    case W_VERTEX: mMRS.setWireframeColorPerVertex(); break;
    case W_MESH: mMRS.setWireframeColorPerMesh(); break;
    case W_USER: mMRS.setWireframeColorUserDefined(); break;
    }
    mUI->userColorFrame->setEnabled(index == W_USER);
    emit settingsUpdated();
}

void WireframeFrame::onColorDialogButtonClicked()
{
    QColor color =
        QColorDialog::getColor(getButtonBackGround(mUI->colorDialogPushButton));
    if (color.isValid()) {
        setButtonBackGround(mUI->colorDialogPushButton, color);

        mMRS.setWireframeUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void WireframeFrame::onSizeChanged(int value)
{
    mMRS.setWireframeWidth(value);
    emit settingsUpdated();
}

} // namespace vcl::qt
