// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
    bool onlyVisible                        = state == Qt::CheckState::Checked;
    mSettings.customSettings["onlyVisible"] = onlyVisible;
    emit settingsUpdated();
}

} // namespace vcl::qt
