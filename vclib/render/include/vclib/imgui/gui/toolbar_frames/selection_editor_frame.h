// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H
#define VCL_IMGUI_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H

#include "../editor_frame.h"

#include <vclib/render/editors/selection_editor.h>

#include <imgui/imgui.h>

#include <memory>

namespace vcl::imgui {

template<typename ViewerType>
class SelectionEditorFrameImgui : public EditorFrameImgui
{
    std::shared_ptr<vcl::SelectionEditor<ViewerType>> mEditor;

public:
    explicit SelectionEditorFrameImgui(
        std::shared_ptr<vcl::SelectionEditor<ViewerType>> editor) :
            mEditor(editor)
    {
    }

    void draw() override
    {
        if (!mEditor)
            return;

        vcl::EditorSettings& selSettings = mEditor->settings();

        bool vSel =
            std::any_cast<bool>(selSettings.customSettings["selectVertices"]);
        bool fSel =
            std::any_cast<bool>(selSettings.customSettings["selectFaces"]);

        if (ImGui::Button(vSel ? "[V Sel]" : " V Sel ")) {
            vSel                                         = !vSel;
            selSettings.customSettings["selectVertices"] = vSel;
            mEditor->setActive(vSel || fSel);
            mEditor->refreshSettings();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Vertex Selection");

        ImGui::SameLine(0, 2);
        if (ImGui::Button(fSel ? "[F Sel]" : " F Sel ")) {
            fSel                                      = !fSel;
            selSettings.customSettings["selectFaces"] = fSel;
            mEditor->setActive(vSel || fSel);
            mEditor->refreshSettings();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Face Selection");

        // small settings popup button
        ImGui::SameLine(0, 2);
        if (ImGui::Button("v##SelSettings")) {
            ImGui::OpenPopup("##SelSettingsPopup");
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Selection Settings");

        if (ImGui::BeginPopup("##SelSettingsPopup")) {
            drawSelectionSettings();
            ImGui::EndPopup();
        }
    }

private:
    void drawSelectionSettings()
    {
        vcl::EditorSettings& sts = mEditor->settings();

        // Edit mode
        static const char* editModeNames[] = {
            "None", "Selected Object", "Visible Objects", "All Objects"};
        int currentMode = vcl::toUnderlying(sts.editMode);
        ImGui::Text("Apply to:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(130);
        if (ImGui::BeginCombo("##SelEditMode", editModeNames[currentMode])) {
            for (int n = 0; n < IM_ARRAYSIZE(editModeNames); n++) {
                bool selected = (n == currentMode);
                if (n == 0 || n == 3)
                    ImGui::BeginDisabled();
                if (ImGui::Selectable(editModeNames[n], selected)) {
                    sts.editMode =
                        static_cast<vcl::EditorSettings::EditMode>(n);
                    mEditor->refreshSettings();
                }
                if (n == 0 || n == 3)
                    ImGui::EndDisabled();
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Only visible checkbox
        assert(sts.customSettings["onlyVisible"].has_value());
        bool onlyVisible =
            std::any_cast<bool>(sts.customSettings["onlyVisible"]);
        ImGui::Checkbox(
            "Only Visible Faces",
            [&] {
                return onlyVisible;
            },
            [&](bool v) {
                sts.customSettings["onlyVisible"] = v;
                mEditor->refreshSettings();
            });
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::SelectionEditor, ViewerType>
{
    using FrameType = SelectionEditorFrameImgui<ViewerType>;
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_GUI_TOOLBAR_FRAMES_SELECTION_EDITOR_FRAME_H
