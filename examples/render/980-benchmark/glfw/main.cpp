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

#include "cmd_opt_parser.h"

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#include <vclib/render/automation/actions.h>
#include <vclib/render/automation/actions/automation_action_factory.h>
#include <vclib/render/automation/metrics.h>
#include <vclib/render/automation/printers.h>
#include <vclib/render/drawers/benchmark_drawer.h>
#include <vclib/render/drawers/benchmark_viewer_drawer.h>

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/glfw/window_manager.h>

#include <imgui.h>

#include <regex>

#include <vclib/misc/logger/console_logger.h>

#if defined(WIN32) || defined(_WIN32) || \
    defined(__WIN32) && !defined(__CYGWIN__)
static const std::string PATH_SEP = "\\";
#else
static const std::string PATH_SEP = "/";
#endif

static const vcl::uint DEFAULT_WINDOW_WIDTH  = 1440;
static const vcl::uint DEFAULT_WINDOW_HEIGHT = 1080;

static const vcl::Color DEFAULT_USER_COLOR = vcl::Color(0, 255, 0);

static const vcl::uint DEFAULT_REPETITIONS = 2;

static const vcl::uint DEFAULT_FRAMES = 1000;

vcl::DrawableMesh<vcl::TriMesh> getMesh(std::string path, vcl::Color userColor)
{
    vcl::LoadSettings ldstngs{true, true};
    vcl::TriMesh mesh = vcl::load<vcl::TriMesh, vcl::ConsoleLogger>(path, ldstngs);
    vcl::updatePerVertexAndFaceNormals(mesh);

    vcl::MeshRenderSettings mrs(mesh);
    mrs.setSurfaceUserColor(userColor);
    mrs.setSurface(vcl::MeshRenderInfo::Surface::COLOR_USER);

    if (mrs.canSurface(vcl::MeshRenderInfo::Surface::COLOR_WEDGE_TEX)) {
        mrs.setSurface(vcl::MeshRenderInfo::Surface::COLOR_WEDGE_TEX);
    }
    else if (mrs.canSurface(vcl::MeshRenderInfo::Surface::COLOR_VERTEX_TEX)) {
        mrs.setSurface(vcl::MeshRenderInfo::Surface::COLOR_VERTEX_TEX);
    }
    else if (mrs.canSurface(vcl::MeshRenderInfo::Surface::COLOR_VERTEX)) {
        mrs.setSurface(vcl::MeshRenderInfo::Surface::COLOR_VERTEX);
    }
    else if (mrs.canSurface(vcl::MeshRenderInfo::Surface::COLOR_FACE)) {
        mrs.setSurface(vcl::MeshRenderInfo::Surface::COLOR_FACE);
    }
    vcl::DrawableMesh<vcl::TriMesh> drawable(mesh);
    drawable.setRenderSettings(mrs);
    return drawable;
}

