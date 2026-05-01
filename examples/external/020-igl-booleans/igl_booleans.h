#ifndef IGL_BOOLEANS_H
#define IGL_BOOLEANS_H

#include <vclib/igl/booleans.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

auto meshBooleans()
{
    vcl::TriMesh m1;
    vcl::loadMesh(m1, VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

    vcl::TriMesh m2;
    vcl::loadMesh(m2, VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    vcl::TriMesh res =
        vcl::igl::meshBoolean(m1, m2, igl::MESH_BOOLEAN_TYPE_UNION);

    return std::make_tuple(m1, m2, res);
}

#endif // IGL_BOOLEANS_H
