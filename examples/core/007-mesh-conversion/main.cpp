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

#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/misc/timer.h>

int main()
{
    vcl::TriMesh m = vcl::loadPly<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                                "/TextureDouble.ply");

    vcl::PolyMesh pm;

    vcl::Timer t("import");
    pm.enableSameOptionalComponentsOf(m);
    pm.importFrom(m);
    t.stopAndPrint();

    assert(pm.isPerFaceWedgeTexCoordsEnabled());

    vcl::save(pm, VCLIB_RESULTS_PATH "/TextureDouble_converted.obj");

    pm =
        vcl::loadPly<vcl::PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    m.clear();
    m.disableAllPerFaceOptionalComponents();
    m.disableAllPerVertexOptionalComponents();
    m.enableSameOptionalComponentsOf(pm);
    m.importFrom(pm);

    vcl::SaveSettings s;
    s.binary = false;

    vcl::save(m, VCLIB_RESULTS_PATH "/cube_from_poly.ply", s);

    pm = vcl::loadObj<vcl::PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                     "/rhombicosidodecahedron.obj");

    m.importFrom(pm);

    vcl::save(m, VCLIB_RESULTS_PATH "/tri_rhombicosidodecahedron.ply", s);

    return 0;
}
