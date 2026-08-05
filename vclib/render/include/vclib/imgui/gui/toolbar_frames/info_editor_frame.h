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
        vcl::EditorSettings& sts = mEditor->settings();

        // Lines width
        assert(sts.customSettings.count("thickness"));
        float thickness = std::any_cast<float>(sts.customSettings["thickness"]);
        ImGui::Text("Lines Width:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(130);
        if (ImGui::SliderFloat(
                "##InfoThickness", &thickness, 1.0f, 10.0f, "%.1f")) {
            sts.customSettings["thickness"] = thickness;
            mEditor->refreshSettings();
        }

        // Lines color
        assert(sts.customSettings.count("color"));
        ImGui::Text("Lines Color:");
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##InfoColor",
            [&] {
                return std::any_cast<vcl::Color>(sts.customSettings["color"]);
            },
            [&](vcl::Color c) {
                sts.customSettings["color"] = c;
                mEditor->refreshSettings();
            },
            ImGuiColorEditFlags_NoInputs);
        // Text Size
        assert(sts.customSettings.count("text_size"));
        int textSize = std::any_cast<int>(sts.customSettings["text_size"]);
        ImGui::Text("Text Size:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(130);
        if (ImGui::SliderInt(
                "##InfoTextSize", &textSize, 5, 100)) {
            sts.customSettings["text_size"] = textSize;
            mEditor->refreshSettings();
        }

        // Text Color
        assert(sts.customSettings.count("text_color"));
        ImGui::Text("Text Color:");
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##InfoTextColor",
            [&] {
                return std::any_cast<vcl::Color>(sts.customSettings["text_color"]);
            },
            [&](vcl::Color c) {
                sts.customSettings["text_color"] = c;
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
