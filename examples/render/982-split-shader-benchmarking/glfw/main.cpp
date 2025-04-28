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

#include "change_shader_automation_action.h"
#include "get_drawable_mesh.h"
#include "glfw_maximized_window_manager.h"

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#include <vclib/render/automation/actions.h>
#include <vclib/render/automation/actions/automation_action_factory.h>
#include <vclib/render/automation/metrics.h>
#include <vclib/render/automation/printers.h>
#include <vclib/render/drawers/benchmark_drawer.h>
#include <vclib/render/drawers/benchmark_viewer_drawer.h>

#include <vclib/imgui/imgui_drawer.h>

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/glfw/window_manager.h>

#include <imgui.h>

int main(void)
{
    using BenchmarkViewer = vcl::RenderApp<
        vcl::glfw::MaximizedWindowManager,
        vcl::Canvas,
        vcl::BenchmarkViewerDrawer>;

    using BenchmarDrawerT = vcl::BenchmarkDrawer<BenchmarkViewer>;

#ifdef VCLIB_RENDER_BACKEND_BGFX

    vcl::Context::setResetFlags(BGFX_RESET_NONE);

#endif

    BenchmarkViewer tw("Benchmark Viewer GLFW");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable =
        getDrawableMesh<vcl::TriMesh>("bunny.obj");

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();

    tw.setDrawableObjectVector(vec);

    // An automation action factory, to shorten the length of Automation
    // declarations
    vcl::AutomationActionFactory<BenchmarDrawerT> aaf;

    vcl::ChangeShaderAutomationAction<BenchmarDrawerT> csaa(
        vec, vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType::SPLIT);
    vcl::ChangeShaderAutomationAction<BenchmarDrawerT> csaa2(
        vec,
        vcl::DrawableMesh<
            vcl::TriMesh>::SurfaceProgramsType::UBER_WITH_STATIC_IF);

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(std::move(drawable));

    // Repeat all automations 2 times
    tw.setRepeatTimes(120);

    tw.addAutomationNoMetric(aaf.createStartCountLimited(
        aaf.createTimeLimited(aaf.createChangeScaleMultiplicative(1.f), 1.f),
        1));

    // Change the measured metric to FPS
    tw.setMetric(vcl::FpsBenchmarkMetric());

    // Rotate and scale at the same time for 2 seconds
    tw.addAutomation(
        aaf.createTimeLimited(aaf.createRotation(5.f, {0.f, 0.f, 1.f}), 2.f));

    // Change the measured metric to time (seconds)
    tw.setMetric(vcl::TimeBenchmarkMetric());

    // Rotate for 5000 frames and then scale for 5000 frames
    tw.addAutomation(aaf.createFrameLimited(
        aaf.createPerFrameRotation(1e-1f, {0.f, -1.f, 0.f}), 1000));

    tw.setMetric(vcl::FpsBenchmarkMetric());

    tw.addAutomation(
        aaf.createTimeLimited(aaf.createRotation(5.f, {1.f, 0.f, 0.f}), 2.f));

    tw.addAutomation(
        aaf.createStartCountDelay(aaf.createStartCountLimited(csaa, 1), 40));
    tw.addAutomation(
        aaf.createStartCountDelay(aaf.createStartCountLimited(csaa2, 1), 80));

    // Print the results in a json file
    tw.setPrinter(vcl::JsonBenchmarkPrinter("./test_out.json"));

    tw.fitScene();

    tw.show();

    return 0;
}
