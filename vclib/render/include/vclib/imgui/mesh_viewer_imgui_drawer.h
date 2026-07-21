// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_MESH_VIEWER_IMGUI_DRAWER_H
#define VCL_IMGUI_MESH_VIEWER_IMGUI_DRAWER_H

#include "imgui_helpers.h"

#include <vclib/render/concepts/viewer.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/editors.h>
#include <vclib/render/settings/viewer_settings.h>
#include <vclib/render/settings/render_mode.h>
#include <vclib/imgui/gui/editor_frame.h>
#include <vclib/imgui/gui/toolbar_frames/bounding_box_editor_frame.h>
#include <vclib/imgui/gui/toolbar_frames/selection_editor_frame.h>

#include <imgui/imgui.h>

#include <algorithm>
#include <cstring>
#include <iterator>

namespace vcl::imgui {

template<typename DerivedRenderApp>
class MeshViewerDrawerImgui : public vcl::ViewerDrawer<DerivedRenderApp>
{
    using Base = vcl::ViewerDrawer<DerivedRenderApp>;

    bool mShowViewerSettings       = false;
    char mPanoramaPathBuffer[1024] = "";

    std::vector<std::shared_ptr<EditorFrameImgui>> mEditorFrames;

public:
    MeshViewerDrawerImgui(uint width = 1024, uint height = 768) :
            Base(width, height)
    {
    }

    void addEditorFrame(std::shared_ptr<EditorFrameImgui> frame)
    {
        mEditorFrames.push_back(frame);
    }

