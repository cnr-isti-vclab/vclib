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

#include "ui_mesh_render_settings_frame.h"
#include <vclib/ext/qt/gui/mesh_render_settings_frame.h>

#include <QColorDialog>
#include <QStandardItemModel>

#include <vclib/ext/qt/gui/mesh_render_settings_frame/points_frame.h>
#include <vclib/ext/qt/gui/mesh_render_settings_frame/surface_frame.h>

namespace vcl::qt {

MeshRenderSettingsFrame::MeshRenderSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::MeshRenderSettingsFrame)
{
    mUI->setupUi(this);

    auto* pointsFrame = new PointsFrame(mMRS, this);
    mUI->tabWidget->addTab(pointsFrame, "Points");
    frames.push_back(pointsFrame);

    auto* surfaceFrame = new SurfaceFrame(mMRS, this);
    mUI->tabWidget->addTab(surfaceFrame, "Surface");
    frames.push_back(surfaceFrame);

    for (auto* frame : frames) {
        connect(
            frame, SIGNAL(settingsUpdated()), this, SIGNAL(settingsUpdated()));
    }
}

MeshRenderSettingsFrame::~MeshRenderSettingsFrame()
{
    delete mUI;
}

const MeshRenderSettings& MeshRenderSettingsFrame::meshRenderSettings() const
{
    return mMRS;
}

void MeshRenderSettingsFrame::setMeshRenderSettings(
    const MeshRenderSettings& settings)
{
    mMRS = settings;
    updateGuiFromSettings();
}

