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

namespace vcl::qt {

MeshRenderSettingsFrame::MeshRenderSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::MeshRenderSettingsFrame)
{
    mUI->setupUi(this);
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

void MeshRenderSettingsFrame::on_pointVisibilityCheckBox_stateChanged(int arg1)
{
    mMRS.setPointCloudVisibility(arg1 == Qt::Checked);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_pointShapeCircleRadioButton_toggled(bool)
{
    // todo
}

void MeshRenderSettingsFrame::on_pointShapePixelRadioButton_toggled(bool)
{
    // todo
}

void MeshRenderSettingsFrame::on_pointShadingVertexRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setPointCloudShadingPerVertex();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_pointShadingNoneRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setPointCloudShadingNone();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_pointColorComboBox_currentIndexChanged(
    int index)
{
    switch (index) {
    case P_VERT: mMRS.setPointCloudColorPerVertex(); break;
    case P_MESH: mMRS.setPointCloudColorPerMesh(); break;
    case P_USER: mMRS.setPointCloudColorUserDefined(); break;
    }
    mUI->pointUserColorFrame->setVisible(index == P_USER);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_pointColorDialogPushButton_clicked()
{
    QColor color = QColorDialog::getColor();

    if (color.isValid()) {
        setButtonBackGround(mUI->pointColorDialogPushButton, color);

        mMRS.setPointCloudUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_pointSizeSlider_valueChanged(int value)
{
    mMRS.setPointWidth(value);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_surfaceVisibilityCheckBox_stateChanged(
    int arg1)
{
    mMRS.setSurfaceVisibility(arg1 == Qt::Checked);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_surfaceShadingNoneRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setSurfaceShadingNone();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_surfaceShadingSmoothRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setSurfaceShadingSmooth();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_surfaceShadingFlatRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mMRS.setSurfaceShadingFlat();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_surfaceColorComboBox_currentIndexChanged(
    int index)
{
    switch (index) {
    case SC_FACE: mMRS.setSurfaceColorPerFace(); break;
    case SC_VERT: mMRS.setSurfaceColorPerVertex(); break;
    case SC_MESH: mMRS.setSurfaceColorPerMesh(); break;
    case SC_VERT_TEX: mMRS.setSurfaceColorPerVertexTexcoords(); break;
    case SC_WEDG_TEX: mMRS.setSurfaceColorPerWedgeTexcoords(); break;
    case SC_USER: mMRS.setSurfaceColorUserDefined(); break;
    }
    mUI->surfaceUserColorFrame->setVisible(index == SC_USER);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_surfaceColorDialogPushButton_clicked()
{
    QColor color = QColorDialog::getColor();

    if (color.isValid()) {
        setButtonBackGround(mUI->surfaceColorDialogPushButton, color);

        mMRS.setSurfaceUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
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
    if (mMRS.canBeVisible()) {
        mUI->tabWidget->setEnabled(true);
        updatePointsTabFromSettings();
        updateSurfaceTabFromSettings();
        updateWireframeTabFromSettings();
        updateEdgesTabFromSettings();
    }
    else {
        mUI->tabWidget->setEnabled(false);
        mUI->pointVisibilityCheckBox->setChecked(false);
        mUI->surfaceVisibilityCheckBox->setChecked(false);
        mUI->wireframeVisibilityCheckBox->setChecked(false);
        mUI->edgesVisibilityCheckBox->setChecked(false);
    }
}

void MeshRenderSettingsFrame::updatePointsTabFromSettings()
{
    if (mMRS.canPointCloudBeVisible()) {
        mUI->tabWidget->setCurrentIndex(0);
        mUI->pointsTab->setEnabled(true);
        mUI->pointVisibilityCheckBox->setEnabled(true);
        mUI->pointVisibilityCheckBox->setChecked(mMRS.isPointCloudVisible());

        mUI->pointShadingVertexRadioButton->setEnabled(
            mMRS.canPointCloudShadingBePerVertex());
        mUI->pointShadingVertexRadioButton->setChecked(
            mMRS.isPointCloudShadingPerVertex());
        mUI->pointShadingNoneRadioButton->setChecked(
            mMRS.isPointCloudShadingNone());

        // todo
        mUI->pointShapePixelRadioButton->setChecked(true);
        mUI->pointShapeCircleRadioButton->setEnabled(false);

        updatePointsColorComboBoxFromSettings();
        mUI->pointSizeSlider->setValue((uint) mMRS.pointWidth());
    }
    else {
        mUI->pointsTab->setEnabled(false);
        mUI->pointVisibilityCheckBox->setChecked(false);
    }
}

void MeshRenderSettingsFrame::updatePointsColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->pointColorComboBox->model());
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
        mUI->pointColorComboBox->setCurrentIndex(P_VERT);
    if (mMRS.isPointCloudColorPerMesh())
        mUI->pointColorComboBox->setCurrentIndex(P_MESH);
    if (mMRS.isPointCloudColorUserDefined())
        mUI->pointColorComboBox->setCurrentIndex(P_USER);

    mUI->pointUserColorFrame->setVisible(mMRS.isPointCloudColorUserDefined());
    vcl::Color vc = mMRS.pointCloudUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->pointColorDialogPushButton, c);
}

void MeshRenderSettingsFrame::updateSurfaceTabFromSettings()
{
    if (mMRS.canSurfaceBeVisible()) {
        mUI->tabWidget->setCurrentIndex(1);
        mUI->surfaceTab->setEnabled(true);
        mUI->surfaceVisibilityCheckBox->setEnabled(true);
        mUI->surfaceVisibilityCheckBox->setChecked(mMRS.isSurfaceVisible());
        uptateSurfaceShadingRadioButtonsFromSettings();
        updateSurfaceColorComboBoxFromSettings();
    }
    else {
        mUI->surfaceTab->setEnabled(false);
    }
}

void MeshRenderSettingsFrame::uptateSurfaceShadingRadioButtonsFromSettings()
{
    if (!mMRS.canSurfaceShadingBeSmooth()) {
        mUI->surfaceShadingSmoothRadioButton->setEnabled(false);
    }
    if (!mMRS.canSurfaceShadingBeFlat()) {
        mUI->surfaceShadingFlatRadioButton->setEnabled(false);
    }
    mUI->surfaceShadingNoneRadioButton->setChecked(mMRS.isSurfaceShadingNone());
    mUI->surfaceShadingFlatRadioButton->setChecked(mMRS.isSurfaceShadingFlat());
    mUI->surfaceShadingSmoothRadioButton->setChecked(
        mMRS.isSurfaceShadingSmooth());
}

void MeshRenderSettingsFrame::updateSurfaceColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->surfaceColorComboBox->model());
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
        mUI->surfaceColorComboBox->setCurrentIndex(SC_VERT);
    if (mMRS.isSurfaceColorPerFace())
        mUI->surfaceColorComboBox->setCurrentIndex(SC_FACE);
    if (mMRS.isSurfaceColorPerMesh())
        mUI->surfaceColorComboBox->setCurrentIndex(SC_MESH);
    if (mMRS.isSurfaceColorPerVertexTexcoords())
        mUI->surfaceColorComboBox->setCurrentIndex(SC_VERT_TEX);
    if (mMRS.isSurfaceColorPerWedgeTexcoords())
        mUI->surfaceColorComboBox->setCurrentIndex(SC_WEDG_TEX);
    if (mMRS.isSurfaceColorUserDefined())
        mUI->surfaceColorComboBox->setCurrentIndex(SC_USER);
    mUI->surfaceUserColorFrame->setVisible(mMRS.isSurfaceColorUserDefined());
    vcl::Color vc = mMRS.surfaceUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(mUI->surfaceColorDialogPushButton, c);
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
