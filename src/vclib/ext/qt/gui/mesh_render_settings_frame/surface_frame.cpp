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

#include "ui_surface_frame.h"
#include <vclib/ext/qt/gui/mesh_render_settings_frame/surface_frame.h>

#include <QColorDialog>
#include <QStandardItemModel>

namespace vcl::qt {

SurfaceFrame::SurfaceFrame(MeshRenderSettings& settings, QWidget* parent) :
        GenericMeshRenderSettingsFrame(settings, parent),
        mUI(new Ui::SurfaceFrame)
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
}

SurfaceFrame::~SurfaceFrame()
{
    delete mUI;
}

void SurfaceFrame::updateFrameFromSettings()
{
    if (mMRS.canSurfaceBeVisible()) {
        this->setEnabled(true);
        mUI->visibilityCheckBox->setEnabled(true);
        mUI->visibilityCheckBox->setChecked(mMRS.isSurfaceVisible());
        uptateShadingRadioButtonsFromSettings();
        updateColorComboBoxFromSettings();
    }
    else {
        this->setEnabled(false);
        mUI->visibilityCheckBox->setChecked(false);
    }
}


void SurfaceFrame::uptateShadingRadioButtonsFromSettings()
{
    if (!mMRS.canSurfaceShadingBeSmooth()) {
        mUI->shadingSmoothRadioButton->setEnabled(false);
    }
    if (!mMRS.canSurfaceShadingBeFlat()) {
        mUI->shadingFlatRadioButton->setEnabled(false);
    }
    mUI->shadingNoneRadioButton->setChecked(mMRS.isSurfaceShadingNone());
    mUI->shadingFlatRadioButton->setChecked(mMRS.isSurfaceShadingFlat());
    mUI->shadingSmoothRadioButton->setChecked(mMRS.isSurfaceShadingSmooth());
}

void SurfaceFrame::updateColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->colorComboBox->model());
    assert(model != nullptr);
    QStandardItem* item = model->item(SC_VERT);
    if (mMRS.canSurfaceColorBePerVertex()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_FACE);
    if (mMRS.canSurfaceColorBePerFace()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_MESH);
    if (mMRS.canSurfaceColorBePerMesh()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_VERT_TEX);
    if (mMRS.canSurfaceColorBePerVertexTexcoords()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_WEDG_TEX);
    if (mMRS.canSurfaceColorBePerWedgeTexcoords()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    if (mMRS.isSurfaceColorPerVertex())
        mUI->colorComboBox->setCurrentIndex(SC_VERT);
    if (mMRS.isSurfaceColorPerFace())
        mUI->colorComboBox->setCurrentIndex(SC_FACE);
    if (mMRS.isSurfaceColorPerMesh())
        mUI->colorComboBox->setCurrentIndex(SC_MESH);
    if (mMRS.isSurfaceColorPerVertexTexcoords())
        mUI->colorComboBox->setCurrentIndex(SC_VERT_TEX);
    if (mMRS.isSurfaceColorPerWedgeTexcoords())
        mUI->colorComboBox->setCurrentIndex(SC_WEDG_TEX);
    if (mMRS.isSurfaceColorUserDefined())
        mUI->colorComboBox->setCurrentIndex(SC_USER);
    mUI->userColorFrame->setVisible(mMRS.isSurfaceColorUserDefined());
    vcl::Color vc = mMRS.surfaceUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->colorDialogPushButton, c);
}

void SurfaceFrame::onVisibilityChanged(int arg1)
{
    mMRS.setSurfaceVisibility(arg1 == Qt::Checked);
    emit settingsUpdated();
}


void SurfaceFrame::onShadingSmoothToggled(bool checked)
{
    if (checked) {
        mMRS.setSurfaceShadingSmooth();
        emit settingsUpdated();
    }
}


void SurfaceFrame::onShadingFlatToggled(bool checked)
{
    if (checked) {
        mMRS.setSurfaceShadingFlat();
        emit settingsUpdated();
    }
}

void SurfaceFrame::onShadingNoneToggled(bool checked)
{
    if (checked) {
        mMRS.setSurfaceShadingNone();
        emit settingsUpdated();
    }
}

void SurfaceFrame::onColorComboBoxChanged(int index)
{
    switch (index) {
    case SC_FACE: mMRS.setSurfaceColorPerFace(); break;
    case SC_VERT: mMRS.setSurfaceColorPerVertex(); break;
    case SC_MESH: mMRS.setSurfaceColorPerMesh(); break;
    case SC_VERT_TEX: mMRS.setSurfaceColorPerVertexTexcoords(); break;
    case SC_WEDG_TEX: mMRS.setSurfaceColorPerWedgeTexcoords(); break;
    case SC_USER: mMRS.setSurfaceColorUserDefined(); break;
    }
    mUI->userColorFrame->setVisible(index == SC_USER);
    emit settingsUpdated();
}

void SurfaceFrame::onColorDialogButtonClicked()
{
    QColor color =
        QColorDialog::getColor(getButtonBackGround(mUI->colorDialogPushButton));

    if (color.isValid()) {
        setButtonBackGround(mUI->colorDialogPushButton, color);

        mMRS.setSurfaceUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

} // namespace vcl::qt
