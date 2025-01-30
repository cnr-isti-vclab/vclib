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

#include <default_viewer.h>
#include <get_drawable_mesh.h>

#include <vclib/qt/viewer_widget.h>

#include <QApplication>

int main(int argc, char** argv)
{
#if VCLIB_RENDER_EXAMPLES_WITH_QT
    QApplication application(argc, argv);
#endif

    auto viewer = defaultViewer();

    const bool TEXCOORDS_PER_VERTEX = false;
    const bool USE_BUNNY = true;

    if constexpr (TEXCOORDS_PER_VERTEX) {
        vcl::DrawableMesh<vcl::TriMesh> drawable =
            getDrawableMesh<vcl::TriMesh>("VertTextureDouble.ply");
        auto mrs = drawable.renderSettings();
        mrs.setSurfaceShadingFlat();
        mrs.setSurfaceColorPerVertexTexcoords();
        drawable.setRenderSettings(mrs);
        showMeshesOnViewer(argc, argv, viewer, drawable);
    }
    else if constexpr(USE_BUNNY)
    {
        vcl::DrawableMesh<vcl::TriMesh> drawable =
            getDrawableMesh<vcl::TriMesh>("bunny_textured.ply");
        auto mrs = drawable.renderSettings();
        mrs.setSurfaceColorPerWedgeTexcoords();
        drawable.setRenderSettings(mrs);
        showMeshesOnViewer(argc, argv, viewer, drawable);
    }
    else{
        vcl::DrawableMesh<vcl::TriMesh> drawable =
            getDrawableMesh<vcl::TriMesh>("TextureDouble.obj");
        auto mrs = drawable.renderSettings();
        mrs.setSurfaceShadingFlat();
        mrs.setSurfaceColorPerWedgeTexcoords();
        drawable.setRenderSettings(mrs);
        showMeshesOnViewer(argc, argv, viewer, drawable);
    }

#if VCLIB_RENDER_EXAMPLES_WITH_QT
    viewer.showMaximized();
    return application.exec();
#else
    (void) argc; // unused
    (void) argv;
    return 0;
#endif
}
