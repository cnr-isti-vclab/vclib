// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/imgui/mesh_viewer_imgui_drawer.h>

#include "get_drawable_mesh.h"

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#include <vclib/render/automation/actions.h>
#include <vclib/render/automation/actions/automation_action_factory.h>
#include <vclib/render/automation/metrics.h>
#include <vclib/render/automation/printers.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/editors/benchmark_editor.h>

#include <vclib/glfw/window_manager.h>

int main(int argc, char** argv)
{
    using BenchmarkViewer = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::ViewerDrawer>;



#ifdef VCLIB_RENDER_BACKEND_BGFX
    vcl::Context::setResetFlags(BGFX_RESET_NONE);
#endif

    BenchmarkViewer tw("981 - Bimba Benchmark");

    auto be = tw.pushEditor<vcl::BenchmarkEditor>();

    using VDT = decltype(be)::element_type::ViewerDrawerType;
    using BenchmarkEditorT = vcl::BenchmarkEditor<VDT>;

    auto benchmarkEditor = std::dynamic_pointer_cast<BenchmarkEditorT>(be);


    // load and set up a drawable mesh (bimba.obj)
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>("bimba.obj");

    tw.pushDrawableObject(drawable);

    // An automation action factory
    vcl::AutomationActionFactory<BenchmarkEditorT> aaf;

    // Run the automation 1 time
    benchmarkEditor->setRepeatTimes(100);

    // Measure FPS
    benchmarkEditor->setMetric(vcl::FpsBenchmarkMetric());

    // Rotate 360 degrees (2*PI) around the Y axis over 3 seconds
    benchmarkEditor->addAutomation(aaf.createTimeLimited(
        aaf.createRotation(2.0f * M_PI, {0.f, 1.f, 0.f}),
        3.0f));

    // Print the results to stdout
    vcl::StdoutBenchmarkPrinter prntr;
    benchmarkEditor->setPrinter(vcl::StdoutBenchmarkPrinter(prntr));

    benchmarkEditor->setActive(true);

    tw.fitScene();

    tw.show();

    return 0;
}
