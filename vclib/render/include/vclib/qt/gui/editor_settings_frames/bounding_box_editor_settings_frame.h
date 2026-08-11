// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_BOUNDING_BOX_EDITOR_SETTINGS_FRAME_H
#define VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_BOUNDING_BOX_EDITOR_SETTINGS_FRAME_H

#include <vclib/render/editors/bounding_box_editor.h>

#include <QColor>
#include <QFrame>

namespace vcl::qt {

namespace Ui {
class BoundingBoxEditorSettingsFrame;
} // namespace Ui

class BoundingBoxEditorSettingsFrame : public QFrame
{
    Q_OBJECT

    Ui::BoundingBoxEditorSettingsFrame* mUI;
    BoundingBoxEditorSettings&          mSettings;

public:
    explicit BoundingBoxEditorSettingsFrame(
        BoundingBoxEditorSettings& sts,
        QWidget*                   parent = nullptr);
    ~BoundingBoxEditorSettingsFrame();

    void updateGUI();

signals:
    void settingsUpdated();

private slots:
    void editModeChanged(int index);

    void onLinesWidthSliderValueChanged(int value);

    void onColorChanged(const QColor& c);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_BOUNDING_BOX_EDITOR_SETTINGS_FRAME_H
