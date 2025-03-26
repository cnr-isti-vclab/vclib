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

#include <vclib/imgui/mesh_viewer_imgui_drawer.h>

#include "get_drawable_mesh.h"

#include <vclib/imgui/imgui_drawer.h>

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#include <vclib/render/automation/actions.h>
#include <vclib/render/automation/printers.h>
#include <vclib/render/automation/metrics.h>
#include <vclib/render/drawers/benchmark_drawer.h>

#include <imgui.h>

int main(int argc, char** argv)
{
    using ImguiMeshViewer = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::imgui::MeshViewerDrawerImgui,
        vcl::BenchmarkDrawer>;

    vcl::Context::setResetFlags(BGFX_RESET_NONE);

    ImguiMeshViewer tw("ImGui Mesh Viewer GLFW");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>("bunny.obj");
    vcl::DrawableMesh<vcl::TriMesh> drawable2 = getDrawableMesh<vcl::TriMesh>();

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

   //Repeat all automations 2 times
   tw.setRepeatTimes(3);
    
   //Before starting we disable trackball events. We want this to be done only once
   tw.addAutomation(
       vcl::StartCountLimitedAutomationAction(
           vcl::TrackballEventIgnoreAutomationAction(&tw, true),
           1
       ),
       //We don't want to measure the metric for this automation
       false
    );

    //After the first loop, change the mesh
    tw.addAutomation(
        vcl::StartCountDelayAutomationAction(
            vcl::StartCountLimitedAutomationAction(
                vcl::MeshChangerAutomationAction(&tw, drawable2),
                1
            ),
            1
        ),
        //We don't want to measure the metric for this automation
        false
    );

    //Change the measured metric to FPS
    tw.addAutomation(
        vcl::MetricChangerAutomationAction<vcl::BenchmarkDrawer<ImguiMeshViewer>>(&tw, vcl::FpsBenchmarkMetric()),
        //We don't want to measure the metric for this automation
        false
    );

    //Rotate and scale at the same time for 2 seconds
    tw.addAutomation(
        vcl::TimeLimitedAutomationAction(
            vcl::SimultaneousAutomationActions{
                vcl::RotationAutomationAction(&tw, 5.f, {0.f,0.f,1.f}),
                vcl::ScaleAutomationAction(&tw, -0.01f)
            },
            2.f
        )
    );

    //Change the measured metric to time (seconds) 
    tw.addAutomation(
        vcl::MetricChangerAutomationAction<vcl::BenchmarkDrawer<ImguiMeshViewer>>(&tw, vcl::TimeBenchmarkMetric()),
        false
    );

    //Rotate for 5000 frames and then scale for 5000 frames
    tw.addAutomation(
        vcl::SequentialAutomationActions{
            vcl::FrameLimitedAutomationAction( vcl::RotationAutomationAction(&tw, 5.f, {0.f,-1.f,0.f}), 5000.f),
            vcl::FrameLimitedAutomationAction( vcl::ScaleAutomationAction(&tw, 0.02f), 5000.f)
        }
    );

    //When all the automations are finished, we reenable trackball events
    tw.addAutomation(
        vcl::StartCountDelayAutomationAction(
            vcl::TrackballEventIgnoreAutomationAction(&tw, false),
            2
        ),
        //We don't want to measure the metric for this automation
        false
    );

    //Print the results in a json file
    tw.setPrinter(vcl::JsonBenchmarkPrinter("./test_out.json"));

    tw.fitScene();

    tw.show();

    return 0;
}
