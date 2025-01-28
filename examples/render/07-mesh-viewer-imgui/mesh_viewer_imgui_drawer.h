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

        // // points
        // ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        // if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        // {
        //     if (ImGui::BeginTabItem("Points"))
        //     {
        //         ImGui::BeginDisabled(!newSettings.canPointCloudBeVisible());
        //         ImGui::Checkbox("Visible",
        //             [&]{return newSettings.isPointCloudVisible();},
        //             [&](bool vis){newSettings.setPointCloudVisibility(vis);}
        //         );


        //         ImGui::EndDisabled();

        //         ImGui::EndTabItem();
        //     }
        //     if (ImGui::BeginTabItem("Broccoli"))
        //     {
        //         ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
        //         ImGui::EndTabItem();
        //     }
        //     if (ImGui::BeginTabItem("Cucumber"))
        //     {
        //         ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
        //         ImGui::EndTabItem();
        //     }
        //     ImGui::EndTabBar();
        // }
        ImGui::BeginDisabled(!newSettings.canPointCloudBeVisible());
        ImGui::Checkbox("Points",
            [&]{return newSettings.isPointCloudVisible();},
            [&](bool vis){newSettings.setPointWidth(3);
                newSettings.setPointCloudVisibility(vis);}
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