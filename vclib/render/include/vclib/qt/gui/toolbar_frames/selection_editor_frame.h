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

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H

#include "generic_editor_frame.h"
#include "settings/selection_editor_settings_frame.h"

#include <vclib/render/editors/selection_editor.h>

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

        assert(settings.customSettings.at("selectVertices").has_value());
        assert(settings.customSettings.at("selectFaces").has_value());

        QIcon        icv(":/icons/select_vertex.png");
        QPushButton* selectVerticesButton = Base::addButton(icv);
        selectVerticesButton->setToolTip("Select Vertices");

        QIcon        icf(":/icons/select_face.png");
        QPushButton* selectFacesButton = Base::addButton(icf);
        selectFacesButton->setToolTip("Select Faces");

        auto onSelectVerticesButtonClicked = [&](bool checked) {
            bool selFaces =
                std::any_cast<bool>(settings.customSettings["selectFaces"]);
            mSelectionEditor->setActive(checked || selFaces);
            settings.customSettings["selectVertices"] = checked;
        };

        auto onSelectFacesButtonClicked = [&](bool checked) {
            bool selVertices =
                std::any_cast<bool>(settings.customSettings["selectVertices"]);
            mSelectionEditor->setActive(checked || selVertices);
            settings.customSettings["selectFaces"] = checked;
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

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H
