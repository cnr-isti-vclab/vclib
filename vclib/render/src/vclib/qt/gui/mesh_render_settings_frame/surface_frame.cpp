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

#include <vclib/qt/gui/mesh_render_settings_frame/surface_frame.h>

#include "ui_surface_frame.h"

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
    if (mMRS.canSurface(VISIBLE)) {
        this->setEnabled(true);
        mUI->visibilityCheckBox->setEnabled(true);
        mUI->visibilityCheckBox->setChecked(mMRS.isSurface(VISIBLE));
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
    if (!mMRS.canSurface(SHADING_SMOOTH)) {
        mUI->shadingSmoothRadioButton->setEnabled(false);
    }
    if (!mMRS.canSurface(SHADING_FLAT)) {
        mUI->shadingFlatRadioButton->setEnabled(false);
    }
    mUI->shadingNoneRadioButton->setChecked(mMRS.isSurface(SHADING_NONE));
    mUI->shadingFlatRadioButton->setChecked(mMRS.isSurface(SHADING_FLAT));
    mUI->shadingSmoothRadioButton->setChecked(mMRS.isSurface(SHADING_SMOOTH));
}

void SurfaceFrame::updateColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->colorComboBox->model());
    assert(model != nullptr);
    QStandardItem* item = model->item(SC_VERT);
    if (mMRS.canSurface(COLOR_VERTEX)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_FACE);
    if (mMRS.canSurface(COLOR_FACE)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_MESH);
    if (mMRS.canSurface(COLOR_MESH)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_VERT_TEX);
    if (mMRS.canSurface(COLOR_VERTEX_TEX)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_WEDG_TEX);
    if (mMRS.canSurface(COLOR_WEDGE_TEX)) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    if (mMRS.isSurface(COLOR_VERTEX))
        mUI->colorComboBox->setCurrentIndex(SC_VERT);
    if (mMRS.isSurface(COLOR_FACE))
        mUI->colorComboBox->setCurrentIndex(SC_FACE);
    if (mMRS.isSurface(COLOR_MESH))
        mUI->colorComboBox->setCurrentIndex(SC_MESH);
    if (mMRS.isSurface(COLOR_VERTEX_TEX))
        mUI->colorComboBox->setCurrentIndex(SC_VERT_TEX);
    if (mMRS.isSurface(COLOR_WEDGE_TEX))
        mUI->colorComboBox->setCurrentIndex(SC_WEDG_TEX);
    if (mMRS.isSurface(COLOR_USER))
        mUI->colorComboBox->setCurrentIndex(SC_USER);
    mUI->userColorFrame->setEnabled(mMRS.isSurface(COLOR_USER));
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
    mUI->userColorFrame->setEnabled(index == SC_USER);
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