    virtual void onDraw(vcl::uint viewId) override
    {
        // draw parent
        Base::onDraw(viewId);

        if constexpr (ViewerConcept<Base>) {
            ViewerSettings viewerSettings = Base::viewerSettings();

            // Main Menu Bar
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("Render")) {
                    if (ImGui::BeginMenu("Render Mode")) {
                        bool classicChecked = (viewerSettings.renderMode == RenderMode::CLASSIC);
                        if (ImGui::MenuItem("Classic", nullptr, &classicChecked)) {
                            viewerSettings.renderMode = RenderMode::CLASSIC;
                            Base::setViewerSettings(viewerSettings);
                        }
                        bool pbrChecked = (viewerSettings.renderMode == RenderMode::PBR);
                        if (ImGui::MenuItem("PBR", nullptr, &pbrChecked)) {
                            viewerSettings.renderMode = RenderMode::PBR;
                            Base::setViewerSettings(viewerSettings);
                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("Viewer Settings", nullptr, &mShowViewerSettings)) {
                        if (mShowViewerSettings) {
                            std::string panName = Base::panoramaFileName();
                            std::strncpy(
                                mPanoramaPathBuffer,
                                panName.c_str(),
                                sizeof(mPanoramaPathBuffer) - 1);
                            mPanoramaPathBuffer[sizeof(mPanoramaPathBuffer) - 1] = '\0';
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            // Viewer Settings Window
            if (mShowViewerSettings) {
                if (ImGui::Begin("Viewer Settings", &mShowViewerSettings)) {
                    // exposure slider
                    float exposure = viewerSettings.exposure;
                    if (ImGui::SliderFloat(
                            "Exposure",
                            &exposure,
                            0.001f,
                            64.0f,
                            "%.3f",
                            ImGuiSliderFlags_Logarithmic))
                        viewerSettings.exposure = exposure;

                    // tone mapping combo box
                    uint toneMapping = toUnderlying(viewerSettings.toneMapping);
                    const auto* toneMappingNames = ViewerSettings::TONE_MAPPING_STRINGS;
                    if (ImGui::BeginCombo("Tone mapping", toneMappingNames[toneMapping])) {
                        const uint CNT = toUnderlying(ViewerSettings::ToneMapping::COUNT);
                        for (uint n = 0; n < CNT; n++) {
                            bool isSelected = toneMapping == n;
                            if (ImGui::Selectable(toneMappingNames[n], isSelected)) {
                                viewerSettings.toneMapping = static_cast<ViewerSettings::ToneMapping>(n);
                            }
                            if (isSelected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::InputText(
                        "Panorama Path",
                        mPanoramaPathBuffer,
                        IM_ARRAYSIZE(mPanoramaPathBuffer));
                    ImGui::SameLine();
                    if (ImGui::Button("Load")) {
                        Base::setPanorama(std::string(mPanoramaPathBuffer));
                    }

                    bool hasPanorama = !Base::panoramaFileName().empty();
                    ImGui::BeginDisabled(!hasPanorama);

                    // image based lighting
                    ImGui::Checkbox(
                        "Image Based Lighting",
                        [&]() { return viewerSettings.imageBasedLighting; },
                        [&](bool ibl) { viewerSettings.imageBasedLighting = ibl; });

                    // draw background checkbox
                    ImGui::Checkbox(
                        "Render Background Panorama",
                        [&]() { return viewerSettings.renderBackgroundPanorama; },
                        [&](bool renderBg) { viewerSettings.renderBackgroundPanorama = renderBg; });

                    ImGui::EndDisabled();

                    Base::setViewerSettings(viewerSettings);
                }
                ImGui::End();
            }
        }

        // draw imgui
        ImGui::Begin("Meshes");

        // mesh table
        {
            ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_HorizontalScrollbar;
            ImGui::BeginChild(
                "##ListContainer",
                ImVec2(ImGui::GetContentRegionAvail().x, 260),
                ImGuiChildFlags_None,
                window_flags);
            drawMeshList();
            ImGui::EndChild();
        }

        // drawable mesh info and settings for selected mesh
        uint meshIndex = Base::mDrawList->selectedObjectId();
        if (meshIndex >= 0 && meshIndex < Base::mDrawList->size()) {
            auto drawable =
                std::dynamic_pointer_cast<vcl::AbstractDrawableMesh>(
                    Base::mDrawList->at(meshIndex));
            if (drawable) {
                drawMeshSettings(*drawable);
            }
        }

        ImGui::End();

        // floating editors toolbar
        drawToolbar();
    }

private:
    void drawToolbar()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2               toolbarPos =
            ImVec2(viewport->WorkPos.x + 10.0f, viewport->WorkPos.y + 10.0f);

        ImGui::SetNextWindowPos(toolbarPos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.85f);

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if (ImGui::Begin("Toolbar", nullptr, flags)) {
            // axis toggle
            bool axisActive = Base::isAxisVisible();
            if (ImGui::Button(axisActive ? "[Axis]" : " Axis ")) {
                Base::toggleAxisVisibility();
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
                ImGui::SetTooltip("Show Axis");

            ImGui::SameLine();

            // trackball
            bool trackballActive = Base::isTrackBallVisible();
            if (ImGui::Button(trackballActive ? "[TB]" : " TB ")) {
                Base::toggleTrackBallVisibility();
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
                ImGui::SetTooltip("Show Trackball");

            for (auto& frame : mEditorFrames) {
                ImGui::SameLine(0, 6);
                frame->draw();
            }
        }
        ImGui::End();
    }

    void drawMeshList()
    {
        uint meshIndex = Base::mDrawList->selectedObjectId();

        if (!Base::mDrawList || Base::mDrawList->empty()) {
            ImGui::Text("No objects loaded");
            return;
        }

        int                    meshId         = 0;
        static ImGuiTableFlags meshTableFlags = 0 | ImGuiTableFlags_BordersH;
        if (ImGui::BeginTable("meshtable", 2, meshTableFlags)) {
            ImGui::TableSetupColumn(
                "Visible", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            for (auto& d : *(Base::mDrawList)) {
                auto& drawable = *d;
                ImGui::TableNextRow();

                ImGui::PushID(meshId++);

                ImGui::TableSetColumnIndex(0);

                // visibility checkbox
                ImGui::Checkbox(
                    "##Visible",
                    [&] {
                        return drawable.isVisible();
                    },
                    [&](bool vis) {
                        drawable.setVisibility(vis);
                    });

                ImGui::TableSetColumnIndex(1);

                // row selection
                bool isSelected = (meshIndex == meshId - 1);
                if (ImGui::Selectable(
                        drawable.name().c_str(),
                        isSelected,
                        ImGuiSelectableFlags_SpanAllColumns)) {
                    Base::mDrawList->setSelectedObjectId(meshId - 1);
                }
                // tooltip with info
                if (!drawable.info().empty() &&
                    ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", drawable.info().c_str());
                    ImGui::EndTooltip();
                }
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
    }

    static void drawMeshPointSettings(
        const vcl::AbstractDrawableMesh& drawable,
        vcl::MeshRenderSettings&         settings)
    {
        using enum vcl::MeshRenderInfo::Points;

        ImGui::BeginDisabled(!settings.canPoints(VISIBLE));

        // visibility
        ImGui::Checkbox(
            "Visible",
            [&] {
                return settings.isPoints(VISIBLE);
            },
            [&](bool vis) {
                settings.setPoints(VISIBLE, vis);
            });

        // shape
        ImGui::Text("Shape:");
        ImGui::SameLine();
        ImGui::RadioButton(
            "Circle",
            [&] {
                return settings.isPoints(SHAPE_CIRCLE);
            },
            [&](bool v) {
                if (v)
                    settings.setPoints(SHAPE_CIRCLE);
            });
        ImGui::SameLine();
        ImGui::RadioButton(
            "Pixel",
            [&] {
                return settings.isPoints(SHAPE_PIXEL);
            },
            [&](bool v) {
                if (v)
                    settings.setPoints(SHAPE_PIXEL);
            });

        // shading
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.canPoints(SHADING_VERT));
        ImGui::RadioButton(
            "Vertex",
            [&] {
                return settings.isPoints(SHADING_VERT);
            },
            [&](bool v) {
                if (v)
                    settings.setPoints(SHADING_VERT);
            });
        ImGui::SameLine();
        ImGui::EndDisabled();
        ImGui::RadioButton(
            "None",
            [&] {
                return settings.isPoints(SHADING_NONE);
            },
            [&](bool vis) {
                if (vis)
                    settings.setPoints(SHADING_NONE);
            });

        // color
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* pointColorNames[]           = {"Vertex", "Mesh", "User"};
        const std::array<bool, 3> colorSelected = {
            settings.isPoints(COLOR_VERTEX),
            settings.isPoints(COLOR_MESH),
            settings.isPoints(COLOR_USER)};

        assert(
            std::accumulate(
                std::begin(colorSelected), std::end(colorSelected), 0) == 1);
        int idx = std::distance(
            std::begin(colorSelected),
            std::find(
                std::begin(colorSelected), std::end(colorSelected), true));
        assert(idx >= 0 && idx < 3);

        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboPointColor", pointColorNames[idx])) {
            for (int n = 0; n < IM_ARRAYSIZE(pointColorNames); n++) {
                const bool selected = (n == idx);

                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canPoints(COLOR_VERTEX));
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPoints(COLOR_VERTEX);
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canPoints(COLOR_MESH));
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPoints(COLOR_MESH);
                    ImGui::EndDisabled();
                    break;
                case 2:
                    if (ImGui::Selectable(pointColorNames[n], selected))
                        settings.setPoints(COLOR_USER);
                    break;
                default: assert(false); break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isPoints(COLOR_USER));
        ImGui::ColorEdit4(
            "##PointColor",
            [&] {
                return settings.pointUserColor();
            },
            [&](vcl::Color c) {
                settings.setPointsUserColor(c);
            },
            ImGuiColorEditFlags_NoInputs);
        ImGui::EndDisabled();

        // point size
        ImGui::Text("Size:");
        // set the width of the window minus the width of the label
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-10);
        ImGui::SliderFloat(
            "##PointSize",
            [&] {
                return settings.pointWidth();
            },
            [&](float v) {
                settings.setPointsWidth(v);
            },
            1.0f,
            32.0f);

        // selection visibility
        ImGui::Checkbox(
            "Selection",
            [&] {
                return settings.isPoints(SELECTION);
            },
            [&](bool vis) {
                settings.setPoints(SELECTION, vis);
            });

        // selection color picker
        ImGui::SameLine();
        ImGui::Text("Selection Color:");
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##PointSelectionColor",
            [&] {
                return settings.pointSelectionColor();
            },
            [&](vcl::Color c) {
                // alpha is always 128
                c.alpha() = 128;
                settings.setPointSelectionColor(c);
            },
            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

        ImGui::EndDisabled();
    }

    static void drawMeshSurfaceSettings(
        const vcl::AbstractDrawableMesh& drawable,
        vcl::MeshRenderSettings&         settings)
    {
        using enum vcl::MeshRenderInfo::Surface;

        ImGui::BeginDisabled(!settings.canSurface(VISIBLE));

        // visibility
        ImGui::Checkbox(
            "Visible",
            [&] {
                return settings.isSurface(VISIBLE);
            },
            [&](bool vis) {
                settings.setSurface(VISIBLE, vis);
            });

        // shading
        assert(
            (settings.isSurface(SHADING_SMOOTH) +
             settings.isSurface(SHADING_NORMAL_MAP) +
             settings.isSurface(SHADING_FLAT) +
             settings.isSurface(SHADING_NONE)) == 1);
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::RadioButton(
            "Smooth",
            [&] {
                return settings.isSurface(SHADING_SMOOTH);
            },
            [&](bool vis) {
                if (vis)
                    settings.setSurface(SHADING_SMOOTH);
            });
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.canSurface(SHADING_NORMAL_MAP));
        ImGui::RadioButton(
            "Normal Map",
            [&] {
                return settings.isSurface(SHADING_NORMAL_MAP);
            },
            [&](bool vis) {
                if (vis)
                    settings.setSurface(SHADING_NORMAL_MAP);
            });
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::RadioButton(
            "Flat",
            [&] {
                return settings.isSurface(SHADING_FLAT);
            },
            [&](bool vis) {
                if (vis)
                    settings.setSurface(SHADING_FLAT);
            });
        ImGui::SameLine();
        ImGui::RadioButton(
            "None",
            [&] {
                return settings.isSurface(SHADING_NONE);
            },
            [&](bool vis) {
                if (vis)
                    settings.setSurface(SHADING_NONE);
            });

        // color
        const uint CS_COUNT =
            toUnderlying(COLOR_USER) - toUnderlying(COLOR_VERTEX) + 1;
        // exclude shading and selection options

        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* surfColorNames[CS_COUNT] = {
            "Vertex", "Face", "Mesh", "PerVertexTex", "PerWedgeTex", "User"};

        const std::array<bool, CS_COUNT> colorSelected = {
            settings.isSurface(COLOR_VERTEX),
            settings.isSurface(COLOR_FACE),
            settings.isSurface(COLOR_MESH),
            settings.isSurface(COLOR_VERTEX_TEX),
            settings.isSurface(COLOR_WEDGE_TEX),
            settings.isSurface(COLOR_USER)};
        assert(
            std::accumulate(
                std::begin(colorSelected), std::end(colorSelected), 0) == 1);
        int idx = std::distance(
            std::begin(colorSelected),
            std::find(
                std::begin(colorSelected), std::end(colorSelected), true));
        assert(idx >= 0 && idx < CS_COUNT);
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboSurfColor", surfColorNames[idx])) {
            for (int n = 0; n < CS_COUNT; n++) {
                const bool selected = (n == idx);

                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canSurface(COLOR_VERTEX));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_VERTEX);
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canSurface(COLOR_FACE));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_FACE);
                    ImGui::EndDisabled();
                    break;
                case 2:
                    ImGui::BeginDisabled(!settings.canSurface(COLOR_MESH));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_MESH);
                    ImGui::EndDisabled();
                    break;
                case 3:
                    ImGui::BeginDisabled(
                        !settings.canSurface(COLOR_VERTEX_TEX));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_VERTEX_TEX);
                    ImGui::EndDisabled();
                    break;
                case 4:
                    ImGui::BeginDisabled(!settings.canSurface(COLOR_WEDGE_TEX));
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_WEDGE_TEX);
                    ImGui::EndDisabled();
                    break;
                case 5:
                    if (ImGui::Selectable(surfColorNames[n], selected))
                        settings.setSurface(COLOR_USER);
                    break;
                default: assert(false); break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isSurface(COLOR_USER));
        ImGui::ColorEdit4(
            "##SurfUserColor",
            [&] {
                return settings.surfaceUserColor();
            },
            [&](vcl::Color c) {
                settings.setSurfaceUserColor(c);
            },
            ImGuiColorEditFlags_NoInputs);
        ImGui::EndDisabled();

