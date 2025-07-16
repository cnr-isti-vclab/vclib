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
        vcl::save(
            tetrahedron, VCLIB_RESULTS_PATH "/003_tetrahedron.ply", settings);

        vcl::save(cube, VCLIB_RESULTS_PATH "/003_cube_tri.ply", settings);

        vcl::save(
            dodecahedron,
            VCLIB_RESULTS_PATH "/003_dodecahedron_tri.ply",
            settings);

        vcl::save(
            customCube, VCLIB_RESULTS_PATH "/003_custom_cube.ply", settings);

        vcl::save(cubeQuads, VCLIB_RESULTS_PATH "/003_cube_poly.ply", settings);

        vcl::save(
            dodecahedronPoly,
            VCLIB_RESULTS_PATH "/003_dodecahedron_poly.ply",
            settings);

        vcl::savePly(
            sphereQuads, VCLIB_RESULTS_PATH "/003_sphere_poly.ply", settings);

        std::cout << "\nAll meshes have been saved to the results directory."
                  << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    std::cout << "Example completed successfully!\n";

    return 0;
}
