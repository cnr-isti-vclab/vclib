/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_IMGUI_MESH_VIEWER_IMGUI_DRAWER_H
#define VCL_IMGUI_MESH_VIEWER_IMGUI_DRAWER_H

#include "imgui_helpers.h"

#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawers/trackball_viewer_drawer.h>

#include <imgui.h>

#include <algorithm>
#include <iterator>
#include <numeric>

namespace vcl::imgui {

template<typename DerivedRenderApp>
class MeshViewerDrawerImgui :
        public vcl::TrackBallViewerDrawer<DerivedRenderApp>
{
    using Base = vcl::TrackBallViewerDrawer<DerivedRenderApp>;

    // selected mesh index
    int mMeshIndex = 0;

public:
    using Base::Base;

    virtual void onDraw(vcl::uint viewId) override
    {
        // draw parent
        Base::onDraw(viewId);

        // draw imgui
        ImGui::Begin("Meshes");

        // mesh table
        {
            ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_HorizontalScrollbar;
            ImGui::BeginChild(
                "##ListContainer",
                ImVec2(ImGui::GetContentRegionAvail().x, 260),
                ImGuiChildFlags_None,
                window_flags);
            drawMeshList();
            ImGui::EndChild();
        }

        // drawable mesh info and settings for selected mesh
        if (mMeshIndex >= 0 && mMeshIndex < Base::mDrawList->size()) {
            auto drawable =
                std::dynamic_pointer_cast<vcl::AbstractDrawableMesh>(
                    Base::mDrawList->at(mMeshIndex));
            if (drawable) {
                drawMeshSettings(*drawable);
            }
        }

        ImGui::End();
    }

    void onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        if (button == MouseButton::RIGHT) {
            this->readIdRequest(x, y, [&](uint id) {
                if (id == UINT_NULL)
                    return;

                mMeshIndex = id;
                std::cout << "Selected  ID: " << id << std::endl;
            });
        }

        Base::onMousePress(button, x, y, modifiers);
    }

private:
    void drawMeshList()
    {
        if (!Base::mDrawList || Base::mDrawList->empty()) {
            ImGui::Text("No objects loaded");
            return;
        }

        int                    meshId         = 0;
        static ImGuiTableFlags meshTableFlags = 0 | ImGuiTableFlags_BordersH;
        if (ImGui::BeginTable("meshtable", 2, meshTableFlags)) {
            ImGui::TableSetupColumn(
                "Visible", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            for (auto& d : *(Base::mDrawList)) {
                auto& drawable = *d;
                ImGui::TableNextRow();

                ImGui::PushID(meshId++);

                ImGui::TableSetColumnIndex(0);

                // visibility checkbox
                ImGui::Checkbox(
                    "##Visible",
                    [&] {
                        return drawable.isVisible();
                    },
                    [&](bool vis) {
                        drawable.setVisibility(vis);
                    });

                ImGui::TableSetColumnIndex(1);

                // row selection
                bool isSelected = (mMeshIndex == meshId - 1);
                if (ImGui::Selectable(
                        drawable.name().c_str(),
                        isSelected,
                        ImGuiSelectableFlags_SpanAllColumns)) {
                    mMeshIndex = meshId - 1;
                }
                // tooltip with info
                if (!drawable.info().empty() &&
                    ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", drawable.info().c_str());
                    ImGui::EndTooltip();
                }
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
    }

    static void drawMeshPointSettings(
        const vcl::AbstractDrawableMesh& drawable,
        vcl::MeshRenderSettings&         settings)
    {
        using enum vcl::MeshRenderInfo::Points;

        ImGui::BeginDisabled(!settings.canPoints(VISIBLE));

        // visibility
        ImGui::Checkbox(
            "Visible",
            [&] {
                return settings.isPoints(VISIBLE);
            },
            [&](bool vis) {
                settings.setPoints(VISIBLE, vis);
            });

        // shape
        ImGui::Text("Shape:");
        ImGui::SameLine();
        ImGui::BeginDisabled(true);
        ImGui::RadioButton(
            "Circle",
            [&] {
                return false;
            },
            [&](bool v) {
            });
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(false);
        ImGui::RadioButton(
            "Pixel",
            [&] {
                return true;
            },
            [&](bool v) {
            });
        ImGui::EndDisabled();

        // shading
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.canPoints(SHADING_VERT));
        ImGui::RadioButton(
            "Vertex",
            [&] {
                return settings.isPoints(SHADING_VERT);
            },
            [&](bool v) {
                if (v)
                    settings.setPoints(SHADING_VERT);
            });
        ImGui::SameLine();
        ImGui::EndDisabled();
        ImGui::RadioButton(
            "None",
            [&] {
                return settings.isPoints(SHADING_NONE);
            },
            [&](bool vis) {
                if (vis)
                    settings.setPoints(SHADING_NONE);
            });

        // color
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* pointColorNames[]           = {"Vertex", "Mesh", "User"};
        const std::array<bool, 3> colorSelected = {
            settings.isPoints(COLOR_VERTEX),
            settings.isPoints(COLOR_MESH),
            settings.isPoints(COLOR_USER)};

        assert(
            std::accumulate(
                std::begin(colorSelected), std::end(colorSelected), 0) == 1);
        int idx = std::distance(
            std::begin(colorSelected),
            std::find(
                std::begin(colorSelected), std::end(colorSelected), true));
        assert(idx >= 0 && idx < 3);

        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboPointColor", pointColorNames[idx])) {
            for (int n = 0; n < IM_ARRAYSIZE(pointColorNames); n++) {
                const bool selected = (n == idx);

                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canPoints(COLOR_VERTEX));
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPoints(COLOR_VERTEX);
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canPoints(COLOR_MESH));
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPoints(COLOR_MESH);
                    ImGui::EndDisabled();
                    break;
                case 2:
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPoints(COLOR_USER);
                    break;
                default: assert(false); break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isPoints(COLOR_USER));
        ImGui::ColorEdit4(
            "##PointColor",
            [&] {
                return settings.pointUserColor();
            },
            [&](vcl::Color c) {
                settings.setPointsUserColor(c);
            },
            ImGuiColorEditFlags_NoInputs);
        ImGui::EndDisabled();

        // point size
        ImGui::Text("Size:");
        // set the width of the window minus the width of the label
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-10);
        ImGui::SliderFloat(
            "##PointSize",
            [&] {
                return settings.pointWidth();
            },
            [&](float v) {
                settings.setPointsWidth(v);
            },
            1.0f,
            10.0f);

