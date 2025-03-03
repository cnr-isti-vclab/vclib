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

#include <imgui.h>
#include <vclib/render/drawers/plain_drawer.h>

#include <bgfx/bgfx.h>

#include "globals.h"

template<typename DerivedRenderApp>
class ImguiSplitProgramDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{

    public:
        using vcl::PlainDrawer<DerivedRenderApp>::PlainDrawer;

        static inline bool *useSwitchProgram;

        ImguiSplitProgramDrawer(){};

        void onDrawContent(vcl::uint viewId) override {
            ImGuiIO &io = ImGui::GetIO();
            ImGui::Begin("Split programs checkbox",nullptr);
            ImGui::Checkbox("Use split programs", &Globals::useSplitShading);
            ImGui::Checkbox("Use separated face color", &Globals::useSplitFaceColor);
            ImGui::End();
        };

        void onResize(vcl::uint width, vcl::uint height) override {
            bgfx::reset(width, height, BGFX_RESET_NONE);
        };
};

#endif