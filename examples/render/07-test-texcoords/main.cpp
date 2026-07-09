// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <default_viewer.h>
#include <get_drawable_mesh.h>

int main(int argc, char** argv)
{
    vcl::Application app(argc, argv);

    vcl::MeshViewer viewer;

    enum TexCoordsExamples {
        SPONZA = 0,
        VERT_TEXTURE_PLY,
        BUNNY,
        QUAD_OBJ,
        TEXTURE_DOUBLE_OBJ,
        SPOT,
        COUNT
    };

    static const std::string exampleFilenames[COUNT] = {
        "sponza/sponza.obj",
        "VertTextureDouble.ply",
        "bunny_textured.ply",
        "TextureDoubleQuad.obj",
        "TextureDouble.obj",
        "spot/spot_quadrangulated.obj"};

    uint selectedExample = SPONZA;

    vcl::DrawableMesh<vcl::PolyMesh> drawable =
        getDrawableMesh<vcl::PolyMesh>(exampleFilenames[selectedExample]);
    showMeshesOnViewer(argc, argv, viewer, std::move(drawable));

    return app.exec();
}