        ImGui::EndDisabled();
    }

    static void drawMeshSurfaceSettings(
        const vcl::AbstractDrawableMesh& drawable,
        vcl::MeshRenderSettings&         settings)
    {
        using enum vcl::MeshRenderInfo::Surface;

        ImGui::BeginDisabled(!settings.canSurface(VISIBLE));

        // visibility
        ImGui::Checkbox(
            "Visible",
            [&] {
                return settings.isSurface(VISIBLE);
            },
            [&](bool vis) {
                settings.setSurface(VISIBLE, vis);
            });

        // shading
        assert(
            (settings.isSurface(SHADING_SMOOTH) +
             settings.isSurface(SHADING_FLAT) +
             settings.isSurface(SHADING_NONE)) == 1);
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::RadioButton(
            "Smooth",
            [&] {
                return settings.isSurface(SHADING_SMOOTH);
            },
            [&](bool vis) {
                if (vis)
                    settings.setSurface(SHADING_SMOOTH);
            });
        ImGui::SameLine();
        ImGui::RadioButton(
            "Flat",
            [&] {
                return settings.isSurface(SHADING_FLAT);
            },
            [&](bool vis) {
                if (vis)
                    settings.setSurface(SHADING_FLAT);
            });
        ImGui::SameLine();
        ImGui::RadioButton(
            "None",
            [&] {
                return settings.isSurface(SHADING_NONE);
            },
            [&](bool vis) {
                if (vis)
                    settings.setSurface(SHADING_NONE);
            });

        // color
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* surfColorNames[] = {
            "Vertex", "Face", "Mesh", "PerVertexTex", "PerWedgeTex", "User"};
        const std::array<bool, 6> colorSelected = {
            settings.isSurface(COLOR_VERTEX),
            settings.isSurface(COLOR_FACE),
            settings.isSurface(COLOR_MESH),
            settings.isSurface(COLOR_VERTEX_TEX),
            settings.isSurface(COLOR_WEDGE_TEX),
            settings.isSurface(COLOR_USER)};
        assert(
            std::accumulate(
                std::begin(colorSelected), std::end(colorSelected), 0) == 1);
        int idx = std::distance(
            std::begin(colorSelected),
            std::find(
                std::begin(colorSelected), std::end(colorSelected), true));
        assert(idx >= 0 && idx < 6);
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboSurfColor", surfColorNames[idx])) {
            for (int n = 0; n < IM_ARRAYSIZE(surfColorNames); n++) {
                const bool selected = (n == idx);

                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canSurface(COLOR_VERTEX));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_VERTEX);
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canSurface(COLOR_FACE));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_FACE);
                    ImGui::EndDisabled();
                    break;
                case 2:
                    ImGui::BeginDisabled(!settings.canSurface(COLOR_MESH));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_MESH);
                    ImGui::EndDisabled();
                    break;
                case 3:
                    ImGui::BeginDisabled(
                        !settings.canSurface(COLOR_VERTEX_TEX));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_VERTEX_TEX);
                    ImGui::EndDisabled();
                    break;
                case 4:
                    ImGui::BeginDisabled(!settings.canSurface(COLOR_WEDGE_TEX));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_WEDGE_TEX);
                    ImGui::EndDisabled();
                    break;
                case 5:
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_USER);
                    break;
                default: assert(false); break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isSurface(COLOR_USER));
        ImGui::ColorEdit4(
            "##SurfUserColor",
            [&] {
                return settings.surfaceUserColor();
            },
            [&](vcl::Color c) {
                settings.setSurfaceUserColor(c);
            },
            ImGuiColorEditFlags_NoInputs);
        ImGui::EndDisabled();

        ImGui::EndDisabled();
    }

    static void drawMeshWireframeSettings(
        const vcl::AbstractDrawableMesh& drawable,
        vcl::MeshRenderSettings&         settings)
    {
        using enum vcl::MeshRenderInfo::Wireframe;

        ImGui::BeginDisabled(!settings.canWireframe(VISIBLE));

        // visibility
        ImGui::Checkbox(
            "Visible",
            [&] {
                return settings.isWireframe(VISIBLE);
            },
            [&](bool v) {
                settings.setWireframe(VISIBLE, v);
            });

        // shading
        assert(
            settings.isWireframe(SHADING_VERT) !=
            settings.isWireframe(SHADING_NONE));
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::RadioButton(
            "Vertex",
            [&] {
                return settings.isWireframe(SHADING_VERT);
            },
            [&](bool vis) {
                if (vis)
                    settings.setWireframe(SHADING_VERT);
            });
        ImGui::SameLine();
        ImGui::RadioButton(
            "None",
            [&] {
                return settings.isWireframe(SHADING_NONE);
            },
            [&](bool vis) {
                if (vis)
                    settings.setWireframe(SHADING_NONE);
            });

        // color
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char*               wireColorNames[] = {"Vertex", "Mesh", "User"};
        const std::array<bool, 3> colorSelected    = {
            settings.isWireframe(COLOR_VERTEX),
            settings.isWireframe(COLOR_MESH),
            settings.isWireframe(COLOR_USER)};
        assert(
            std::accumulate(
                std::begin(colorSelected), std::end(colorSelected), 0) == 1);
        int idx = std::distance(
            std::begin(colorSelected),
            std::find(
                std::begin(colorSelected), std::end(colorSelected), true));
        assert(idx >= 0 && idx < 3);
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboWireColor", wireColorNames[idx])) {
            for (int n = 0; n < IM_ARRAYSIZE(wireColorNames); n++) {
                const bool selected = (n == idx);

                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canWireframe(COLOR_VERTEX));
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframe(COLOR_VERTEX);
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canWireframe(COLOR_MESH));
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframe(COLOR_MESH);
                    ImGui::EndDisabled();
                    break;
                case 2:
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframe(COLOR_USER);
                    break;
                default: assert(false); break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isWireframe(COLOR_USER));
        ImGui::ColorEdit4(
            "##WireUserColor",
            [&] {
                return settings.wireframeUserColor();
            },
            [&](vcl::Color c) {
                settings.setWireframeUserColor(c);
            },
            ImGuiColorEditFlags_NoInputs);
        ImGui::EndDisabled();

        ImGui::EndDisabled();
    }

    static void drawMeshEdgeSettings(
        const vcl::AbstractDrawableMesh& drawable,
        vcl::MeshRenderSettings&         settings)
    {
        using enum vcl::MeshRenderInfo::Edges;

        ImGui::BeginDisabled(!settings.canEdges(VISIBLE));

        // visibility
        ImGui::Checkbox(
            "Visible",
            [&] {
                return settings.isEdges(VISIBLE);
            },
            [&](bool v) {
                settings.setEdges(VISIBLE, v);
            });

        // shading
        assert(
            (settings.isEdges(SHADING_SMOOTH) + settings.isEdges(SHADING_FLAT) +
             settings.isEdges(SHADING_NONE)) == 1);
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::RadioButton(
            "Smooth",
            [&] {
                return settings.isEdges(SHADING_SMOOTH);
            },
            [&](bool v) {
                if (v)
                    settings.setEdges(SHADING_SMOOTH);
            });
        ImGui::SameLine();
        ImGui::RadioButton(
            "Flat",
            [&] {
                return settings.isEdges(SHADING_FLAT);
            },
            [&](bool v) {
                if (v)
                    settings.setEdges(SHADING_FLAT);
            });
        ImGui::RadioButton(
            "None",
            [&] {
                return settings.isEdges(SHADING_NONE);
            },
            [&](bool vis) {
                if (vis)
                    settings.setEdges(SHADING_NONE);
            });

        // color
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* edgeColorNames[] = {"Vertex", "Edge", "Mesh", "User"};
        const std::array<bool, 4> colorSelected = {
            settings.isEdges(COLOR_VERTEX),
            settings.isEdges(COLOR_EDGE),
            settings.isEdges(COLOR_MESH),
            settings.isEdges(COLOR_USER)};
        assert(
            std::accumulate(
                std::begin(colorSelected), std::end(colorSelected), 0) == 1);
        int idx = std::distance(
            std::begin(colorSelected),
            std::find(
                std::begin(colorSelected), std::end(colorSelected), true));
        assert(idx >= 0 && idx < 4);
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboEdgeColor", edgeColorNames[idx])) {
            for (int n = 0; n < IM_ARRAYSIZE(edgeColorNames); n++) {
                const bool selected = (n == idx);

                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canEdges(COLOR_VERTEX));
                    if (ImGui::Selectable(edgeColorNames[n], selected))
                        settings.setEdges(COLOR_VERTEX);
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canEdges(COLOR_EDGE));
                    if (ImGui::Selectable(edgeColorNames[n], selected))
                        settings.setEdges(COLOR_EDGE);
                    ImGui::EndDisabled();
                    break;
                case 2:
                    ImGui::BeginDisabled(!settings.canEdges(COLOR_MESH));
                    if (ImGui::Selectable(edgeColorNames[n], selected))
                        settings.setEdges(COLOR_MESH);
                    ImGui::EndDisabled();
                    break;
                case 3:
                    if (ImGui::Selectable(edgeColorNames[n], selected))
                        settings.setEdges(COLOR_USER);
                    break;
                default: assert(false); break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            // color picker
            ImGui::SameLine();
            ImGui::BeginDisabled(!settings.isEdges(COLOR_USER));
            ImGui::ColorEdit4(
                "##EdgeUserColor",
                [&] {
                    return settings.edgesUserColor();
                },
                [&](vcl::Color c) {
                    settings.setEdgesUserColor(c);
                },
                ImGuiColorEditFlags_NoInputs);
            ImGui::EndCombo();
        }
        ImGui::EndDisabled();
    }

    static void drawMeshSettings(vcl::AbstractDrawableMesh& drawable)
    {
        using MRI = vcl::MeshRenderInfo;

        ImGui::Separator();
        // mesh settings
        const auto settings    = drawable.renderSettings();
        auto       newSettings = settings;

        // points
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
            // points
            if (newSettings.canPoints(MRI::Points::VISIBLE) &&
                ImGui::BeginTabItem("Points")) {
                drawMeshPointSettings(drawable, newSettings);
                ImGui::EndTabItem();
            }

            // surface + wireframe
            if (newSettings.canSurface(MRI::Surface::VISIBLE)) {
                if (ImGui::BeginTabItem("Surface")) {
                    drawMeshSurfaceSettings(drawable, newSettings);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Wireframe")) {
                    drawMeshWireframeSettings(drawable, newSettings);
                    ImGui::EndTabItem();
                }
            }

            // edges
            if (newSettings.canEdges(MRI::Edges::VISIBLE) &&
                ImGui::BeginTabItem("Edges")) {
                drawMeshEdgeSettings(drawable, newSettings);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        if (newSettings != settings) {
            drawable.setRenderSettings(newSettings);
        }
    }
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_MESH_VIEWER_IMGUI_DRAWER_H
