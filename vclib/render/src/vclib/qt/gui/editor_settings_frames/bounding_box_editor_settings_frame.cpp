// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/editor_settings_frames/bounding_box_editor_settings_frame.h>

#include <vclib/space/core.h>

#include "ui_bounding_box_editor_settings_frame.h"

namespace vcl::qt {

BoundingBoxEditorSettingsFrame::BoundingBoxEditorSettingsFrame(
    BoundingBoxEditorSettings& sts,
    QWidget*                   parent) :
        QFrame(parent), mUI(new Ui::BoundingBoxEditorSettingsFrame),
        mSettings(sts)
{
    mUI->setupUi(this);

    updateGUI();

    connect(
        mUI->editModeFrame,
        &EditModeSettingsFrame::editModeChanged,
        this,
        &BoundingBoxEditorSettingsFrame::editModeChanged);

    connect(
        mUI->linesWidthSlider,
        &QSlider::valueChanged,
        this,
        &BoundingBoxEditorSettingsFrame::onLinesWidthSliderValueChanged);

    connect(
        mUI->colorPushButton,
        SIGNAL(colorChanged(const QColor&)),
        this,
        SLOT(onColorChanged(const QColor&)));

    connect(
        mUI->resetDefaultButton,
        SIGNAL(clicked()),
        this,
        SLOT(onResetDefaultClicked()));
}

BoundingBoxEditorSettingsFrame::~BoundingBoxEditorSettingsFrame()
{
    delete mUI;
}

void BoundingBoxEditorSettingsFrame::updateGUI()
{
    // Block signals so we don't trigger settingsUpdated()
    // during the initialization/update of the GUI components.
    bool b1 = mUI->editModeFrame->blockSignals(true);
    bool b2 = mUI->linesWidthSlider->blockSignals(true);
    bool b3 = mUI->colorPushButton->blockSignals(true);

    Color c         = mSettings.color;
    float thickness = mSettings.thickness;

    mUI->editModeFrame->setEditMode(mSettings.editMode);
    mUI->linesWidthSlider->setValue(int(thickness));
    mUI->colorPushButton->setBackgroundColor(
        QColor(c.red(), c.green(), c.blue(), c.alpha()));

    mUI->editModeFrame->blockSignals(b1);
    mUI->linesWidthSlider->blockSignals(b2);
    mUI->colorPushButton->blockSignals(b3);
}

void BoundingBoxEditorSettingsFrame::editModeChanged(int index)
{
    using enum EditorSettings::EditMode;
    assert(index <= toUnderlying(ALL_OBJECTS));

    mSettings.editMode = static_cast<EditorSettings::EditMode>(index);
    emit settingsUpdated();
}

void BoundingBoxEditorSettingsFrame::onLinesWidthSliderValueChanged(int value)
{
    mSettings.thickness = float(value);
    emit settingsUpdated();
}

void BoundingBoxEditorSettingsFrame::onColorChanged(const QColor& c)
{
    mSettings.color = Color(c.red(), c.green(), c.blue(), c.alpha());
    emit settingsUpdated();
}

void BoundingBoxEditorSettingsFrame::onResetDefaultClicked()
{
    mSettings.resetDefaults();
    updateGUI();
    emit settingsUpdated();
}

} // namespace vcl::qt
