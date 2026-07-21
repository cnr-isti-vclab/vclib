// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/toolbar_frames/settings/edit_mode_settings_frame.h>

#include "ui_edit_mode_settings_frame.h"

#include <QStandardItemModel>

namespace vcl::qt {

EditModeSettingsFrame::EditModeSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::EditModeSettingsFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->editModeComboBox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        &EditModeSettingsFrame::editModeChanged);
}

EditModeSettingsFrame::~EditModeSettingsFrame()
{
    delete mUI;
}

void EditModeSettingsFrame::setEditMode(EditorSettings::EditMode mode)
{
    mUI->editModeComboBox->setCurrentIndex(toUnderlying(mode));
}

void EditModeSettingsFrame::disableEditMode(EditorSettings::EditMode mode)
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->editModeComboBox->model());
    if (model) {
        QStandardItem* item = model->item(toUnderlying(mode));

        if (item) {
            item->setEnabled(false);
        }
    }
}

void EditModeSettingsFrame::enableEditMode(EditorSettings::EditMode mode)
{
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mUI->editModeComboBox->model());
    if (model) {
        QStandardItem* item = model->item(toUnderlying(mode));

        if (item) {
            item->setEnabled(true);
        }
    }
}

} // namespace vcl::qt
