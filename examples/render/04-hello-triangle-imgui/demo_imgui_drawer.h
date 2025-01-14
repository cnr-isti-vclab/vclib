#ifndef DEMO_IMGUI_DRAWER_H
#define DEMO_IMGUI_DRAWER_H

#include <vclib/imgui/imgui_drawer.h>

#include <imgui.h>

template<typename DerivedRenderApp>
class DemoImGuiDrawer : public vcl::imgui::ImGuiDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::imgui::ImGuiDrawer<DerivedRenderApp>;

public:
    using ParentDrawer::ParentDrawer;

    virtual void onDraw(vcl::uint viewId) override
    {
        // draw the scene
        ParentDrawer::onDraw(viewId);

        if (!ParentDrawer::isWindowMinimized()) {
            // imgui demo window
            ImGui::ShowDemoWindow();
        }
    }
};

#endif // DEMO_IMGUI_DRAWER_H
