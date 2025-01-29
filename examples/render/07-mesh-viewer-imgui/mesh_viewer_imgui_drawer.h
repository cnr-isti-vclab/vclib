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

#ifndef VCL_RENDER_DRAWERS_MESH_VIEWER_DRAWER_IMGUI_H
#define VCL_RENDER_DRAWERS_MESH_VIEWER_DRAWER_IMGUI_H

#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <imgui.h>
#include "imgui_helpers.h"

#include <numeric>
#include <algorithm>
#include <iterator>

template<typename DerivedRenderApp>
class MeshViewerDrawerImgui : public vcl::ViewerDrawer<DerivedRenderApp>
{
    using Base = vcl::ViewerDrawer<DerivedRenderApp>;

    int mSelectIndex = 0;

    void drawMeshList()
    {
        if (!Base::mDrawList || Base::mDrawList->empty()) {
            ImGui::Text("No objects loaded");
            return;
        }

        int meshId = 0;
        static ImGuiTableFlags meshTableFlags = 0
            | ImGuiTableFlags_BordersH;
        if (ImGui::BeginTable("meshtable", 2, meshTableFlags))
        {
            ImGui::TableSetupColumn("Visible", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            for (auto & d : *(Base::mDrawList))
            {
                auto & drawable = *d;
                ImGui::TableNextRow();

                ImGui::PushID(meshId++);
                
                ImGui::TableSetColumnIndex(0);

                // visibility checkbox
                ImGui::Checkbox("##Visible",
                [&]{return drawable.isVisible();},
                [&](bool vis){drawable.setVisibility(vis);});

                ImGui::TableSetColumnIndex(1);

                // row selection
                bool isSelected = (mSelectIndex == meshId - 1);
                if (ImGui::Selectable(drawable.name().c_str(), isSelected,
                    ImGuiSelectableFlags_SpanAllColumns))
                {
                    mSelectIndex = meshId - 1;
                }
                // tooltip with info
                if (!drawable.info().empty() &&
                    ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
                {
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
        const vcl::DrawableMesh<vcl::TriMesh>& drawable,
        vcl::MeshRenderSettings& settings)
    {
        ImGui::BeginDisabled(!settings.canPointBeVisible());

        // visibility
        ImGui::Checkbox("Visible",
            [&]{return settings.isPointVisible();},
            [&](bool vis){settings.setPointVisibility(vis);}
        );

        // shape
        ImGui::Text("Shape:");
        ImGui::SameLine();
        // TODO: implement point shape enum? in render settings
        ImGui::BeginDisabled(true);
        ImGui::RadioButton("Circle",
            [&]{return false;},
            [&](bool v){}
        );
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(false);
        ImGui::RadioButton("Pixel",
            [&]{return true;},
            [&](bool v){}
        );
        ImGui::EndDisabled();

        // shading
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.canPointShadingBePerVertex());
        ImGui::RadioButton("Vertex",
            [&]{return settings.isPointShadingPerVertex();},
            [&](bool v){if (v) settings.setPointShadingPerVertex();}
        );
        ImGui::SameLine();
        ImGui::EndDisabled();
        ImGui::RadioButton("None",
            [&]{return settings.isPointShadingNone();},
            [&](bool vis){if (vis) settings.setPointShadingNone();}
        );

        // color
        // TODO: implement color enum? in render settings
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* pointColorNames[] = { "Vertex", "Mesh", "User" };
        const std::array<bool,3> colorSelected = {
            settings.isPointColorPerVertex(),
            settings.isPointColorPerMesh(),
            settings.isPointColorUserDefined()
        };

        assert(std::accumulate(
            std::begin(colorSelected),
            std::end(colorSelected), 0) == 1);
        int idx = std::distance( std::begin(colorSelected),
            std::find(std::begin(colorSelected),
                        std::end(colorSelected),
                        true));
        assert(idx >= 0 && idx < 3);
        
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboPointColor",
            pointColorNames[idx]))
        {
            for (int n = 0; n < IM_ARRAYSIZE(pointColorNames); n++)
            {
                const bool selected = (n == idx);
            
                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canPointColorBePerVertex());
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPointColorPerVertex();
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canPointColorBePerMesh());
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPointColorPerMesh();
                    ImGui::EndDisabled();
                    break;
                case 2:
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPointColorUserDefined();
                    break;
                default:
                    assert(false);
                    break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isPointColorUserDefined());
        ImGui::ColorEdit4("##PointColor",
            [&]{return settings.pointUserColor();},
            [&](vcl::Color c){settings.setPointUserColor(c);},
            ImGuiColorEditFlags_NoInputs
        );
        ImGui::EndDisabled();

        // point size
        ImGui::Text("Size:");
        // set the width of the window minus the width of the label
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-10);
        ImGui::SliderFloat("##PointSize",
            [&]{return settings.pointWidth();},
            [&](float v){settings.setPointWidth(v);},
            1.0f,
            10.0f);

        ImGui::EndDisabled();
    }

