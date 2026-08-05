// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_GUI_TOOLBAR_FRAMES_INFO_EDITOR_FRAME_H
#define VCL_IMGUI_GUI_TOOLBAR_FRAMES_INFO_EDITOR_FRAME_H

#include "../editor_frame.h"

#include <vclib/render/editors/info_editor.h>

#include <imgui/imgui.h>

#include <memory>

namespace vcl::imgui {

template<typename ViewerType>
class InfoEditorFrameImgui : public EditorFrameImgui
{
    std::shared_ptr<vcl::InfoEditor<ViewerType>> mEditor;

public:
    explicit InfoEditorFrameImgui(
        std::shared_ptr<vcl::InfoEditor<ViewerType>> editor) :
            mEditor(editor)
    {
    }

    void draw() override
    {
        if (!mEditor)
            return;

        bool active = mEditor->isActive();
        if (ImGui::Button(active ? "[ I ]" : "  I  ")) {
            mEditor->setActive(!active);
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Info Tool");
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::InfoEditor, ViewerType>
{
    using FrameType = InfoEditorFrameImgui<ViewerType>;
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_GUI_TOOLBAR_FRAMES_INFO_EDITOR_FRAME_H
