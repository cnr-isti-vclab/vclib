// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_SHOW_NORMALS_EDITOR_SETTINGS_FRAME_H
#define VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_SHOW_NORMALS_EDITOR_SETTINGS_FRAME_H

#include <vclib/render/editors/show_normals_editor.h>

#include <QColor>
#include <QFrame>

namespace vcl::qt {

namespace Ui {
class ShowNormalsEditorSettingsFrame;
} // namespace Ui

/**
 * @brief Frame that contains the settings for the ShowNormalsEditor.
 */
class ShowNormalsEditorSettingsFrame : public QFrame
{
    Q_OBJECT

    Ui::ShowNormalsEditorSettingsFrame* mUI;
    ShowNormalsEditorSettings&          mSettings;

public:
    explicit ShowNormalsEditorSettingsFrame(
        ShowNormalsEditorSettings& sts,
        QWidget*                   parent = nullptr);
    ~ShowNormalsEditorSettingsFrame();

    void updateGUI();

signals:
    void settingsUpdated();
    void contentUpdated();

private slots:
    void editModeChanged(int index);

    void onShowVertexNormalsChanged(int state);
    void onShowFaceNormalsChanged(int state);

    void onVertexNormalColorChanged(const QColor& c);
    void onFaceNormalColorChanged(const QColor& c);

    void onNormalLengthRatioChanged(double value);
    void onLinesWidthSliderValueChanged(int value);

    void onResetDefaultClicked();
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_SHOW_NORMALS_EDITOR_SETTINGS_FRAME_H