    static void drawMeshSurfaceSettings(
        const vcl::DrawableMesh<vcl::TriMesh>& drawable,
        vcl::MeshRenderSettings& settings)
    {
        ImGui::BeginDisabled(!settings.canSurfaceBeVisible());

        // visibility
        ImGui::Checkbox("Visible",
            [&]{return settings.isSurfaceVisible();},
            [&](bool vis){settings.setSurfaceVisibility(vis);}
        );

        // shading
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::RadioButton("Smooth",
            [&]{return settings.isSurfaceShadingSmooth();},
            [&](bool vis){if (vis) settings.setSurfaceShadingSmooth();}
        );
        ImGui::SameLine();
        ImGui::RadioButton("Flat",
            [&]{return settings.isSurfaceShadingFlat();},
            [&](bool vis){if (vis) settings.setSurfaceShadingFlat();}
        );
        ImGui::SameLine();
        ImGui::RadioButton("None",
            [&]{return settings.isSurfaceShadingNone();},
            [&](bool vis){if (vis) settings.setSurfaceShadingNone();}
        );

        // color
        // TODO: implement color enum? in render settings
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* surfColorNames[] = {
            "Vertex", "Face", "Mesh", "PerVertexTex", "PerWedgeTex", "User" };
        const std::array<bool,6> colorSelected = {
            settings.isSurfaceColorPerVertex(),
            settings.isSurfaceColorPerFace(),
            settings.isSurfaceColorPerMesh(),
            settings.isSurfaceColorPerVertexTexcoords(),
            settings.isSurfaceColorPerWedgeTexcoords(),
            settings.isSurfaceColorUserDefined()
        };
        assert(std::accumulate(
            std::begin(colorSelected),
            std::end(colorSelected), 0) == 1);
        int idx = std::distance( std::begin(colorSelected),
            std::find(std::begin(colorSelected),
                        std::end(colorSelected),
                        true));
        assert(idx >= 0 && idx < 6);
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboSurfColor",
            surfColorNames[idx]))
        {
            for (int n = 0; n < IM_ARRAYSIZE(surfColorNames); n++)
            {
                const bool selected = (n == idx);
            
                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canSurfaceColorBePerVertex());
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurfaceColorPerVertex();      
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canSurfaceColorBePerFace());
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurfaceColorPerFace();
                    ImGui::EndDisabled();
                    break;
                case 2:
                    ImGui::BeginDisabled(!settings.canSurfaceColorBePerMesh());
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurfaceColorPerMesh();
                    ImGui::EndDisabled();
                    break;
                case 3:
                    ImGui::BeginDisabled(
                        !settings.canSurfaceColorBePerVertexTexcoords());
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurfaceColorPerVertexTexcoords();
                    ImGui::EndDisabled();
                    break;
                case 4:
                    ImGui::BeginDisabled(
                        !settings.canSurfaceColorBePerWedgeTexcoords());
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurfaceColorPerWedgeTexcoords();
                    ImGui::EndDisabled();
                    break;
                case 5:
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurfaceColorUserDefined();
                    break;
                default:
                    assert(false);
                    break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isSurfaceColorUserDefined());
        ImGui::ColorEdit4("##SurfUserColor",
            [&]{return settings.surfaceUserColor();},
            [&](vcl::Color c){settings.setSurfaceUserColor(c);},
            ImGuiColorEditFlags_NoInputs
        );
        ImGui::EndDisabled();


        ImGui::EndDisabled();
    }

    static void drawMeshWireframeSettings(
        const vcl::DrawableMesh<vcl::TriMesh>& drawable,
        vcl::MeshRenderSettings& settings)
    {
        ImGui::BeginDisabled(!settings.canSurfaceBeVisible());

        // visibility
        ImGui::Checkbox("Visible",
            [&]{return settings.isWireframeVisible();},
            [&](bool v){settings.setWireframeVisibility(v);}
        );

        // shading
        assert(settings.isWireframeShadingPerVertex() != 
            settings.isWireframeShadingNone());
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::RadioButton("Vertex",
            [&]{return settings.isWireframeShadingPerVertex();},
            [&](bool vis){if (vis) settings.setWireframeShadingPerVertex();}
        );
        ImGui::SameLine();
        ImGui::RadioButton("None",
            [&]{return settings.isWireframeShadingNone();},
            [&](bool vis){if (vis) settings.setWireframeShadingNone();}
        );

        // color
        // TODO: implement color enum? in render settings
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* wireColorNames[] = { "Vertex", "Mesh", "User" };
        const std::array<bool,3> colorSelected = {
            settings.isWireframeColorPerVertex(),
            settings.isWireframeColorPerMesh(),
            settings.isWireframeColorUserDefined()
        };
        assert(std::accumulate(
            std::begin(colorSelected),
            std::end(colorSelected), 0) == 1);
        int idx = std::distance( std::begin(colorSelected),
            std::find(std::begin(colorSelected),
                        std::end(colorSelected),
                        true));
        assert(idx >= 0 && idx < 3);
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboWireColor",
            wireColorNames[idx]))
        {
            for (int n = 0; n < IM_ARRAYSIZE(wireColorNames); n++)
            {
                const bool selected = (n == idx);
            
                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canWireframeColorBePerVertex());
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframeColorPerVertex();
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canWireframeColorBePerMesh());
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframeColorPerMesh();
                    ImGui::EndDisabled();
                    break;
                case 2:
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframeColorUserDefined();
                    break;
                default:
                    assert(false);
                    break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isWireframeColorUserDefined());
        ImGui::ColorEdit4("##WireUserColor",
            [&]{return settings.wireframeUserColor();},
            [&](vcl::Color c){settings.setWireframeUserColor(c);},
            ImGuiColorEditFlags_NoInputs
        );
        ImGui::EndDisabled();

        ImGui::EndDisabled();
    }

    static void drawMeshSettings(vcl::DrawableMesh<vcl::TriMesh>& drawable)
    {
        ImGui::Separator();
        ImGui::Text("Vertices: %d", drawable.vertexNumber());
        ImGui::Text("Faces: %d", drawable.faceNumber());
        
        ImGui::Separator();
        // mesh settings
        ImGui::Text("Mesh Settings");
        const auto settings = drawable.renderSettings();
        auto newSettings = settings;

        // points
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            // points
            if (ImGui::BeginTabItem("Points"))
            {
                drawMeshPointSettings(drawable, newSettings);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Surface"))
            {
                drawMeshSurfaceSettings(drawable, newSettings);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Wireframe"))
            {
                drawMeshWireframeSettings(drawable, newSettings);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        if (newSettings != settings)
        {
            drawable.setRenderSettings(newSettings);
        }
    }

public:
    using Base::Base;

    virtual void onDraw(vcl::uint viewId) override
    {
        // draw parent
        Base::onDraw(viewId);

        // draw imgui
        ImGui::Begin("Meshes");
        
        // mesh table
        drawMeshList();



        // drawable mesh info and settings for selected mesh
        if (mSelectIndex >= 0 && mSelectIndex < Base::mDrawList->size())
        {
            auto drawable = std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::TriMesh>>(Base::mDrawList->at(mSelectIndex));
            if (drawable)
            {
                drawMeshSettings(*drawable);
            }
        }

        ImGui::ShowDemoWindow();
        ImGui::End();
    }
};

#endif // VCL_RENDER_DRAWERS_MESH_VIEWER_DRAWER_IMGUI_H