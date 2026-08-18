// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/editor_settings_frames/selection_editor_settings_frame.h>

#include "ui_selection_editor_settings_frame.h"

namespace vcl::qt {

SelectionEditorSettingsFrame::SelectionEditorSettingsFrame(
    SelectionEditorSettings& sts,
    QWidget*                 parent) :
        QFrame(parent), mUI(new Ui::SelectionEditorSettingsFrame),
        mSettings(sts)
{
    mUI->setupUi(this);

    mUI->editModeFrame->disableEditMode(EditorSettings::EditMode::NONE);
    mUI->editModeFrame->disableEditMode(EditorSettings::EditMode::ALL_OBJECTS);

    updateGUI();

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

void SelectionEditorSettingsFrame::updateGUI()
{
    // Block signals so we don't trigger settingsUpdated()
    // during the initialization/update of the GUI components.
    bool b1 = mUI->editModeFrame->blockSignals(true);
    bool b2 = mUI->onlyVisibleCheckBox->blockSignals(true);

    mUI->editModeFrame->setEditMode(mSettings.editMode);
    mUI->onlyVisibleCheckBox->setChecked(mSettings.onlyVisible);

    mUI->editModeFrame->blockSignals(b1);
    mUI->onlyVisibleCheckBox->blockSignals(b2);
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
    bool onlyVisible      = state == Qt::CheckState::Checked;
    mSettings.onlyVisible = onlyVisible;
    emit settingsUpdated();
}

} // namespace vcl::qt
