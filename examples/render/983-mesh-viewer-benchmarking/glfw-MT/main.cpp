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

#include <thread>

// Function that will be called by the separate thread
void toStdOutMTFun(std::shared_ptr<vcl::ObjectBenchmarkPrinterResult> res)
{
    // We want to block until all the automations are finished and we have all
    // the metrics at our disposal
    auto vec = res->getVectorBlocking();
    for (const auto& el : vec) {
        for (const auto& measurement : (el.second->getMeasureStrings())) {
            std::cout << measurement << ", ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv)
{
    using BenchmarkViewer = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::BenchmarkViewerDrawer>;

#ifdef VCLIB_RENDER_BACKEND_BGFX

    vcl::Context::setResetFlags(BGFX_RESET_NONE);

#endif

    BenchmarkViewer tw("Benchmark Viewer GLFW");

    // Create the ObjectBenchmarkPrinter instance
    vcl::ObjectBenchmarkPrinter obp;

    // Initialize the thread that will read the results
    std::thread thr(toStdOutMTFun, obp.getResultPtr());

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable =
        getDrawableMesh<vcl::TriMesh>("bunny.obj");

    // An automation action factory, to shorten the length of Automation
    // declarations
    vcl::AutomationActionFactory<vcl::BenchmarkDrawer<BenchmarkViewer>> aaf;

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    // Repeat all automations 2 times
    tw.setRepeatTimes(3);

    // Change the measured metric to FPS
    tw.setMetric(vcl::FpsBenchmarkMetric());

    // Rotate and scale at the same time for 2 seconds
    tw.addAutomation(aaf.createTimeLimited(
        aaf.createSimultaneous(
            {aaf.createRotation(5.f, {0.f, 0.f, 1.f}),
             aaf.createChangeScaleAbsolute(-0.2f)}),
        2.f));

    // Change the measured metric to time (seconds)
    tw.setMetric(vcl::TimeBenchmarkMetric());

    // Rotate for 5000 frames and then scale for 5000 frames
    tw.addAutomation(aaf.createSequential(
        {aaf.createFrameLimited(
             aaf.createPerFrameRotation(1.f/1000.f, {0.f, -1.f, 0.f}), 5000.f),
         aaf.createFrameLimited(aaf.createPerFrameChangeScaleAbsolute(1.f/5000.f), 5000.f)}));

    // Print the results to a vector
    tw.setPrinter(obp);

    tw.fitScene();

    tw.show();

    thr.join();

    return 0;
}
