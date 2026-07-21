// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_GUI_TOOLBAR_FRAMES_BOUNDING_BOX_EDITOR_FRAME_H
#define VCL_IMGUI_GUI_TOOLBAR_FRAMES_BOUNDING_BOX_EDITOR_FRAME_H

#include "../editor_frame.h"

#include <vclib/render/editors/bounding_box_editor.h>
#include <vclib/space/core/color.h>

#include <imgui/imgui.h>

#include <memory>

namespace vcl::imgui {

template<typename ViewerType>
class BoundingBoxEditorFrameImgui : public EditorFrameImgui
{
    std::shared_ptr<vcl::BoundingBoxEditor<ViewerType>> mEditor;

public:
    explicit BoundingBoxEditorFrameImgui(
        std::shared_ptr<vcl::BoundingBoxEditor<ViewerType>> editor) :
            mEditor(editor)
    {
    }

    void draw() override
    {
        if (!mEditor)
            return;

        bool bbActive = mEditor->isActive();
        if (ImGui::Button(bbActive ? "[BB]" : " BB ")) {
            mEditor->setActive(!bbActive);
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Show Bounding Box");

        ImGui::SameLine(0, 2);
        if (ImGui::Button("v##BBSettings")) {
            ImGui::OpenPopup("##BBSettingsPopup");
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Bounding Box Settings");

        if (ImGui::BeginPopup("##BBSettingsPopup")) {
            drawBoundingBoxSettings();
            ImGui::EndPopup();
        }
    }

private:
    void drawBoundingBoxSettings()
    {
        vcl::EditorSettings& sts = mEditor->settings();

        // Edit mode
        static const char* editModeNames[] = {
            "None", "Selected Object", "Visible Objects", "All Objects"};
        int currentMode = vcl::toUnderlying(sts.editMode);
        ImGui::Text("Apply to:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(130);
        if (ImGui::BeginCombo("##BBEditMode", editModeNames[currentMode])) {
            for (int n = 0; n < IM_ARRAYSIZE(editModeNames); n++) {
                bool selected = (n == currentMode);
                if (ImGui::Selectable(editModeNames[n], selected)) {
                    sts.editMode =
                        static_cast<vcl::EditorSettings::EditMode>(n);
                    mEditor->refreshSettings();
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Lines width
        assert(sts.customSettings["thickness"].has_value());
        float thickness = std::any_cast<float>(sts.customSettings["thickness"]);
        ImGui::Text("Lines Width:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(130);
        if (ImGui::SliderFloat(
                "##BBThickness", &thickness, 1.0f, 10.0f, "%.1f")) {
            sts.customSettings["thickness"] = thickness;
            mEditor->refreshSettings();
        }

        // Lines color
        assert(sts.customSettings["color"].has_value());
        ImGui::Text("Lines Color:");
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##BBColor",
            [&] {
                return std::any_cast<vcl::Color>(sts.customSettings["color"]);
            },
            [&](vcl::Color c) {
                sts.customSettings["color"] = c;
                mEditor->refreshSettings();
            },
            ImGuiColorEditFlags_NoInputs);
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::BoundingBoxEditor, ViewerType>
{
    using FrameType = BoundingBoxEditorFrameImgui<ViewerType>;
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_GUI_TOOLBAR_FRAMES_BOUNDING_BOX_EDITOR_FRAME_H
