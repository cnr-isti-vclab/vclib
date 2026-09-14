// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/editor_settings_frames/show_normals_editor_settings_frame.h>

#include <vclib/space/core.h>

#include "ui_show_normals_editor_settings_frame.h"

namespace vcl::qt {

ShowNormalsEditorSettingsFrame::ShowNormalsEditorSettingsFrame(
    ShowNormalsEditorSettings& sts,
    QWidget*                   parent) :
        QFrame(parent), mUI(new Ui::ShowNormalsEditorSettingsFrame),
        mSettings(sts)
{
    mUI->setupUi(this);

    updateGUI();

    connect(
        mUI->editModeFrame,
        &EditModeSettingsFrame::editModeChanged,
        this,
        &ShowNormalsEditorSettingsFrame::editModeChanged);

    connect(
        mUI->showVertexNormalsCheckBox,
        &QCheckBox::stateChanged,
        this,
        &ShowNormalsEditorSettingsFrame::onShowVertexNormalsChanged);

    connect(
        mUI->showFaceNormalsCheckBox,
        &QCheckBox::stateChanged,
        this,
        &ShowNormalsEditorSettingsFrame::onShowFaceNormalsChanged);

    connect(
        mUI->vertexNormalColorButton,
        SIGNAL(colorChanged(const QColor&)),
        this,
        SLOT(onVertexNormalColorChanged(const QColor&)));

    connect(
        mUI->faceNormalColorButton,
        SIGNAL(colorChanged(const QColor&)),
        this,
        SLOT(onFaceNormalColorChanged(const QColor&)));

    connect(
        mUI->normalLengthRatioSpinBox,
        SIGNAL(valueChanged(double)),
        this,
        SLOT(onNormalLengthRatioChanged(double)));

    connect(
        mUI->linesWidthSlider,
        &QSlider::valueChanged,
        this,
        &ShowNormalsEditorSettingsFrame::onLinesWidthSliderValueChanged);

    connect(
        mUI->resetDefaultButton,
        SIGNAL(clicked()),
        this,
        SLOT(onResetDefaultClicked()));
}

ShowNormalsEditorSettingsFrame::~ShowNormalsEditorSettingsFrame()
{
    delete mUI;
}

void ShowNormalsEditorSettingsFrame::updateGUI()
{
    bool b1 = mUI->editModeFrame->blockSignals(true);
    bool b2 = mUI->showVertexNormalsCheckBox->blockSignals(true);
    bool b3 = mUI->showFaceNormalsCheckBox->blockSignals(true);
    bool b4 = mUI->vertexNormalColorButton->blockSignals(true);
    bool b5 = mUI->faceNormalColorButton->blockSignals(true);
    bool b6 = mUI->normalLengthRatioSpinBox->blockSignals(true);
    bool b7 = mUI->linesWidthSlider->blockSignals(true);

    mUI->editModeFrame->setEditMode(mSettings.editMode);
    mUI->showVertexNormalsCheckBox->setChecked(mSettings.showVertexNormals);
    mUI->showFaceNormalsCheckBox->setChecked(mSettings.showFaceNormals);

    Color vc = mSettings.vertexNormalColor;
    mUI->vertexNormalColorButton->setBackgroundColor(
        QColor(vc.red(), vc.green(), vc.blue(), vc.alpha()));

    Color fc = mSettings.faceNormalColor;
    mUI->faceNormalColorButton->setBackgroundColor(
        QColor(fc.red(), fc.green(), fc.blue(), fc.alpha()));

    mUI->normalLengthRatioSpinBox->setValue(mSettings.normalLengthRatio);
    mUI->linesWidthSlider->setValue(int(mSettings.thickness));

    mUI->editModeFrame->blockSignals(b1);
    mUI->showVertexNormalsCheckBox->blockSignals(b2);
    mUI->showFaceNormalsCheckBox->blockSignals(b3);
    mUI->vertexNormalColorButton->blockSignals(b4);
    mUI->faceNormalColorButton->blockSignals(b5);
    mUI->normalLengthRatioSpinBox->blockSignals(b6);
    mUI->linesWidthSlider->blockSignals(b7);
}

void ShowNormalsEditorSettingsFrame::editModeChanged(int index)
{
    using enum EditorSettings::EditMode;
    assert(index <= toUnderlying(ALL_OBJECTS));

    mSettings.editMode = static_cast<EditorSettings::EditMode>(index);
    emit settingsUpdated();
}

void ShowNormalsEditorSettingsFrame::onShowVertexNormalsChanged(int state)
{
    mSettings.showVertexNormals = (state == Qt::Checked);
    emit settingsUpdated();
}

void ShowNormalsEditorSettingsFrame::onShowFaceNormalsChanged(int state)
{
    mSettings.showFaceNormals = (state == Qt::Checked);
    emit settingsUpdated();
}

void ShowNormalsEditorSettingsFrame::onVertexNormalColorChanged(const QColor& c)
{
    mSettings.vertexNormalColor =
        Color(c.red(), c.green(), c.blue(), c.alpha());
    emit settingsUpdated();
}

void ShowNormalsEditorSettingsFrame::onFaceNormalColorChanged(const QColor& c)
{
    mSettings.faceNormalColor = Color(c.red(), c.green(), c.blue(), c.alpha());
    emit settingsUpdated();
}

void ShowNormalsEditorSettingsFrame::onNormalLengthRatioChanged(double value)
{
    mSettings.normalLengthRatio = value;
    emit settingsUpdated();
}

void ShowNormalsEditorSettingsFrame::onLinesWidthSliderValueChanged(int value)
{
    mSettings.thickness = float(value);
    emit settingsUpdated();
}

void ShowNormalsEditorSettingsFrame::onResetDefaultClicked()
{
    mSettings.resetDefaults();
    updateGUI();
    emit settingsUpdated();
}

} // namespace vcl::qt