void MeshRenderSettingsFrame::on_wireframeVisibilityCheckBox_stateChanged(
    int arg1)
{
    mMRS.setWireframeVisibility(arg1 == Qt::Checked);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_wireframeShadingNoneRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setWireframeShadingNone();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_wireframeShadingVertexRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setWireframeShadingPerVertex();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_wireframeColorComboBox_currentIndexChanged(
    int index)
{
    switch (index) {
    case W_VERTEX: mMRS.setWireframeColorPerVertex(); break;
    case W_MESH: mMRS.setWireframeColorPerMesh(); break;
    case W_USER: mMRS.setWireframeColorUserDefined(); break;
    }
    mUI->wireframeUserColorFrame->setVisible(index == W_USER);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_wireframeColorDialogPushButton_clicked()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()) {
        setButtonBackGround(mUI->wireframeColorDialogPushButton, color);

        mMRS.setWireframeUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_wireframeSizeSlider_valueChanged(int value)
{
    mMRS.setWireframeWidth(value);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_edgesVisibilityCheckBox_stateChanged(int arg1)
{
    mMRS.setEdgesVisibility(arg1 == Qt::Checked);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_edgesShadingSmoothRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setEdgesShadingSmooth();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_edgesShadingFlatRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setEdgesShadingFlat();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_edgesShadingNoneRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setEdgesShadingNone();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_edgesColorComboBox_currentIndexChanged(
    int index)
{
    switch (index) {
    case E_VERTEX: mMRS.setEdgesColorPerVertex(); break;
    case E_EDGES: mMRS.setEdgesColorPerEdge(); break;
    case E_MESH: mMRS.setEdgesColorPerMesh(); break;
    case E_USER: mMRS.setEdgesColorUserDefined(); break;
    }
    mUI->edgesUserColorFrame->setVisible(index == E_USER);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_edgesColorDialogPushButton_clicked()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()) {
        setButtonBackGround(mUI->edgesColorDialogPushButton, color);

        mMRS.setEdgesUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_edgesSizeSlider_valueChanged(int value)
{
    mMRS.setEdgesWidth(value);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::updateGuiFromSettings()
{
    for (auto* frame : frames) {
        frame->updateFrameFromSettings();
    }

    if (mMRS.canBeVisible()) {
        mUI->tabWidget->setEnabled(true);
        updateWireframeTabFromSettings();
        updateEdgesTabFromSettings();
    }
    else {
        mUI->tabWidget->setEnabled(false);
        // mUI->pointVisibilityCheckBox->setChecked(false);
        // mUI->surfaceVisibilityCheckBox->setChecked(false);
        mUI->wireframeVisibilityCheckBox->setChecked(false);
        mUI->edgesVisibilityCheckBox->setChecked(false);
    }
}

void MeshRenderSettingsFrame::updateWireframeTabFromSettings()
{
    if (mMRS.canSurfaceBeVisible()) {
        mUI->wireframeTab->setEnabled(true);
        mUI->wireframeVisibilityCheckBox->setEnabled(true);
        mUI->wireframeVisibilityCheckBox->setChecked(mMRS.isWireframeVisible());
        mUI->wireframeShadingVertexRadioButton->setEnabled(
            mMRS.canWireframeShadingBePerVertex());
        mUI->wireframeShadingVertexRadioButton->setChecked(
            mMRS.isWireframeShadingPerVertex());
        mUI->wireframeShadingNoneRadioButton->setChecked(
            mMRS.isWireframeShadingNone());

        updateWireframeComboBoxFromSettings();
        mUI->wireframeSizeSlider->setValue(mMRS.wireframeWidth());
    }
    else {
        mUI->wireframeTab->setEnabled(false);
    }
}

void MeshRenderSettingsFrame::updateWireframeComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->wireframeColorComboBox->model());
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
        mUI->wireframeColorComboBox->setCurrentIndex(W_VERTEX);
    if (mMRS.isWireframeColorPerMesh())
        mUI->wireframeColorComboBox->setCurrentIndex(W_MESH);
    if (mMRS.isWireframeColorUserDefined())
        mUI->wireframeColorComboBox->setCurrentIndex(W_USER);

    mUI->wireframeUserColorFrame->setVisible(
        mMRS.isWireframeColorUserDefined());
    vcl::Color vc = mMRS.wireframeUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->wireframeColorDialogPushButton, c);
}

void MeshRenderSettingsFrame::updateEdgesTabFromSettings()
{
    if (mMRS.canEdgesBeVisible()) {
        mUI->edgesTab->setEnabled(true);
        mUI->edgesVisibilityCheckBox->setEnabled(true);
        mUI->edgesVisibilityCheckBox->setChecked(mMRS.isEdgesVisible());
        mUI->edgesShadingSmoothRadioButton->setEnabled(
            mMRS.canEdgesShadingBeSmooth());
        mUI->edgesShadingSmoothRadioButton->setChecked(
            mMRS.isEdgesShadingSmooth());
        mUI->edgesShadingFlatRadioButton->setEnabled(
            mMRS.canEdgesShadingBeFlat());
        mUI->edgesShadingFlatRadioButton->setChecked(mMRS.isEdgesShadingFlat());
        mUI->edgesShadingNoneRadioButton->setChecked(mMRS.isEdgesShadingNone());

        updateEdgesComboBoxFromSettings();
        mUI->edgesSizeSlider->setValue(mMRS.edgesWidth());
    }
    else {
        mUI->edgesTab->setEnabled(false);
        mUI->edgesVisibilityCheckBox->setEnabled(false);
    }
}

void MeshRenderSettingsFrame::updateEdgesComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->edgesColorComboBox->model());
    assert(model != nullptr);
    QStandardItem* item = model->item(E_VERTEX);
    if (mMRS.canEdgesColorBePerVertex()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    item = model->item(E_EDGES);
    if (mMRS.canEdgesColorBePerEdge()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    item = model->item(E_MESH);
    if (mMRS.canEdgesColorBePerMesh()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    if (mMRS.isEdgesColorPerVertex())
        mUI->edgesColorComboBox->setCurrentIndex(E_VERTEX);
    if (mMRS.isEdgesColorPerEdge())
        mUI->edgesColorComboBox->setCurrentIndex(E_EDGES);
    if (mMRS.isEdgesColorPerMesh())
        mUI->edgesColorComboBox->setCurrentIndex(E_MESH);
    if (mMRS.isEdgesColorUserDefined())
        mUI->edgesColorComboBox->setCurrentIndex(E_USER);

    mUI->edgesUserColorFrame->setVisible(mMRS.isEdgesColorUserDefined());
    vcl::Color vc = mMRS.edgesUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->edgesColorDialogPushButton, c);
}

void MeshRenderSettingsFrame::setButtonBackGround(
    QPushButton*  b,
    const QColor& c)
{
    QPalette px;
    px.setColor(QPalette::Button, c);
    b->setPalette(px);
    b->update();
}

QColor MeshRenderSettingsFrame::getButtonBackGround(QPushButton* b)
{
    QPalette px = b->palette();
    return px.color(QPalette::Button);
}

} // namespace vcl::qt
