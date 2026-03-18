/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/qt/gui/toolbar_frames/settings/bounding_box_editor_settings_frame.h>

#include <vclib/space/core.h>

#include "ui_bounding_box_editor_settings_frame.h"

namespace vcl::qt {

BoundingBoxEditorSettingsFrame::BoundingBoxEditorSettingsFrame(
    EditorSettings& sts,
    QWidget*        parent) :
        QFrame(parent), mUI(new Ui::BoundingBoxEditorSettingsFrame),
        mSettings(sts)
{
    mUI->setupUi(this);

    assert(mSettings.customSettings.at("color").has_value());
    assert(mSettings.customSettings.at("thickness").has_value());

    Color c = std::any_cast<Color>(mSettings.customSettings.at("color"));

    float thickness =
        std::any_cast<float>(mSettings.customSettings.at("thickness"));

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
    mSettings.customSettings["thickness"] = float(value);
    emit settingsUpdated();
}

void BoundingBoxEditorSettingsFrame::onColorChanged(const QColor& c)
{
    mSettings.customSettings["color"] =
        Color(c.red(), c.green(), c.blue(), c.alpha());
    emit settingsUpdated();
}

} // namespace vcl::qt