int main(int argc, char** argv)
{
    using BenchmarkViewer = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::BenchmarkViewerDrawer>;

    using BenchmarDrawerT = vcl::BenchmarkDrawer<BenchmarkViewer>;

    using enum vcl::MeshRenderInfo::Buffers;
#ifdef VCLIB_RENDER_BACKEND_BGFX

    vcl::Context::setResetFlags(BGFX_RESET_NONE);

#endif

    CmdOptionParser optionParser = CmdOptionParser {
        {"--stdout",           0},
        {"-o",                 1},
        {"--output-dir",       1},
        {"-f",                 1},
        {"-r",                 1},
        {"-h",                 0},
        {"--help",             0},
        {"--no-print",         0},
        {"--flat",             0},
        {"--split",            0},
        {"--uber-static-if",   0},
        {"--res",              2},
        {"--user-color",       3},
        {"--force-user-color", 0}
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
               "each axis) 2 times (by default) using the given shader "
               "splitting (Uber by default) and the given shading type (Smooth "
               "by default):"
            << "\nusage: " << programName << " [options] model1 model2 ..."
            << "\noptions:"
            << "\n\t--stdout:           " << "Prints results to standard output"
            << "\n\t-o:                 "
            << "Allows you to choose the output file"
            << "\n\t--output-dir:       "
            << "Takes a directory path as an argument. "
               "Writes the "
               "results in DIRECTORY/SPLITTYPE_result_SHADINGTYPE.csv"
            << "\n\t--no-print:         " << "Disables result printing"
            << "\n\t-f:                 "
            << "Allows you to choose how many frames the rotations last "
               "(default 1000)"
            << "\n\t-r:                 "
            << "Allows you to choose how many times the 3 rotations "
               "are executed (default 2)"
            << "\n\t--flat:             "
            << "Uses flat shading for all the meshes"
            << "\n\t--split:            "
            << "Uses the \"SPLIT\" shader splitting"
            << "\n\t--uber-static-if:   "
            << "Uses the \"UBER_WITH_STATIC_IF\" shader splitting"
            << "\n\t--res:              "
            << "Allows you to choose window resolution. Takes width and height "
               "as parameters"
            << "\n\t--user-color:       "
            << "Allows you to choose user color. Takes R G B as parameters."
            << "\n\t--force-user-color: "
            << "Forces all meshes to use user color"
            << "\n\t-h, --help:         " << "Shows help page\n";
        exit(0);
    }

    // Check if any models are in the remaining arguments
    if (remainingArgs.size() == 0) {
        std::cerr << "Error: missing model argument(s)\n";
        exit(1);
    }

    // Check if there are multiple options pertaining the shader splitting type
    if (options.contains("--split") + options.contains("--uber-static-if") >
        1) {
        std::cerr << "Error: conflicting shader split type options\n";
        exit(1);
    }

    // Check if there are multiple options pertaining the output location
    if (options.contains("--stdout") + options.contains("-o") +
            options.contains("--output-dir") + options.contains("--no-print") >
        1) {
        std::cerr << "Error: conflicting output location options\n";
        exit(1);
    }

    // -f option implementation
    vcl::uint frames = DEFAULT_FRAMES;
    if (options.contains("-f")) {
        frames = std::strtoul(options["-f"][0].c_str(), nullptr, 10);
        if (frames == 0) {
            std::cerr
                << "Error: invalid frame amount (option -f), using default"
                << std::endl;
            frames = DEFAULT_FRAMES;
        }
    }

    // -r option implementation
    vcl::uint repetitions = 2;
    if (options.contains("-r")) {
        repetitions = std::strtoul(options["-r"][0].c_str(), nullptr, 10);
        if (repetitions == 0) {
            std::cerr << "Error: invalid repetitions amount (option -r), using "
                         "default"
                      << std::endl;
            repetitions = DEFAULT_REPETITIONS;
        }
    }

    // --res option implementation
    vcl::uint width  = DEFAULT_WINDOW_WIDTH;
    vcl::uint height = DEFAULT_WINDOW_HEIGHT;
    if (options.contains("--res")) {
        width  = std::strtoul(options["--res"][0].c_str(), nullptr, 10);
        height = std::strtoul(options["--res"][1].c_str(), nullptr, 10);
        if (width == 0) {
            std::cerr
                << "Error: invalid window width (option --res), using default"
                << std::endl;
            width = DEFAULT_WINDOW_WIDTH;
        }
        if (height == 0) {
            std::cerr
                << "Error: invalid window height (option --res), using default"
                << std::endl;
            height = DEFAULT_WINDOW_HEIGHT;
        }
    }

    // --user-color implementation
    vcl::Color userColor = DEFAULT_USER_COLOR;
    if (options.contains("--user-color")) {
        userColor = vcl::Color(
            std::strtoul(options["--user-color"][0].c_str(), nullptr, 10) % 256,
            std::strtoul(options["--user-color"][1].c_str(), nullptr, 10) % 256,
            std::strtoul(options["--user-color"][2].c_str(), nullptr, 10) % 256);
    }

    BenchmarkViewer tw("Benchmark", width, height);

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();

    // Insert the meshes one next to the other along the X axis
    vcl::Box3d bb;
    for (const auto& path : remainingArgs) {
        vcl::DrawableMesh<vcl::TriMesh> msh = getMesh(path, userColor);

        if (options.contains("--force-user-color")) {
            auto mrs = msh.renderSettings();
            mrs.setSurface(vcl::MeshRenderInfo::Surface::COLOR_USER);
            msh.setRenderSettings(mrs);
        }

        if (!bb.isNull()) {
            vcl::translate(msh, vcl::Point3d(bb.size().x(), 0, 0));
        }
        bb.add(vcl::boundingBox(msh));
        msh.updateBuffers({VERTICES, VERT_NORMALS});

        // --flat option implementation
        if (options.contains("--flat")) {
            vcl::MeshRenderSettings mrs = msh.renderSettings();
            mrs.setSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT);
            msh.setRenderSettings(mrs);
        }

        // --split and --uber-static-if options implementation
        if (options.contains("--split")) {
            msh.setSurfaceProgramType(
                vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType::SPLIT);
        }
        else if (options.contains("--uber-static-if")) {
            msh.setSurfaceProgramType(
                vcl::DrawableMesh<
                    vcl::TriMesh>::SurfaceProgramsType::UBER_WITH_STATIC_IF);
        }
        else {
            msh.setSurfaceProgramType(
                vcl::DrawableMesh<vcl::TriMesh>::SurfaceProgramsType::UBER);
        }
        vec->pushBack(std::move(msh));
    }

    tw.setDrawableObjectVector(vec);

    // An automation action factory, to shorten the length of Automation
    // declarations
    vcl::AutomationActionFactory<BenchmarDrawerT> aaf;

    tw.setRepeatTimes(repetitions);

    tw.setMetric(vcl::FpsBenchmarkMetric());

    // Rotation around Z axis
    tw.addAutomation(aaf.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<
            BenchmarDrawerT>::fromFramesPerRotation(frames, {0.f, 0.f, 1.f}),
        frames));

    // Rotation around Y axis
    tw.addAutomation(aaf.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<
            BenchmarDrawerT>::fromFramesPerRotation(frames, {0.f, 1.f, 0.f}),
        frames));

    // Rotation around X axis
    tw.addAutomation(aaf.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<
            BenchmarDrawerT>::fromFramesPerRotation(frames, {1.f, 0.f, 0.f}),
        frames));

    // Do nothing
    tw.addAutomation(aaf.createFrameLimited(aaf.createNull(), frames));

    std::string shadingType = "smooth";
    if (options.contains("--flat")) {
        shadingType = "flat";
    }

    std::string splitType = "uber";
    if (options.contains("--split")) {
        splitType = "split";
    }
    if (options.contains("--uber-static-if")) {
        splitType = "uber_static_if";
    }

    if (options.contains("--stdout")) {
        vcl::StdoutBenchmarkPrinter prntr;
        tw.setPrinter(vcl::StdoutBenchmarkPrinter(prntr));
    }
    else if (options.contains("-o")) {
        std::vector<std::string> optArgs = options["-o"];
        auto prntr = vcl::CsvBenchmarkPrinter(optArgs[0], true);
        prntr.useDescription(false);
        prntr.useHeader(false);
        prntr.useUnitOfMeasure(false);
        tw.setPrinter(prntr);
    }
    else if (options.contains("--output-dir")) {
        std::string folderString = options["--output-dir"][0];
        auto        prntr        = vcl::CsvBenchmarkPrinter(
            folderString + splitType + "_result_" + shadingType + ".csv", true);
        prntr.useDescription(false);
        prntr.useHeader(false);
        prntr.useUnitOfMeasure(false);
        tw.setPrinter(prntr);
    }
    else if (options.contains("--no-print")) {
        tw.setPrinter(vcl::NullBenchmarkPrinter());
    }
    else {
        auto prntr = vcl::CsvBenchmarkPrinter(
            "./" + splitType + "_result_" + shadingType + ".csv", true);
        prntr.useDescription(false);
        prntr.useHeader(false);
        prntr.useUnitOfMeasure(false);
        tw.setPrinter(prntr);
    }

    tw.terminateUponCompletion();

    tw.show();

    return 0;
}
