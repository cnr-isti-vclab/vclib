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

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_SETTINGS_EDIT_MODE_SETTINGS_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_SETTINGS_EDIT_MODE_SETTINGS_FRAME_H

#include <vclib/render/settings/editor_settings.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class EditModeSettingsFrame;
} // namespace Ui

class EditModeSettingsFrame : public QFrame
{
    Q_OBJECT

    Ui::EditModeSettingsFrame* mUI;

public:
    explicit EditModeSettingsFrame(QWidget* parent = nullptr);
    ~EditModeSettingsFrame();

    void setEditMode(EditorSettings::EditMode mode);

signals:
    void editModeChanged(int index);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_SETTINGS_EDIT_MODE_SETTINGS_FRAME_H
