// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/context.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/headless_mesh_viewer.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/io.h>
#include <vclib/mesh.h>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

// We need a helper to load meshes and create drawable mesh
template<vcl::MeshConcept MeshType>
vcl::DrawableMesh<MeshType> getDrawableMesh(const std::string& filename)
{
    vcl::MeshInfo info;
    MeshType      m = vcl::loadMesh<MeshType>(
        std::string(VCLIB_EXAMPLE_MESHES_PATH) + "/gltf/" + filename, info);

    if constexpr (vcl::FaceMeshConcept<MeshType>) {
        if (!info.hasPerFaceNormal()) {
            vcl::updatePerFaceNormals(m);
        }
        if (!info.hasPerVertexNormal()) {
            vcl::updatePerVertexNormalsFromFaceNormals(m);
        }
    }

    return vcl::makeDrawable(std::move(m));
}

void runRenderTest(
    const std::string&                            testName,
    float                                         angle,
    std::function<void(vcl::HeadlessMeshViewer&)> setup)
{
    vcl::HeadlessMeshViewer mv("Headless Mesh Viewer", 1920, 1080);

    // run custom setup
    setup(mv);

    // Apply fitscene to center everything
    mv.fitScene();

    // Zoom in a bit to make the mesh larger
    mv.trackballZoom(-150.0f);

    if (angle > 0.0f) {
        mv.trackballRotate(vcl::Point3f(0.0f, 1.0f, 0.0f), angle);
    }

    vcl::Image renderedImage;
    // this auto concludes loop
    mv.screenshot(renderedImage);

    REQUIRE_FALSE(renderedImage.isNull());

    std::string groundTruthFilename = std::string(VCLIB_GROUND_TRUTH_PATH) +
                                      "/006-mesh-pbr-headless_" + testName +
                                      "_gt.png";

    if (!std::filesystem::exists(groundTruthFilename)) {
        FAIL("Ground truth image not found.");
    }

    vcl::Image groundTruthImage = vcl::loadImage(groundTruthFilename);
    REQUIRE_FALSE(groundTruthImage.isNull());

    REQUIRE(renderedImage.width() == groundTruthImage.width());
    REQUIRE(renderedImage.height() == groundTruthImage.height());
    REQUIRE(renderedImage.colorSpace() == groundTruthImage.colorSpace());
    REQUIRE(renderedImage.sizeInBytes() == groundTruthImage.sizeInBytes());

    // PBR rendering can produce slightly different results across different
    // hardwares. We increase the tolerance to max diff 10 per channel and 2%
    // failing pixels.
    bool match = renderedImage.isAlmostEqual(groundTruthImage, 10, 0.02f);

    std::string resultFilename = std::string(VCLIB_RENDER_RESULTS_PATH) +
                                 "/006-mesh-pbr-headless_" + testName +
                                 "_res.png";
    vcl::saveImage(renderedImage, resultFilename);

    REQUIRE(match);
}

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
                        testName, angle, [&](vcl::HeadlessMeshViewer& mv) {
                            auto mesh = getDrawableMesh<vcl::TriMesh>(meshName);
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
                        });
                }
            }
        }
    }
}
