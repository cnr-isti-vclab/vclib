// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"
#include "run_render_test.h"

static const std::string TEST_NAME = "006-mesh-pbr-headless";

TEST_CASE("PBR Rendering")
{
    std::vector<std::string> panoramas = {"pisa.hdr", "uffizi.hdr"};
    std::vector<std::string> meshes    = {
        "DamagedHelmet/DamagedHelmet.gltf",
        "MetalRoughSpheres/MetalRoughSpheres.gltf",
        "CompareAmbientOcclusion/CompareAmbientOcclusion.gltf",
        "NormalTangentMirrorTest/NormalTangentMirrorTest.gltf",
        "CompareEmissiveStrength/CompareEmissiveStrength.gltf"};
    std::vector<float> angles = {0.0f, 3.14159265f / 4.0f};

    for (const auto& meshName : meshes) {
        for (const auto& panoramaName : panoramas) {
            for (size_t i = 0; i < angles.size(); ++i) {
                float angle = angles[i];

                std::string cleanMeshName =
                    meshName.substr(0, meshName.find_first_of("/."));
                std::string cleanPanName =
                    panoramaName.substr(0, panoramaName.find_first_of("/."));
                std::string testName = cleanMeshName + "_" + cleanPanName +
                                       "_angle" + std::to_string(i);

                SECTION(testName)
                {
                    runRenderTest(
                        TEST_NAME,
                        testName,
                        [&](vcl::HeadlessMeshViewer& mv) {
                            auto mesh = getDrawableMesh<vcl::TriMesh>(
                                "gltf/" + meshName);
                            auto settings = mesh.renderSettings();
                            settings.setSurface(
                                vcl::MeshRenderInfo::Surface::VISIBLE, true);
                            mesh.setRenderSettings(settings);

                            mv.pushDrawableObject(std::move(mesh));

                            auto viewerSettings       = mv.viewerSettings();
                            viewerSettings.renderMode = vcl::RenderMode::PBR;
                            viewerSettings.renderBackgroundPanorama = true;
                            viewerSettings.imageBasedLighting       = true;
                            mv.setViewerSettings(viewerSettings);

                            std::string panPath =
                                std::string(VCLIB_ASSETS_PATH) + "/panoramas/" +
                                panoramaName;
                            mv.setPanorama(panPath);
                        },
                        angle,
                        false,
                        -150.0f,
                        10,
                        0.02f);
                }
            }
        }
    }
}
