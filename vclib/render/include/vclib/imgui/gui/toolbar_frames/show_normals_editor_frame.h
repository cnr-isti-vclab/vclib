// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_GUI_TOOLBAR_FRAMES_SHOW_NORMALS_EDITOR_FRAME_H
#define VCL_IMGUI_GUI_TOOLBAR_FRAMES_SHOW_NORMALS_EDITOR_FRAME_H

#include "../editor_frame.h"

#include <vclib/render/editors/show_normals_editor.h>
#include <vclib/space/core/color.h>

#include <imgui/imgui.h>

#include <memory>

namespace vcl::imgui {

/**
 * @brief Frame that allows the user to interact with a ShowNormalsEditor.
 *
 * @tparam ViewerType: the type of the viewer.
 */
template<typename ViewerType>
class ShowNormalsEditorFrameImgui : public EditorFrameImgui
{
    std::shared_ptr<vcl::ShowNormalsEditor<ViewerType>> mEditor;

public:
    explicit ShowNormalsEditorFrameImgui(
        std::shared_ptr<vcl::ShowNormalsEditor<ViewerType>> editor) :
            mEditor(editor)
    {
    }

    void draw() override
    {
        if (!mEditor)
            return;

        bool snActive = mEditor->isActive();
        if (ImGui::Button(snActive ? "[SN]" : " SN ")) {
            mEditor->setActive(!snActive);
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Show Normals");

        ImGui::SameLine(0, 2);
        if (ImGui::Button("v##SNSettings")) {
            ImGui::OpenPopup("##SNSettingsPopup");
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Show Normals Settings");

        if (ImGui::BeginPopup("##SNSettingsPopup")) {
            drawShowNormalsSettings();
            ImGui::EndPopup();
        }
    }

private:
    void drawShowNormalsSettings()
    {
        vcl::ShowNormalsEditorSettings& sts = mEditor->settings();

        // Edit mode
        static const char* editModeNames[] = {
            "None", "Selected Object", "Visible Objects", "All Objects"};
        int currentMode = vcl::toUnderlying(sts.editMode);
        ImGui::Text("Apply to:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        if (ImGui::BeginCombo("##SNEditMode", editModeNames[currentMode])) {
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

        ImGui::Separator();

        // Vertex Normals
        if (ImGui::Checkbox("Show Vertex Normals", &sts.showVertexNormals)) {
            mEditor->refreshSettings();
        }
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##SNVertexColor",
            [&] {
                return sts.vertexNormalColor;
            },
            [&](vcl::Color c) {
                sts.vertexNormalColor = c;
                mEditor->refreshSettings();
            },
            ImGuiColorEditFlags_NoInputs);

        // Face Normals
        if (ImGui::Checkbox("Show Face Normals", &sts.showFaceNormals)) {
            mEditor->refreshSettings();
        }
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##SNFaceColor",
            [&] {
                return sts.faceNormalColor;
            },
            [&](vcl::Color c) {
                sts.faceNormalColor = c;
                mEditor->refreshSettings();
            },
            ImGuiColorEditFlags_NoInputs);

        ImGui::Separator();

        // Length Ratio
        float lengthRatio = sts.normalLengthRatio;
        ImGui::Text("Length Ratio:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        if (ImGui::SliderFloat(
                "##SNLengthRatio", &lengthRatio, 0.0f, 1.0f, "%.2f")) {
            sts.normalLengthRatio = lengthRatio;
            mEditor->refresh();
        }

        // Lines width
        float thickness = sts.thickness;
        ImGui::Text("Lines Width:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        if (ImGui::SliderFloat(
                "##SNThickness", &thickness, 1.0f, 10.0f, "%.1f")) {
            sts.thickness = thickness;
            mEditor->refreshSettings();
        }

        ImGui::Separator();

        if (ImGui::Button("Reset Defaults")) {
            sts.resetDefaults();
            mEditor->refresh();
        }
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::ShowNormalsEditor, ViewerType>
{
    using ToolbarFrameType = ShowNormalsEditorFrameImgui<ViewerType>;
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_GUI_TOOLBAR_FRAMES_SHOW_NORMALS_EDITOR_FRAME_H
