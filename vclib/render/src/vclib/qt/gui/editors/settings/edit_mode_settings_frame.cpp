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

#include <vclib/qt/gui/editors/settings/edit_mode_settings_frame.h>

#include "ui_edit_mode_settings_frame.h"

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

} // namespace vcl::qt
