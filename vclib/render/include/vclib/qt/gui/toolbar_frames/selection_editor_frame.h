// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H

#include "generic_editor_frame.h"

#include <vclib/render/editors/selection_editor.h>

#include <vclib/qt/gui/editor_settings_frames/selection_editor_settings_frame.h>

namespace vcl::qt {

template<typename ViewerType>
class SelectionEditorFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    std::shared_ptr<vcl::SelectionEditor<ViewerType>> mSelectionEditor;

public:
    explicit SelectionEditorFrame(
        std::shared_ptr<vcl::SelectionEditor<ViewerType>> ptr,
        QWidget* parent = nullptr) : GenericEditorFrame(parent)
    {
        mSelectionEditor = ptr;

        auto& settings = mSelectionEditor->settings();

        QIcon        icv(":/icons/select_vertex.png");
        QPushButton* selectVerticesButton = Base::addButton(icv);
        selectVerticesButton->setToolTip("Select Vertices");

        QIcon        icf(":/icons/select_face.png");
        QPushButton* selectFacesButton = Base::addButton(icf);
        selectFacesButton->setToolTip("Select Faces");

        auto onSelectVerticesButtonClicked = [&](bool checked) {
            bool selFaces = settings.selectFaces;
            mSelectionEditor->setActive(checked || selFaces);
            settings.selectVertices = checked;
        };

        auto onSelectFacesButtonClicked = [&](bool checked) {
            bool selVertices = settings.selectVertices;
            mSelectionEditor->setActive(checked || selVertices);
            settings.selectFaces = checked;
        };

        connect(
            selectVerticesButton,
            &QPushButton::clicked,
            this,
            onSelectVerticesButtonClicked);

        connect(
            selectFacesButton,
            &QPushButton::clicked,
            this,
            onSelectFacesButtonClicked);

        SelectionEditorSettingsFrame* sf =
            Base::setSettingsFrame<SelectionEditorSettingsFrame>(settings);

        connect(sf, SIGNAL(settingsUpdated()), this, SLOT(refreshSettings()));
    }

private slots:

    // probably not needed here (no drawing to update when settings change)
    void refreshSettings() override
    {
        if (mSelectionEditor) {
            mSelectionEditor->refreshSettings();
        }
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::SelectionEditor, ViewerType>
{
    using FrameType = SelectionEditorFrame<ViewerType>;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H
