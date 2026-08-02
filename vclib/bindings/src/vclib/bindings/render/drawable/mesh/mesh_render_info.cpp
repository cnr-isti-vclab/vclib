// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/drawable/mesh/mesh_render_info.h>
#include <vclib/render/drawable/mesh/mesh_render_info.h>

namespace vcl::bind {

void initMeshRenderInfo(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<MeshRenderInfo> mri(m, "MeshRenderInfo");

    py::enum_<MeshRenderInfo::Buffers>(mri, "Buffers")
        .value("VERTICES", MeshRenderInfo::Buffers::VERTICES)
        .value("VERT_NORMALS", MeshRenderInfo::Buffers::VERT_NORMALS)
        .value("VERT_COLORS", MeshRenderInfo::Buffers::VERT_COLORS)
        .value("VERT_TEXCOORDS", MeshRenderInfo::Buffers::VERT_TEXCOORDS)
        .value("VERT_TANGENT", MeshRenderInfo::Buffers::VERT_TANGENT)
        .value("TRIANGLES", MeshRenderInfo::Buffers::TRIANGLES)
        .value("TRI_NORMALS", MeshRenderInfo::Buffers::TRI_NORMALS)
        .value("TRI_COLORS", MeshRenderInfo::Buffers::TRI_COLORS)
        .value("WEDGE_TEXCOORDS", MeshRenderInfo::Buffers::WEDGE_TEXCOORDS)
        .value("WIREFRAME", MeshRenderInfo::Buffers::WIREFRAME)
        .value("EDGES", MeshRenderInfo::Buffers::EDGES)
        .value("EDGE_COLORS", MeshRenderInfo::Buffers::EDGE_COLORS)
        .value("EDGE_NORMALS", MeshRenderInfo::Buffers::EDGE_NORMALS)
        .value("TEXTURES", MeshRenderInfo::Buffers::TEXTURES)
        .value(
            "MESH_ADDITIONAL_DATA",
            MeshRenderInfo::Buffers::MESH_ADDITIONAL_DATA)
        .export_values();

    py::enum_<MeshRenderInfo::Primitive>(mri, "Primitive")
        .value("POINTS", MeshRenderInfo::Primitive::POINTS)
        .value("SURFACE", MeshRenderInfo::Primitive::SURFACE)
        .value("WIREFRAME", MeshRenderInfo::Primitive::WIREFRAME)
        .value("EDGES", MeshRenderInfo::Primitive::EDGES)
        .export_values();

    py::enum_<MeshRenderInfo::Points>(mri, "Points")
        .value("VISIBLE", MeshRenderInfo::Points::VISIBLE)
        .value("SHAPE_PIXEL", MeshRenderInfo::Points::SHAPE_PIXEL)
        .value("SHAPE_CIRCLE", MeshRenderInfo::Points::SHAPE_CIRCLE)
        .value("SHAPE_SPHERE", MeshRenderInfo::Points::SHAPE_SPHERE)
        .value("SHADING_NONE", MeshRenderInfo::Points::SHADING_NONE)
        .value("SHADING_VERT", MeshRenderInfo::Points::SHADING_VERT)
        .value("COLOR_VERTEX", MeshRenderInfo::Points::COLOR_VERTEX)
        .value("COLOR_MESH", MeshRenderInfo::Points::COLOR_MESH)
        .value("COLOR_USER", MeshRenderInfo::Points::COLOR_USER)
        .export_values();

    py::enum_<MeshRenderInfo::Surface>(mri, "Surface")
        .value("VISIBLE", MeshRenderInfo::Surface::VISIBLE)
        .value("SHADING_NONE", MeshRenderInfo::Surface::SHADING_NONE)
        .value("SHADING_FLAT", MeshRenderInfo::Surface::SHADING_FLAT)
        .value("SHADING_SMOOTH", MeshRenderInfo::Surface::SHADING_SMOOTH)
        .value(
            "SHADING_NORMAL_MAP", MeshRenderInfo::Surface::SHADING_NORMAL_MAP)
        .value("COLOR_VERTEX", MeshRenderInfo::Surface::COLOR_VERTEX)
        .value("COLOR_FACE", MeshRenderInfo::Surface::COLOR_FACE)
        .value("COLOR_VERTEX_TEX", MeshRenderInfo::Surface::COLOR_VERTEX_TEX)
        .value("COLOR_WEDGE_TEX", MeshRenderInfo::Surface::COLOR_WEDGE_TEX)
        .value("COLOR_MESH", MeshRenderInfo::Surface::COLOR_MESH)
        .value("COLOR_USER", MeshRenderInfo::Surface::COLOR_USER)
        .export_values();

    py::enum_<MeshRenderInfo::Wireframe>(mri, "Wireframe")
        .value("VISIBLE", MeshRenderInfo::Wireframe::VISIBLE)
        .value("SHADING_NONE", MeshRenderInfo::Wireframe::SHADING_NONE)
        .value("SHADING_VERT", MeshRenderInfo::Wireframe::SHADING_VERT)
        .value("COLOR_VERTEX", MeshRenderInfo::Wireframe::COLOR_VERTEX)
        .value("COLOR_MESH", MeshRenderInfo::Wireframe::COLOR_MESH)
        .value("COLOR_USER", MeshRenderInfo::Wireframe::COLOR_USER)
        .export_values();

    py::enum_<MeshRenderInfo::Edges>(mri, "Edges")
        .value("VISIBLE", MeshRenderInfo::Edges::VISIBLE)
        .value("SHADING_NONE", MeshRenderInfo::Edges::SHADING_NONE)
        .value("SHADING_FLAT", MeshRenderInfo::Edges::SHADING_FLAT)
        .value("SHADING_SMOOTH", MeshRenderInfo::Edges::SHADING_SMOOTH)
        .value("COLOR_VERTEX", MeshRenderInfo::Edges::COLOR_VERTEX)
        .value("COLOR_EDGE", MeshRenderInfo::Edges::COLOR_EDGE)
        .value("COLOR_MESH", MeshRenderInfo::Edges::COLOR_MESH)
        .value("COLOR_USER", MeshRenderInfo::Edges::COLOR_USER)
        .export_values();
}

} // namespace vcl::bind
