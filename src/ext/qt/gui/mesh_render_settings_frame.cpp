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

#include "ui_mesh_render_settings_frame.h"
#include <vclib/ext/qt/gui/mesh_render_settings_frame.h>

#include <QColorDialog>
#include <QStandardItemModel>

namespace vcl::qt {

MeshRenderSettingsFrame::MeshRenderSettingsFrame(QWidget* parent) :
        QFrame(parent), ui(new Ui::MeshRenderSettingsFrame)
{
    ui->setupUi(this);
}

MeshRenderSettingsFrame::~MeshRenderSettingsFrame()
{
    delete ui;
}

const MeshRenderSettings& MeshRenderSettingsFrame::meshRenderSettings() const
{
    return mrs;
}

void MeshRenderSettingsFrame::setMeshRenderSettings(
    const MeshRenderSettings& settings)
{
    mrs = settings;
    updateGuiFromSettings();
}

void MeshRenderSettingsFrame::on_pointVisibilityCheckBox_stateChanged(int arg1)
{
    mrs.setPointCloudVisibility(arg1 == Qt::Checked);
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
        mrs.setPointCloudShadingPerVertex();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_pointShadingNoneRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mrs.setPointCloudShadingNone();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_pointColorComboBox_currentIndexChanged(
    int index)
{
    switch (index) {
    case P_VERT: mrs.setPointCloudColorPerVertex(); break;
    case P_MESH: mrs.setPointCloudColorPerMesh(); break;
    case P_USER: mrs.setPointCloudColorUserDefined(); break;
    }
    ui->pointUserColorFrame->setVisible(index == P_USER);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_pointColorDialogPushButton_clicked()
{
    QColor color = QColorDialog::getColor();

    if (color.isValid()) {
        setButtonBackGround(ui->pointColorDialogPushButton, color);

        mrs.setPointCloudUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_pointSizeSlider_valueChanged(int value)
{
    mrs.setPointWidth(value);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_surfaceVisibilityCheckBox_stateChanged(
    int arg1)
{
    mrs.setSurfaceVisibility(arg1 == Qt::Checked);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_surfaceShadingNoneRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mrs.setSurfaceShadingNone();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_surfaceShadingSmoothRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mrs.setSurfaceShadingSmooth();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_surfaceShadingFlatRadioButton_toggled(
    bool checked)
{
    if (checked) {
        mrs.setSurfaceShadingFlat();
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_surfaceColorComboBox_currentIndexChanged(
    int index)
{
    switch (index) {
    case SC_FACE: mrs.setSurfaceColorPerFace(); break;
    case SC_VERT: mrs.setSurfaceColorPerVertex(); break;
    case SC_MESH: mrs.setSurfaceColorPerMesh(); break;
    case SC_VERT_TEX: mrs.setSurfaceColorPerVertexTexcoords(); break;
    case SC_WEDG_TEX: mrs.setSurfaceColorPerWedgeTexcoords(); break;
    case SC_USER: mrs.setSurfaceColorUserDefined(); break;
    }
    ui->surfaceUserColorFrame->setVisible(index == SC_USER);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_surfaceColorDialogPushButton_clicked()
{
    QColor color = QColorDialog::getColor();

    if (color.isValid()) {
        setButtonBackGround(ui->surfaceColorDialogPushButton, color);

        mrs.setSurfaceUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_wireframeVisibilityCheckBox_stateChanged(
    int arg1)
{
    mrs.setWireframeVisibility(arg1 == Qt::Checked);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_wireframeColorComboBox_currentIndexChanged(
    int index)
{
    switch (index) {
    case W_VERTEX: mrs.setWireframeColorPerVertex(); break;
    case W_MESH: mrs.setWireframeColorPerMesh(); break;
    case W_USER: mrs.setWireframeColorUserDefined(); break;
    }
    ui->wireframeUserColorFrame->setVisible(index == W_USER);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::on_wireframeColorDialogPushButton_clicked()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()) {
        setButtonBackGround(ui->wireframeColorDialogPushButton, color);

        mrs.setWireframeUserColor(
            color.redF(), color.greenF(), color.blueF(), color.alphaF());
        emit settingsUpdated();
    }
}

void MeshRenderSettingsFrame::on_wireframeSizelSlider_valueChanged(int value)
{
    mrs.setWireframeWidth(value);
    emit settingsUpdated();
}

void MeshRenderSettingsFrame::updateGuiFromSettings()
{
    if (mrs.canBeVisible()) {
        ui->tabWidget->setEnabled(true);
        updatePointsTabFromSettings();
        updateSurfaceTabFromSettings();
        updateWireframeTabFromSettings();
    }
    else {
        ui->tabWidget->setEnabled(false);
        ui->pointVisibilityCheckBox->setChecked(false);
        ui->surfaceVisibilityCheckBox->setChecked(false);
        ui->wireframeVisibilityCheckBox->setChecked(false);
    }
}

void MeshRenderSettingsFrame::updatePointsTabFromSettings()
{
    if (mrs.canPointCloudBeVisible()) {
        ui->tabWidget->setCurrentIndex(0);
        ui->pointsTab->setEnabled(true);
        ui->pointVisibilityCheckBox->setEnabled(true);
        ui->pointVisibilityCheckBox->setChecked(mrs.isPointCloudVisible());

        ui->pointShadingVertexRadioButton->setEnabled(
            mrs.canPointCloudShadingBePerVertex());
        ui->pointShadingVertexRadioButton->setChecked(
            mrs.isPointCloudShadingPerVertex());
        ui->pointShadingNoneRadioButton->setChecked(
            mrs.isPointCloudShadingNone());

        // todo
        ui->pointShapePixelRadioButton->setChecked(true);
        ui->pointShapeCircleRadioButton->setEnabled(false);

        updatePointsColorComboBoxFromSettings();
        ui->pointSizeSlider->setValue((uint) mrs.pointWidth());
    }
    else {
        ui->pointsTab->setEnabled(false);
        ui->pointVisibilityCheckBox->setChecked(false);
    }
}

void MeshRenderSettingsFrame::updatePointsColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(ui->pointColorComboBox->model());
    assert(model != nullptr);

    // color per vertex
    QStandardItem* item = model->item(P_VERT);
    if (mrs.canPointCloudColorBePerVertex()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    // color per mesh
    item = model->item(P_MESH);
    if (mrs.canPointCloudColorBePerMesh()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    if (mrs.isPointCloudColorPerVertex())
        ui->pointColorComboBox->setCurrentIndex(P_VERT);
    if (mrs.isPointCloudColorPerMesh())
        ui->pointColorComboBox->setCurrentIndex(P_MESH);
    if (mrs.isPointCloudColorUserDefined())
        ui->pointColorComboBox->setCurrentIndex(P_USER);

    ui->pointUserColorFrame->setVisible(mrs.isPointCloudColorUserDefined());
    vcl::Color vc = mrs.pointCloudUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(ui->pointColorDialogPushButton, c);
}

void MeshRenderSettingsFrame::updateSurfaceTabFromSettings()
{
    if (mrs.canSurfaceBeVisible()) {
        ui->tabWidget->setCurrentIndex(1);
        ui->surfaceTab->setEnabled(true);
        ui->surfaceVisibilityCheckBox->setEnabled(true);
        ui->surfaceVisibilityCheckBox->setChecked(mrs.isSurfaceVisible());
        uptateSurfaceShadingRadioButtonsFromSettings();
        updateSurfaceColorComboBoxFromSettings();
    }
    else {
        ui->surfaceTab->setEnabled(false);
    }
}

void MeshRenderSettingsFrame::uptateSurfaceShadingRadioButtonsFromSettings()
{
    if (!mrs.canSurfaceShadingBeSmooth()) {
        ui->surfaceShadingSmoothRadioButton->setEnabled(false);
    }
    if (!mrs.canSurfaceShadingBeFlat()) {
        ui->surfaceShadingFlatRadioButton->setEnabled(false);
    }
    ui->surfaceShadingNoneRadioButton->setChecked(mrs.isSurfaceShadingNone());
    ui->surfaceShadingFlatRadioButton->setChecked(mrs.isSurfaceShadingFlat());
    ui->surfaceShadingSmoothRadioButton->setChecked(
        mrs.isSurfaceShadingSmooth());
}

void MeshRenderSettingsFrame::updateSurfaceColorComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(ui->surfaceColorComboBox->model());
    assert(model != nullptr);
    QStandardItem* item = model->item(SC_VERT);
    if (mrs.canSurfaceColorBePerVertex()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_FACE);
    if (mrs.canSurfaceColorBePerFace()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_MESH);
    if (mrs.canSurfaceColorBePerMesh()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_VERT_TEX);
    if (mrs.canSurfaceColorBePerVertexTexcoords()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    item = model->item(SC_WEDG_TEX);
    if (mrs.canSurfaceColorBePerWedgeTexcoords()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    if (mrs.isSurfaceColorPerVertex())
        ui->surfaceColorComboBox->setCurrentIndex(SC_VERT);
    if (mrs.isSurfaceColorPerFace())
        ui->surfaceColorComboBox->setCurrentIndex(SC_FACE);
    if (mrs.isSurfaceColorPerMesh())
        ui->surfaceColorComboBox->setCurrentIndex(SC_MESH);
    if (mrs.isSurfaceColorPerVertexTexcoords())
        ui->surfaceColorComboBox->setCurrentIndex(SC_VERT_TEX);
    if (mrs.isSurfaceColorPerWedgeTexcoords())
        ui->surfaceColorComboBox->setCurrentIndex(SC_WEDG_TEX);
    if (mrs.isSurfaceColorUserDefined())
        ui->surfaceColorComboBox->setCurrentIndex(SC_USER);
    ui->surfaceUserColorFrame->setVisible(mrs.isSurfaceColorUserDefined());
    vcl::Color vc = mrs.surfaceUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(ui->surfaceColorDialogPushButton, c);
}

void MeshRenderSettingsFrame::updateWireframeTabFromSettings()
{
    if (mrs.canSurfaceBeVisible()) {
        ui->wireframeTab->setEnabled(true);
        ui->wireframeVisibilityCheckBox->setEnabled(true);
        ui->wireframeVisibilityCheckBox->setChecked(mrs.isWireframeVisible());
        updateWireframeComboBoxFromSettings();
        ui->wireframeSizelSlider->setValue(mrs.wireframeWidth());
    }
    else {
        ui->wireframeTab->setEnabled(false);
    }
}

void MeshRenderSettingsFrame::updateWireframeComboBoxFromSettings()
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(ui->wireframeColorComboBox->model());
    assert(model != nullptr);
    QStandardItem* item = model->item(W_VERTEX);
    if (mrs.canWireframeColorBePerVertex()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    item = model->item(W_MESH);
    if (mrs.canWireframeColorBePerMesh()) {
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
    }
    else {
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    if (mrs.isWireframeColorPerVertex())
        ui->wireframeColorComboBox->setCurrentIndex(W_VERTEX);
    if (mrs.isWireframeColorPerMesh())
        ui->wireframeColorComboBox->setCurrentIndex(W_MESH);
    if (mrs.isWireframeColorUserDefined())
        ui->wireframeColorComboBox->setCurrentIndex(W_USER);

    ui->wireframeUserColorFrame->setVisible(mrs.isWireframeColorUserDefined());
    vcl::Color vc = mrs.wireframeUserColor();
    QColor     c(vc.red(), vc.green(), vc.blue(), vc.alpha());
    setButtonBackGround(ui->wireframeColorDialogPushButton, c);
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
