// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_INFO_EDITOR_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_INFO_EDITOR_FRAME_H

#include "generic_editor_frame.h"

#include <vclib/render/editors/info_editor.h>

#include <vclib/qt/gui/editor_settings_frames/info_editor_settings_frame.h>

#include <QApplication>
#include <QStyle>

namespace vcl::qt {

template<typename ViewerType>
class InfoEditorFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    std::shared_ptr<vcl::InfoEditor<ViewerType>> mInfoEditor;

public:
    explicit InfoEditorFrame(
        std::shared_ptr<vcl::InfoEditor<ViewerType>> ptr,
        QWidget* parent = nullptr) : GenericEditorFrame(parent)
    {
        mInfoEditor = ptr;

        QIcon ic = QApplication::style()->standardIcon(
            QStyle::SP_MessageBoxInformation);

        QPushButton* editorButton = Base::addButton(ic, true);
        editorButton->setToolTip("Info");

        connect(
            editorButton, &QPushButton::clicked, this, [this](bool checked) {
                if (mInfoEditor) {
                    mInfoEditor->setActive(checked);
                }
            });

        InfoEditorSettingsFrame* sf =
            Base::setSettingsFrame<InfoEditorSettingsFrame>(
                mInfoEditor->settings());

        connect(sf, SIGNAL(settingsUpdated()), this, SLOT(refreshSettings()));
    }

private slots:

    void refreshSettings() override
    {
        if (mInfoEditor) {
            mInfoEditor->refreshSettings();
        }
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::InfoEditor, ViewerType>
{
    using ToolbarFrameType = InfoEditorFrame<ViewerType>;
    using SettingsFrameType = InfoEditorSettingsFrame;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_INFO_EDITOR_FRAME_H
