// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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

    void disableEditMode(EditorSettings::EditMode mode);
    void enableEditMode(EditorSettings::EditMode mode);

signals:
    void editModeChanged(int index);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_SETTINGS_EDIT_MODE_SETTINGS_FRAME_H
