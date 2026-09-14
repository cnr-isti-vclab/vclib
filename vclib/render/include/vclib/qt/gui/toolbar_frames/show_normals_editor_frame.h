// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_SHOW_NORMALS_EDITOR_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_SHOW_NORMALS_EDITOR_FRAME_H

#include "generic_editor_frame.h"

#include <vclib/render/editors/show_normals_editor.h>

#include <vclib/qt/gui/editor_settings_frames/show_normals_editor_settings_frame.h>

namespace vcl::qt {

/**
 * @brief Frame that allows the user to interact with a ShowNormalsEditor.
 *
 * @tparam ViewerType: the type of the viewer.
 */
template<typename ViewerType>
class ShowNormalsEditorFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    std::shared_ptr<vcl::ShowNormalsEditor<ViewerType>> mShowNormalsEditor;

public:
    explicit ShowNormalsEditorFrame(
        std::shared_ptr<vcl::ShowNormalsEditor<ViewerType>> ptr,
        QWidget* parent = nullptr) : GenericEditorFrame(parent)
    {
        mShowNormalsEditor = ptr;

        QIcon ic(":/icons/show_normals.png");

        QPushButton* editorButton = Base::addButton(ic);

        editorButton->setToolTip("Show Normals");

        connect(editorButton, &QPushButton::clicked, this, [this]() {
            if (mShowNormalsEditor) {
                mShowNormalsEditor->setActive(!mShowNormalsEditor->isActive());
            }
        });

        ShowNormalsEditorSettingsFrame* sf =
            Base::setSettingsFrame<ShowNormalsEditorSettingsFrame>(
                mShowNormalsEditor->settings());

        connect(sf, SIGNAL(settingsUpdated()), this, SLOT(refreshSettings()));
        connect(
            sf,
            &ShowNormalsEditorSettingsFrame::contentUpdated,
            this,
            [this]() {
                refreshContent();
            });

        mShowNormalsEditor->setOnStateUpdatedCallback([this, editorButton]() {
            editorButton->setChecked(mShowNormalsEditor->isActive());
        });
    }

private slots:

    void refreshSettings() override
    {
        if (mShowNormalsEditor) {
            mShowNormalsEditor->refreshSettings();
        }
    }

    void refreshContent()
    {
        if (mShowNormalsEditor) {
            mShowNormalsEditor->refresh();
        }
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::ShowNormalsEditor, ViewerType>
{
    using ToolbarFrameType  = ShowNormalsEditorFrame<ViewerType>;
    using SettingsFrameType = ShowNormalsEditorSettingsFrame;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_SHOW_NORMALS_EDITOR_FRAME_H
