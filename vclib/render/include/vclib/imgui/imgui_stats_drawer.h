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

#ifndef VCL_IMGUI_IMGUI_STATS_DRAWER_H
#define VCL_IMGUI_IMGUI_STATS_DRAWER_H

#include <vclib/render/drawers/plain_drawer.h>
#include <imgui.h>

#include <algorithm>
#include <vector>

#ifdef VCLIB_RENDER_BACKEND_BGFX
    #include <bgfx/bgfx.h>
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    // include OpenGL headers
    #ifdef __APPLE__
    #include <OpenGL/gl.h>
    #else
    #ifdef _WIN32
    // dirty trick to avoid including all headers distributed by Khronos
    #define GL_SHADING_LANGUAGE_VERSION 0x8B8C
    #include <windows.h>
    #endif
    #include <GL/gl.h>
    #endif
#endif

namespace vcl::imgui {

template<typename DerivedDrawer>
class ImguiStatsDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    // frame times
    static constexpr size_t FRAME_COUNT = 256;
    std::vector<float> mFrameTimes = std::vector<float>(FRAME_COUNT, 0.0f);

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    // OpenGL information
    const GLubyte * mGlRendererString  = nullptr;
    const GLubyte * mGlVendorString    = nullptr;
    const GLubyte * mGlVersionString   = nullptr;
    const GLubyte * mGlslVersionString = nullptr;
#endif

public:
    using vcl::PlainDrawer<DerivedDrawer>::PlainDrawer;

    void onDrawContent(uint viewId)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Stats", nullptr);

#ifdef VCLIB_RENDER_BACKEND_BGFX
        // map renderer type to string
        static const
        std::unordered_map<uint16_t, const char*>
        sVendorIdNames =
        {
            { BGFX_PCI_ID_NONE                , "None" },
            { BGFX_PCI_ID_SOFTWARE_RASTERIZER , "Software"},
            { BGFX_PCI_ID_AMD                 , "AMD"},
            { BGFX_PCI_ID_APPLE               , "Apple"},
            { BGFX_PCI_ID_INTEL               , "Intel"},
            { BGFX_PCI_ID_NVIDIA              , "NVIDIA"},
            { BGFX_PCI_ID_MICROSOFT           , "Microsoft"},
            { BGFX_PCI_ID_ARM                 , "ARM"},
        };

        ImGui::SeparatorText("BGFX");
        // get bgfx information
        ImGui::Text("Renderer: %s",
            bgfx::getRendererName(bgfx::getCaps()->rendererType));
        auto it = sVendorIdNames.find(bgfx::getCaps()->vendorId);
        ImGui::Text("Vendor ID: %s", it != sVendorIdNames.end() ?
            it->second : "Unknown");
        ImGui::Text("Device ID: %d", bgfx::getCaps()->deviceId);
        ImGui::Text("GPUs: %d", bgfx::getCaps()->numGPUs);

        ImGui::Separator();

        // get bgfx stats
        const bgfx::Stats* stats = bgfx::getStats();
        ImGui::Text("Submitted calls:");
        ImGui::Text("\t%d draw - %d compute - %d blit",
            stats->numDraw, stats->numCompute, stats->numBlit);
        ImGui::Text("Backbuffer size: %d x %d", stats->width, stats->height);
        if (stats->gpuMemoryUsed > 0)
        {
            ImGui::Text("GPU mem: %d / %d MB",
                int(stats->gpuMemoryUsed / (1024 * 1024)),
                int(stats->gpuMemoryMax / (1024 * 1024)));
        }
        ImGui::Text("Texture memory: %d MB",
            int(stats->textureMemoryUsed / (1024 * 1024)));
        ImGui::Text("RT memory: %d MB",
            int(stats->rtMemoryUsed / (1024 * 1024)));
        
        // frame times
        const double toMsCpu = 1000.0/stats->cpuTimerFreq;
        const double toMsGpu = 1000.0/stats->gpuTimerFreq;
        // const double frameMs = double(stats->cpuTimeFrame)*toMsCpu;

        ImGui::Text("Submit CPU %0.3f, GPU %0.3f (L: %d) ms"
        , double(stats->cpuTimeEnd - stats->cpuTimeBegin)*toMsCpu
        , double(stats->gpuTimeEnd - stats->gpuTimeBegin)*toMsGpu
        , stats->maxGpuLatency
        );

#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
        // Display OpenGL information
        if (mGlRendererString == nullptr)
        {
            mGlRendererString =  glGetString(GL_RENDERER);
            mGlVendorString =    glGetString(GL_VENDOR);
            mGlVersionString =   glGetString(GL_VERSION);
            mGlslVersionString = glGetString(GL_SHADING_LANGUAGE_VERSION);
        }
        ImGui::SeparatorText("OpenGL");
        ImGui::Text("Renderer: %s", mGlRendererString);
        ImGui::Text("Vendor: %s", mGlVendorString);
        ImGui::Text("Version: %s", mGlVersionString);
        ImGui::Text("GLSL Version: %s", mGlslVersionString);

#endif
        ImGui::Separator();

        // Update frame times
        std::rotate(
            mFrameTimes.begin(),
            mFrameTimes.begin() + 1,
            mFrameTimes.end());
        mFrameTimes.back() = io.Framerate;

        // Display ImGui information
        ImGui::Text(
            "Display size: %g x %g",
            io.DisplaySize.x,
            io.DisplaySize.y);
        ImGui::Text(
            "Framebuffer scale: %g x %g",
            io.DisplayFramebufferScale.x,
            io.DisplayFramebufferScale.y);
        ImGui::Separator();

        // Display framerate plot
        ImGui::PlotLines("FPS", mFrameTimes.data(), int(mFrameTimes.size()),
                            0, nullptr, -5.0f, 120.0f, ImVec2(200, 0));

        // Display framerate
        ImGui::Text("%.3f ms/frame (%.1f FPS)",
                    1000.0f / io.Framerate, io.Framerate);

        ImGui::End();
    }
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_IMGUI_STATS_DRAWER_H
