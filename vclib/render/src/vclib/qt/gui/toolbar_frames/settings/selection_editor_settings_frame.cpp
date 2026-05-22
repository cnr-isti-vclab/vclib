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

#include <vclib/qt/gui/toolbar_frames/settings/selection_editor_settings_frame.h>

#include "ui_selection_editor_settings_frame.h"

namespace vcl::qt {

SelectionEditorSettingsFrame::SelectionEditorSettingsFrame(
    EditorSettings& sts,
    QWidget*        parent) :
        QFrame(parent), mUI(new Ui::SelectionEditorSettingsFrame),
        mSettings(sts)
{
    mUI->setupUi(this);

    assert(mSettings.customSettings.at("onlyVisible").has_value());

    bool onlyVisible =
        std::any_cast<bool>(mSettings.customSettings.at("onlyVisible"));

    mUI->editModeFrame->disableEditMode(EditorSettings::EditMode::NONE);
    mUI->editModeFrame->disableEditMode(EditorSettings::EditMode::ALL_OBJECTS);

    mUI->editModeFrame->setEditMode(mSettings.editMode);
    mUI->onlyVisibleCheckBox->setChecked(onlyVisible);

    connect(
        mUI->editModeFrame,
        &EditModeSettingsFrame::editModeChanged,
        this,
        &SelectionEditorSettingsFrame::editModeChanged);

    connect(
        mUI->onlyVisibleCheckBox,
        &QCheckBox::checkStateChanged,
        this,
        &SelectionEditorSettingsFrame::onlyVisibleCheckBoxChanged);
}

SelectionEditorSettingsFrame::~SelectionEditorSettingsFrame()
{
    delete mUI;
}

void SelectionEditorSettingsFrame::editModeChanged(int index)
{
    using enum EditorSettings::EditMode;
    assert(index <= toUnderlying(ALL_OBJECTS));

    mSettings.editMode = static_cast<EditorSettings::EditMode>(index);
    emit settingsUpdated();
}

void SelectionEditorSettingsFrame::onlyVisibleCheckBoxChanged(
    Qt::CheckState state)
{
    bool onlyVisible = state == Qt::CheckState::Checked;
    mSettings.customSettings["onlyVisible"] = onlyVisible;
    emit settingsUpdated();
}

} // namespace vcl::qt
