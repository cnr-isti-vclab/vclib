// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_SELECTION_EDITOR_SETTINGS_FRAME_H
#define VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_SELECTION_EDITOR_SETTINGS_FRAME_H

#include <vclib/render/editors/selection_editor.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class SelectionEditorSettingsFrame;
} // namespace Ui

class SelectionEditorSettingsFrame : public QFrame
{
    Q_OBJECT

    Ui::SelectionEditorSettingsFrame* mUI;
    SelectionEditorSettings&          mSettings;

public:
    explicit SelectionEditorSettingsFrame(
        SelectionEditorSettings& sts,
        QWidget*                 parent = nullptr);
    ~SelectionEditorSettingsFrame();

    void updateGUI();

signals:
    void settingsUpdated();

private slots:
    void editModeChanged(int index);

    void onlyVisibleCheckBoxChanged(Qt::CheckState state);

    void onResetDefaultClicked();
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_SELECTION_EDITOR_SETTINGS_FRAME_H
