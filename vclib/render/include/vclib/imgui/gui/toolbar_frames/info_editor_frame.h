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

#include <vclib/space/core.h>

#include <imgui/imgui.h>

#include <memory>

namespace vcl::imgui {

template<typename ViewerType>
class InfoEditorFrameImgui : public EditorFrameImgui
{
    std::shared_ptr<vcl::InfoEditor<ViewerType>> mEditor;

public:
    explicit InfoEditorFrameImgui(
        std::shared_ptr<vcl::InfoEditor<ViewerType>> editor) : mEditor(editor)
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

        ImGui::SameLine(0, 2);
        if (ImGui::Button("v##InfoSettings")) {
            ImGui::OpenPopup("##InfoSettingsPopup");
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Info Tool Settings");

        if (ImGui::BeginPopup("##InfoSettingsPopup")) {
            drawInfoSettings();
            ImGui::EndPopup();
        }
    }

private:
    void drawInfoSettings()
    {
        vcl::InfoEditorSettings& sts = mEditor->settings();

        // Highlight width
        float thickness = sts.thickness;
        ImGui::Text("Highlight Width:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(130);
        if (ImGui::SliderFloat(
                "##InfoThickness", &thickness, 1.0f, 10.0f, "%.1f")) {
            sts.thickness = thickness;
            mEditor->refreshSettings();
        }

        // Highlight color
        ImGui::Text("Highlight Color:");
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##InfoColor",
            [&] {
                return sts.color;
            },
            [&](vcl::Color c) {
                sts.color = c;
                mEditor->refreshSettings();
            },
            ImGuiColorEditFlags_NoInputs);
        // Text Size
        int textSize = sts.textSize;
        ImGui::Text("Text Size:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(130);
        if (ImGui::SliderInt(
                "##InfoTextSize", &textSize, 5, 100)) {
            sts.textSize = textSize;
            mEditor->refreshSettings();
        }

        // Text Color
        ImGui::Text("Text Color:");
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##InfoTextColor",
            [&] {
                return sts.textColor;
            },
            [&](vcl::Color c) {
                sts.textColor = c;
                mEditor->refreshSettings();
            },
            ImGuiColorEditFlags_NoInputs);
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::InfoEditor, ViewerType>
{
    using FrameType = InfoEditorFrameImgui<ViewerType>;
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_GUI_TOOLBAR_FRAMES_INFO_EDITOR_FRAME_H
