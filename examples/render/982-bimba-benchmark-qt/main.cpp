// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/render/automation/actions.h>
#include <vclib/render/automation/actions/automation_action_factory.h>
#include <vclib/render/automation/metrics.h>
#include <vclib/render/automation/printers.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/editors/benchmark_editor.h>

#include <vclib/qt/mesh_viewer.h>
#include <vclib/qt/utils/qapplication.h>

int main(int argc, char** argv)
{
    QApplication app = vcl::qt::qAppl(argc, argv);

#ifdef VCLIB_RENDER_BACKEND_BGFX
    vcl::Context::setResetFlags(BGFX_RESET_NONE);
#endif

    vcl::qt::MeshViewer mv;
    mv.setWindowTitle("982 - Bimba Benchmark Qt");

    auto be = mv.pushEditor<vcl::BenchmarkEditor>();

    using VDT = decltype(be)::element_type::ViewerDrawerType;
    using BenchmarkEditorT = vcl::BenchmarkEditor<VDT>;

    auto benchmarkEditor = std::dynamic_pointer_cast<BenchmarkEditorT>(be);


    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>("bimba.obj");

    auto vec = std::make_shared<vcl::DrawableObjectVector>();
    vec->pushBack(std::move(drawable));
    mv.setDrawableObjectVector(vec);

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

    mv.fitScene();

    mv.show();

    return app.exec();
}
