// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_creation.h"

int main()
{
    auto
        [tetrahedron,
         cube,
         dodecahedron,
         customCube,
         cubeQuads,
         dodecahedronPoly,
         sphereQuads] = meshCreation();

    /****** Setup save settings ******/

    // Configure which mesh data to save
    vcl::MeshInfo info;
    info.setVertices();
    info.setFaces();
    info.setPerVertexPosition(true, vcl::PrimitiveType::FLOAT);
    info.setPerFaceVertexReferences();

    vcl::SaveSettings settings;
    settings.info = info;

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(
            tetrahedron, resultsPath + "/003_tetrahedron.ply", settings);

        vcl::saveMesh(cube, resultsPath + "/003_cube_tri.ply", settings);

        vcl::saveMesh(
            dodecahedron, resultsPath + "/003_dodecahedron_tri.ply", settings);

        vcl::saveMesh(
            customCube, resultsPath + "/003_custom_cube.ply", settings);

        vcl::saveMesh(cubeQuads, resultsPath + "/003_cube_poly.ply", settings);

        vcl::saveMesh(
            dodecahedronPoly,
            resultsPath + "/003_dodecahedron_poly.ply",
            settings);

        vcl::saveMesh(
            sphereQuads, resultsPath + "/003_sphere_poly.ply", settings);

        std::cout << "\nAll meshes have been saved to the results directory."
                  << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    std::cout << "Example completed successfully!\n";

    return 0;
}