        // selection visibility
        ImGui::Checkbox(
            "Selection",
            [&] {
                return settings.isSurface(SELECTION);
            },
            [&](bool vis) {
                settings.setSurface(SELECTION, vis);
            });

        // selection color picker
        ImGui::SameLine();
        ImGui::Text("Selection Color:");
        ImGui::SameLine();
        ImGui::ColorEdit4(
            "##SurfaceSelectionColor",
            [&] {
                return settings.surfaceSelectionColor();
            },
            [&](vcl::Color c) {
                // alpha is always 128
                c.alpha() = 128;
                settings.setSurfaceSelectionColor(c);
            },
            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

        ImGui::EndDisabled();
    }

    static void drawMeshWireframeSettings(
        const vcl::AbstractDrawableMesh& drawable,
        vcl::MeshRenderSettings&         settings)
    {
        using enum vcl::MeshRenderInfo::Wireframe;

        ImGui::BeginDisabled(!settings.canWireframe(VISIBLE));

        // visibility
        ImGui::Checkbox(
            "Visible",
            [&] {
                return settings.isWireframe(VISIBLE);
            },
            [&](bool v) {
                settings.setWireframe(VISIBLE, v);
            });

        // shading
        assert(
            settings.isWireframe(SHADING_VERT) !=
            settings.isWireframe(SHADING_NONE));
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::RadioButton(
            "Vertex",
            [&] {
                return settings.isWireframe(SHADING_VERT);
            },
            [&](bool vis) {
                if (vis)
                    settings.setWireframe(SHADING_VERT);
            });
        ImGui::SameLine();
        ImGui::RadioButton(
            "None",
            [&] {
                return settings.isWireframe(SHADING_NONE);
            },
            [&](bool vis) {
                if (vis)
                    settings.setWireframe(SHADING_NONE);
            });

        // color
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char*               wireColorNames[] = {"Vertex", "Mesh", "User"};
        const std::array<bool, 3> colorSelected    = {
            settings.isWireframe(COLOR_VERTEX),
            settings.isWireframe(COLOR_MESH),
            settings.isWireframe(COLOR_USER)};
        assert(
            std::accumulate(
                std::begin(colorSelected), std::end(colorSelected), 0) == 1);
        int idx = std::distance(
            std::begin(colorSelected),
            std::find(
                std::begin(colorSelected), std::end(colorSelected), true));
        assert(idx >= 0 && idx < 3);
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboWireColor", wireColorNames[idx])) {
            for (int n = 0; n < IM_ARRAYSIZE(wireColorNames); n++) {
                const bool selected = (n == idx);

                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canWireframe(COLOR_VERTEX));
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframe(COLOR_VERTEX);
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canWireframe(COLOR_MESH));
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframe(COLOR_MESH);
                    ImGui::EndDisabled();
                    break;
                case 2:
                    if (ImGui::Selectable(wireColorNames[n], selected))
                        settings.setWireframe(COLOR_USER);
                    break;
                default: assert(false); break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isWireframe(COLOR_USER));
        ImGui::ColorEdit4(
            "##WireUserColor",
            [&] {
                return settings.wireframeUserColor();
            },
            [&](vcl::Color c) {
                settings.setWireframeUserColor(c);
            },
            ImGuiColorEditFlags_NoInputs);
        ImGui::EndDisabled();

        // wireframe size
        ImGui::Text("Size:");
        // set the width of the window minus the width of the label
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-10);
        ImGui::SliderFloat(
            "##WireframeSize",
            [&] {
                return settings.wireframeWidth();
            },
            [&](float v) {
                settings.setWireframeWidth(v);
            },
            1.0f,
            32.0f);

        ImGui::EndDisabled();
    }

    static void drawMeshEdgeSettings(
        const vcl::AbstractDrawableMesh& drawable,
        vcl::MeshRenderSettings&         settings)
    {
        using enum vcl::MeshRenderInfo::Edges;

        ImGui::BeginDisabled(!settings.canEdges(VISIBLE));

        // visibility
        ImGui::Checkbox(
            "Visible",
            [&] {
                return settings.isEdges(VISIBLE);
            },
            [&](bool v) {
                settings.setEdges(VISIBLE, v);
            });

        // shading
        assert(
            (settings.isEdges(SHADING_SMOOTH) + settings.isEdges(SHADING_FLAT) +
             settings.isEdges(SHADING_NONE)) == 1);
        ImGui::Text("Shading:");
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.canEdges(SHADING_SMOOTH));
        ImGui::RadioButton(
            "Smooth",
            [&] {
                return settings.isEdges(SHADING_SMOOTH);
            },
            [&](bool v) {
                if (v)
                    settings.setEdges(SHADING_SMOOTH);
            });
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.canEdges(SHADING_FLAT));
        ImGui::RadioButton(
            "Flat",
            [&] {
                return settings.isEdges(SHADING_FLAT);
            },
            [&](bool v) {
                if (v)
                    settings.setEdges(SHADING_FLAT);
            });
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.canEdges(SHADING_NONE));
        ImGui::RadioButton(
            "None",
            [&] {
                return settings.isEdges(SHADING_NONE);
            },
            [&](bool vis) {
                if (vis)
                    settings.setEdges(SHADING_NONE);
            });
        ImGui::EndDisabled();

        // color
        ImGui::Text("Color:");
        ImGui::SameLine();
        const char* edgeColorNames[] = {"Vertex", "Edge", "Mesh", "User"};
        const std::array<bool, 4> colorSelected = {
            settings.isEdges(COLOR_VERTEX),
            settings.isEdges(COLOR_EDGE),
            settings.isEdges(COLOR_MESH),
            settings.isEdges(COLOR_USER)};
        assert(
            std::accumulate(
                std::begin(colorSelected), std::end(colorSelected), 0) == 1);
        int idx = std::distance(
            std::begin(colorSelected),
            std::find(
                std::begin(colorSelected), std::end(colorSelected), true));
        assert(idx >= 0 && idx < 4);
        ImGui::SetNextItemWidth(-40);
        if (ImGui::BeginCombo("##ComboEdgeColor", edgeColorNames[idx])) {
            for (int n = 0; n < IM_ARRAYSIZE(edgeColorNames); n++) {
                const bool selected = (n == idx);

                switch (n) {
                case 0:
                    ImGui::BeginDisabled(!settings.canEdges(COLOR_VERTEX));
                    if (ImGui::Selectable(edgeColorNames[n], selected))
                        settings.setEdges(COLOR_VERTEX);
                    ImGui::EndDisabled();
                    break;
                case 1:
                    ImGui::BeginDisabled(!settings.canEdges(COLOR_EDGE));
                    if (ImGui::Selectable(edgeColorNames[n], selected))
                        settings.setEdges(COLOR_EDGE);
                    ImGui::EndDisabled();
                    break;
                case 2:
                    ImGui::BeginDisabled(!settings.canEdges(COLOR_MESH));
                    if (ImGui::Selectable(edgeColorNames[n], selected))
                        settings.setEdges(COLOR_MESH);
                    ImGui::EndDisabled();
                    break;
                case 3:
                    if (ImGui::Selectable(edgeColorNames[n], selected))
                        settings.setEdges(COLOR_USER);
                    break;
                default: assert(false); break;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // user color picker
        ImGui::SameLine();
        ImGui::BeginDisabled(!settings.isEdges(COLOR_USER));
        ImGui::ColorEdit4(
            "##EdgeUserColor",
            [&] {
                return settings.edgesUserColor();
            },
            [&](vcl::Color c) {
                settings.setEdgesUserColor(c);
            },
            ImGuiColorEditFlags_NoInputs);
        ImGui::EndDisabled();

        // edge size
        ImGui::Text("Size:");
        // set the width of the window minus the width of the label
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-10);
        ImGui::SliderFloat(
            "##EdgeSize",
            [&] {
                return settings.edgesWidth();
            },
            [&](float v) {
                settings.setEdgesWidth(v);
            },
            1.0f,
            32.0f);

        ImGui::EndDisabled();
    }

    static void drawMeshSettings(vcl::AbstractDrawableMesh& drawable)
    {
        using MRI = vcl::MeshRenderInfo;

        ImGui::Separator();
        // mesh settings
        const auto settings    = drawable.renderSettings();
        auto       newSettings = settings;

        // points
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
            // points
            if (newSettings.canPoints(MRI::Points::VISIBLE) &&
                ImGui::BeginTabItem("Points")) {
                drawMeshPointSettings(drawable, newSettings);
                ImGui::EndTabItem();
            }

            // surface + wireframe
            if (newSettings.canSurface(MRI::Surface::VISIBLE)) {
                if (ImGui::BeginTabItem("Surface")) {
                    drawMeshSurfaceSettings(drawable, newSettings);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Wireframe")) {
                    drawMeshWireframeSettings(drawable, newSettings);
                    ImGui::EndTabItem();
                }
            }

            // edges
            if (newSettings.canEdges(MRI::Edges::VISIBLE) &&
                ImGui::BeginTabItem("Edges")) {
                drawMeshEdgeSettings(drawable, newSettings);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        if (newSettings != settings) {
            drawable.setRenderSettings(newSettings);
        }
    }
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_MESH_VIEWER_IMGUI_DRAWER_H
