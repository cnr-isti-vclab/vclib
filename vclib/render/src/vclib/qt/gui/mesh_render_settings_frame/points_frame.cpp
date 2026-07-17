// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
        SIGNAL(colorChanged(const QColor&)),
        this,
        SLOT(onUserColorChanged(const QColor&)));

    connect(
        mUI->sizeSlider,
        SIGNAL(valueChanged(int)),
        this,
        SLOT(onSizeChanged(int)));

    connect(
        mUI->selectionVisibilityCheckBox,
        SIGNAL(checkStateChanged(Qt::CheckState)),
        this,
        SLOT(onSelectionVisibilityChanged(Qt::CheckState)));

    connect(
        mUI->selectionColorDialogPushButton,
        SIGNAL(colorChanged(const QColor&)),
        this,
        SLOT(onSelectionColorChanged(const QColor&)));
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

        mUI->shapeCircleRadioButton->setChecked(mMRS.isPoints(SHAPE_CIRCLE));
        mUI->shapePixelRadioButton->setChecked(mMRS.isPoints(SHAPE_PIXEL));

        updateColorComboBoxFromSettings();
        mUI->sizeSlider->setValue((uint) mMRS.pointWidth());

        mUI->selectionVisibilityCheckBox->setEnabled(true);
        mUI->selectionVisibilityCheckBox->setChecked(mMRS.isPoints(SELECTION));
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
    mUI->colorDialogPushButton->setBackgroundColor(c);
    vcl::Color sc = mMRS.pointSelectionColor();
    c.setRed(sc.red());
    c.setGreen(sc.green());
    c.setBlue(sc.blue());
    c.setAlpha(sc.alpha());
    mUI->selectionColorDialogPushButton->setBackgroundColor(c);
}

void PointsFrame::onVisibilityChanged(Qt::CheckState arg1)
{
    mMRS.setPoints(VISIBLE, arg1 == Qt::CheckState::Checked);
    emit settingsUpdated();
}

void PointsFrame::onShapeCircleToggled(bool checked)
{
    if (checked) {
        mMRS.setPoints(SHAPE_CIRCLE);
        emit settingsUpdated();
    }
}

void PointsFrame::onShapePixelToggled(bool checked)
{
    if (checked) {
        mMRS.setPoints(SHAPE_PIXEL);
        emit settingsUpdated();
    }
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

void PointsFrame::onUserColorChanged(const QColor& c)
{
    mMRS.setPointsUserColor(c.redF(), c.greenF(), c.blueF(), c.alphaF());
    emit settingsUpdated();
}

void PointsFrame::onSizeChanged(int value)
{
    mMRS.setPointsWidth(value);
    emit settingsUpdated();
}

void PointsFrame::onSelectionVisibilityChanged(Qt::CheckState arg1)
{
    mMRS.setPoints(SELECTION, arg1 == Qt::CheckState::Checked);
    emit settingsUpdated();
}

void PointsFrame::onSelectionColorChanged(const QColor& c)
{
    // alpha is always 0.5
    mMRS.setPointSelectionColor(c.redF(), c.greenF(), c.blueF(), 0.5);
    emit settingsUpdated();
}

} // namespace vcl::qt
