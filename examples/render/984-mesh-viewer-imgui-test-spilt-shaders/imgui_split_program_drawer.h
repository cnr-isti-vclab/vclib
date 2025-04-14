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

#ifndef IMGUI_SWITCH_PROGRAM_DRAWER_H
#define IMGUI_SWITCH_PROGRAM_DRAWER_H

#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <vclib/meshes.h>

#include <imgui.h>
#include <bgfx/bgfx.h>

#include <functional>

template<typename DerivedRenderApp>
class ImguiSplitProgramDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{
    bool useSplitProgram = false;

    using SurfaceProgramsType =
        typename vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType;
    using SurfaceProgramsTypeFunction =
        std::function<void(SurfaceProgramsType)>;

    SurfaceProgramsTypeFunction surfaceProgramChangerFn = nullptr;

public:
    using vcl::PlainDrawer<DerivedRenderApp>::PlainDrawer;

    ImguiSplitProgramDrawer() {};

    void onDrawContent(vcl::uint viewId) override
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Split programs checkbox", nullptr);
        ImGui::Checkbox("Use split programs", &useSplitProgram);
        ImGui::End();

        if (surfaceProgramChangerFn) {
            if (useSplitProgram) {
                surfaceProgramChangerFn(
                    vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType::
                        SPLITTED);
            }
            else {
                surfaceProgramChangerFn(
                    vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType::
                        UBER);
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
