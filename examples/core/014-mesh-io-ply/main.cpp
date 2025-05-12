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

#include <vclib/algorithms/core/polygon/geometry.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/misc/logger/null_logger.h>

#include <iostream>

int main()
{
    vcl::MeshInfo loadedInfo;
    vcl::TriMesh  m = vcl::loadPly<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/brain.ply", loadedInfo);

    assert(loadedInfo.hasVertices());
    assert(m.vertexNumber() == 18844);
    assert(loadedInfo.hasFaces());
    assert(loadedInfo.hasPerFaceVertexReferences());
    assert(m.faceNumber() == 36752);

    m = vcl::loadPly<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bunny_textured.ply", loadedInfo);

    assert(loadedInfo.hasVertices());
    assert(m.vertexNumber() == 5051);
    assert(loadedInfo.hasFaces());
    assert(loadedInfo.hasPerFaceVertexReferences());
    assert(m.faceNumber() == 9999);
    assert(loadedInfo.hasTextures());
    assert(m.textureNumber() == 1);
    assert(loadedInfo.hasPerFaceWedgeTexCoords());
    assert(vcl::isPerFaceWedgeTexCoordsAvailable(m));
    assert(!vcl::isPerFaceAdjacentFacesAvailable(m));

    m = vcl::loadPly<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply", loadedInfo);

    assert(loadedInfo.hasVertices());
    assert(m.vertexNumber() == 8);
    assert(loadedInfo.hasFaces());
    assert(loadedInfo.hasPerFaceVertexReferences());
    assert(m.faceNumber() == 4);
    assert(loadedInfo.hasTextures());
    assert(m.textureNumber() == 2);

    // try to load a polygonal mesh into a trimesh
    m = vcl::loadPly<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply", loadedInfo);
    for (const vcl::TriMesh::Face& f : m.faces()) {
        assert(!f.edgeFaux(0));
        assert(!f.edgeFaux(1));
        assert(f.edgeFaux(2));
    }

    m.addPerFaceCustomComponent<double>("area");
    auto areaVec = m.perFaceCustomComponentVectorHandle<double>("area");

    for (const auto& f : m.faces()) {
        areaVec[m.index(f)] = vcl::faceArea(f);
        std::cerr << "area " << m.index(f) << ": " << areaVec[m.index(f)]
                  << "\n";
    }

    // save again the mesh
    vcl::SaveSettings s;
    s.binary = false;
    vcl::savePly(
        m, VCLIB_RESULTS_PATH "/triangulated_cube.ply", vcl::nullLogger, s);

    vcl::TriMesh mm = vcl::loadPly<vcl::TriMesh>(
        VCLIB_RESULTS_PATH "/triangulated_cube.ply", loadedInfo);
    assert(loadedInfo.hasPerFaceCustomComponents());

    std::cerr << "Loaded custom component areas: \n";
    for (const auto& f : mm.faces()) {
        assert(f.customComponent<double>("area") == 2);
        std::cerr << "area " << mm.index(f) << ": "
                  << f.customComponent<double>("area") << "\n";
    }

    return 0;
}
