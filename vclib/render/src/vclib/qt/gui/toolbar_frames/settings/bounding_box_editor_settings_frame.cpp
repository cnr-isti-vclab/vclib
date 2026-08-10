// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/toolbar_frames/settings/bounding_box_editor_settings_frame.h>

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

    Color c         = mSettings.color;
    float thickness = mSettings.thickness;

    mUI->editModeFrame->setEditMode(mSettings.editMode);
    mUI->linesWidthSlider->setValue(int(thickness));
    mUI->colorPushButton->setBackgroundColor(
        QColor(c.red(), c.green(), c.blue(), c.alpha()));

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
}

BoundingBoxEditorSettingsFrame::~BoundingBoxEditorSettingsFrame()
{
    delete mUI;
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

} // namespace vcl::qt
