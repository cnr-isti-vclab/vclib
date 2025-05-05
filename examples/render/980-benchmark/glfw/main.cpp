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

#include <vclib/imgui/imgui_drawer.h>

#include "CsvBenchmarkPrinterNoDescription.h"
#include "change_shader_automation_action.h"
#include "cmd_opt_parser.h"
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

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/glfw/window_manager.h>

#include <imgui.h>

#include <regex>

#if defined(WIN32) || defined(_WIN32) || \
    defined(__WIN32) && !defined(__CYGWIN__)
static const std::string PATH_SEP = "\\";
#else
static const std::string PATH_SEP = "/";
#endif

int main(int argc, char** argv)
{
    using BenchmarkViewer = vcl::RenderApp<
        vcl::glfw::MaximizedWindowManager,
        vcl::Canvas,
        vcl::BenchmarkViewerDrawer>;

    using BenchmarDrawerT = vcl::BenchmarkDrawer<BenchmarkViewer>;

    using enum vcl::MeshRenderInfo::Buffers;
#ifdef VCLIB_RENDER_BACKEND_BGFX

    vcl::Context::setResetFlags(BGFX_RESET_NONE);

#endif

    CmdOptionParser optionParser = CmdOptionParser {
        {"--stdout",        0},
        {"-o",              3},
        {"--output-folder", 1},
        {"-f",              1},
        {"-r",              1},
        {"-h",              0},
        {"--help",          0},
        {"--no-print",      0},
        {"--flat",          0}
    };
    auto                     res     = optionParser.parseOptions(argc, argv);
    auto                     options = res.first;
    std::vector<std::string> remainingArgs = res.second;
    // 17
    if (options.contains("-h") || options.contains("--help")) {
        std::string argv0    = std::string(argv[0]);
        size_t      slashPos = std::string(argv0).rfind(PATH_SEP);
        std::string programName =
            slashPos == std::string::npos ? argv0 : argv0.substr(slashPos + 1);
        std::cout
            << "Executes a benchmark which comprises 3 rotations (one around "
               "each axis) for every SurfaceProgramType:"
            << "\nusage: " << programName << " [options] model1 model2 ..."
            << "\noptions:"
            << "\n\t--stdout:        " << "Prints results to standard output"
            << "\n\t-o:              "
            << "Takes 3 filenames as parameters; it writes the results "
               "in those files"
            << "\n\t--output-folder: "
            << "Takes a path as an argument. Writes the "
               "results in FOLDER/uber_result.csv, FOLDER/split_result.csv, "
               "FOLDER/uber_if_result.csv"
            << "\n\t--no-print:      " << "Disables result printing"
            << "\n\t-f:              "
            << "Allows you to choose how many frames the rotations last"
            << "\n\t-r:              "
            << "Allows you to choose how many times the 3 rotations "
               "are executed for each SurfaceProgramType"
            << "\n\t--flat:          " << "Uses flat shading for all the meshes"
            << "\n\t-h, --help:      " << "Shows help page\n";
        exit(0);
    }

    if (remainingArgs.size() == 0) {
        std::cerr << "Error: missing model argument(s)\n";
        exit(1);
    }

    vcl::uint frames = 1000;

    if (options.contains("-f")) {
        frames = vcl::uint(std::strtoul(options["-f"][0].c_str(), nullptr, 10));
    }

    vcl::uint repetitionsPerProgramType = 2;

    if (options.contains("-r")) {
        repetitionsPerProgramType =
            vcl::uint(std::strtoul(options["-r"][0].c_str(), nullptr, 10));
    }

    BenchmarkViewer tw("Benchmark");

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();

    vcl::Box3d bb;
    for (const auto& path : remainingArgs) {
        vcl::DrawableMesh<vcl::TriMesh> msh =
            getDrawableMesh<vcl::TriMesh>(path, false);
        if (!bb.isNull()) {
            vcl::translate(msh, vcl::Point3d(bb.size().x(), 0, 0));
        }
        bb.add(vcl::boundingBox(msh));
        msh.updateBuffers({VERTICES, VERT_NORMALS});
        if (options.contains("--flat")) {
            vcl::MeshRenderSettings mrs = msh.renderSettings();
            mrs.setSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT);
            msh.setRenderSettings(mrs);
        }
        vec->pushBack(std::move(msh));
    }

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

    // Repeat all automations 2 times
    tw.setRepeatTimes(repetitionsPerProgramType * 3);

    tw.setMetric(vcl::FpsBenchmarkMetric());

    // Rotate and scale at the same time for 2 seconds
    tw.addAutomation(aaf.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<
            BenchmarDrawerT>::fromFramesPerRotation(frames, {0.f, 0.f, 1.f}),
        frames));

    // Rotate for 5000 frames and then scale for 5000 frames
    tw.addAutomation(aaf.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<
            BenchmarDrawerT>::fromFramesPerRotation(frames, {0.f, 1.f, 0.f}),
        frames));

    tw.addAutomation(aaf.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<
            BenchmarDrawerT>::fromFramesPerRotation(frames, {1.f, 0.f, 0.f}),
        frames));

    tw.addAutomationNoMetric(aaf.createStartCountDelay(
        aaf.createStartCountLimited(csaa, 1), repetitionsPerProgramType - 1));
    tw.addAutomationNoMetric(aaf.createStartCountDelay(
        aaf.createStartCountLimited(csaa2, 1),
        repetitionsPerProgramType * 2 - 1));

    std::string shadingType = "smooth";
    if (options.contains("--flat")) {
        shadingType = "flat";
    }

    if (options.contains("--stdout")) {
        tw.setPrinter(vcl::StdoutBenchmarkPrinter());
    }
    else if (options.contains("-o")) {
        std::vector<std::string> optArgs = options["-o"];
        tw.setPrinter(
            vcl::CsvBenchmarkPrinterNoDescription(
                optArgs[0], optArgs[1], optArgs[2], 6));
    }
    else if (options.contains("--output-folder")) {
        std::string folderString = options["--output-folder"][0];
        tw.setPrinter(
            vcl::CsvBenchmarkPrinterNoDescription(
                folderString + "/uber_result_" + shadingType + ".csv",
                folderString + "/split_result_" + shadingType + ".csv",
                folderString + "/uber_if_result_" + shadingType + ".csv",
                6));
    }
    else if (options.contains("--no-print")) {
        tw.setPrinter(vcl::NullBenchmarkPrinter());
    }
    else {
        tw.setPrinter(
            vcl::CsvBenchmarkPrinterNoDescription(
                "./uber_result_" + shadingType + ".csv",
                "./split_result_" + shadingType + ".csv",
                "./uber_if_result_" + shadingType + ".csv",
                6));
    }

    tw.terminateUponCompletion();

    tw.show();

    return 0;
}
