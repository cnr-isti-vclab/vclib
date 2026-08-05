// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_SETTINGS_INFO_EDITOR_SETTINGS_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_SETTINGS_INFO_EDITOR_SETTINGS_FRAME_H

#include <vclib/render/settings/editor_settings.h>

#include <QColor>
#include <QFrame>

namespace vcl::qt {

namespace Ui {
class InfoEditorSettingsFrame;
} // namespace Ui

class InfoEditorSettingsFrame : public QFrame
{
    Q_OBJECT

    Ui::InfoEditorSettingsFrame* mUI;
    EditorSettings&              mSettings;

public:
    explicit InfoEditorSettingsFrame(
        EditorSettings& sts,
        QWidget*        parent = nullptr);
    ~InfoEditorSettingsFrame();

signals:
    void settingsUpdated();

private slots:
    void onLinesWidthSliderValueChanged(int value);

    void onColorChanged(const QColor& c);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_SETTINGS_INFO_EDITOR_SETTINGS_FRAME_H
