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

        // point size
        ImGui::SliderFloat("Size",
            [&]{return settings.pointWidth();},
            [&](float v){settings.setPointWidth(v);},
            1.0f,
            10.0f);
        

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
                // TODO
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Wireframe")) // are we sure?
            {
                // TODO
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::SeparatorText("OLD");

        ImGui::BeginDisabled(!newSettings.canPointBeVisible());
        ImGui::Checkbox("Points",
            [&]{return newSettings.isPointVisible();},
            [&](bool vis){newSettings.setPointVisibility(vis);}
        );
        ImGui::EndDisabled();

        ImGui::SameLine();

        // edges
        ImGui::BeginDisabled(!newSettings.canEdgesBeVisible());
        ImGui::Checkbox("Edges",
            [&]{return newSettings.isWireframeVisible();},
            [&](bool vis){newSettings.setWireframeVisibility(vis);}
        );
        ImGui::EndDisabled();

        // surface
        ImGui::BeginDisabled(!newSettings.canSurfaceBeVisible());
        {
            ImGui::Checkbox("Surface",
                [&]{return newSettings.isSurfaceVisible();},
                [&](bool vis){newSettings.setSurfaceVisibility(vis);}
            );
            // surface shading
            
            ImGui::Text("Shading");
            ImGui::SameLine();
            // radio buttons
            // ImGui::RadioButton("None",
            //     [&]{return newSettings.isSurfaceShadingNone();},
            //     [&](bool vis){newSettings.setSurfaceShadingNone();}
            // );
            ImGui::RadioButton("Flat",
                [&]{return newSettings.isSurfaceShadingFlat();},
                [&](bool vis){if (vis) newSettings.setSurfaceShadingFlat();}
            );
            ImGui::SameLine();
            ImGui::RadioButton("Smooth",
                [&]{return newSettings.isSurfaceShadingSmooth();},
                [&](bool vis){if (vis) newSettings.setSurfaceShadingSmooth();}
            );
            ImGui::Checkbox("Wireframe",
                [&]{return newSettings.isWireframeVisible();},
                [&](bool vis){newSettings.setWireframeVisibility(vis);}
            );
        }

        ImGui::EndDisabled();

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