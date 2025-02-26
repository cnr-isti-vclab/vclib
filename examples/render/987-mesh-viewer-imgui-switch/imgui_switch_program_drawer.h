#ifndef IMGUI_SWITCH_PROGRAM_DRAWER_H
#define IMGUI_SWITCH_PROGRAM_DRAWER_H

#include <imgui.h>
#include <vclib/render/drawers/plain_drawer.h>

#include <bgfx/bgfx.h>

template<typename DerivedRenderApp>
class ImguiSwitchProgramDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{

    public:
        using vcl::PlainDrawer<DerivedRenderApp>::PlainDrawer;

        static inline bool *useSwitchProgramGlobal;

        ImguiSwitchProgramDrawer(){};

        void onDrawContent(vcl::uint viewId) override {
            ImGuiIO &io = ImGui::GetIO();
            ImGui::Begin("Switch program checkbox",nullptr);
            ImGui::Checkbox("Use switch program", useSwitchProgramGlobal);
            ImGui::End();
        };

        void onResize(vcl::uint width, vcl::uint height) override {
            bgfx::reset(width, height, BGFX_RESET_NONE);
        };
};

#endif