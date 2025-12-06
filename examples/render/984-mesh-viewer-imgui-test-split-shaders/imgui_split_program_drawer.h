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

#ifndef IMGUI_SPLIT_PROGRAM_DRAWER_H
#define IMGUI_SPLIT_PROGRAM_DRAWER_H

#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawers/plain_drawer.h>

#include <vclib/meshes.h>

#include <bgfx/bgfx.h>
#include <imgui.h>

#include <functional>

template<typename DerivedRenderApp>
class ImguiSplitProgramDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{
    using SurfaceProgramsType =
        typename vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType;
    using SurfaceProgramsTypeFunction =
        std::function<void(SurfaceProgramsType)>;

    SurfaceProgramsTypeFunction surfaceProgramChangerFn = nullptr;

    using Base = vcl::PlainDrawer<DerivedRenderApp>;

public:
    using Base::Base;

    ImguiSplitProgramDrawer() {};

    void onDraw(vcl::uint viewId) override
    {
        Base::onDraw(viewId);

        ImGui::SetNextWindowSize(ImVec2(200.0f, 0.0f), ImGuiCond_Once);
        ImGui::Begin("Split programs checkbox", nullptr);

        const char* items[] = {
            "Uber Shader", "Split Shaders", "Uber Shader Static if"};
        static const char* current_item = items[0];

        static vcl::uint currNum = 0;

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::BeginCombo("##combo", current_item)) {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                bool is_selected = (current_item == items[n]);
                if (ImGui::Selectable(items[n], is_selected)) {
                    current_item = items[n];
                    currNum      = n;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::End();

        if (surfaceProgramChangerFn) {
            switch (currNum) {
            case 1:
                surfaceProgramChangerFn(
                    vcl::DrawableMesh<
                        vcl::TriMesh>::SurfaceProgramsType::SPLIT);
                break;
            case 2:
                surfaceProgramChangerFn(
                    vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType::
                        UBER_WITH_STATIC_IF);
                break;
            case 0:
            default:
                surfaceProgramChangerFn(
                    vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType::UBER);
            }
        }
    };

    void onResize(vcl::uint width, vcl::uint height) override
    {
        bgfx::reset(width, height, BGFX_RESET_NONE);
    };

    void setSurfaceProgramChangerFn(SurfaceProgramsTypeFunction fn)
    {
        surfaceProgramChangerFn = fn;
    }
};

#endif
