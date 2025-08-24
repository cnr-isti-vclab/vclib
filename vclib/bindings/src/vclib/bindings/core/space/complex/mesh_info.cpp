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

#include <vclib/bindings/core/space/complex/mesh_info.h>
#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>
#include <vclib/space/complex.h>

namespace vcl::bind {

void initMeshInfo(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using enum MeshInfo::MeshType;
    using enum MeshInfo::Element;
    using enum MeshInfo::Component;
    using enum MeshInfo::DataType;

    py::class_<MeshInfo> c(m, "MeshInfo");
    c.def(py::init<>());

    auto fun = []<MeshConcept MeshType>(
                   pybind11::class_<MeshInfo>& c, MeshType = MeshType()) {
        c.def(py::init<MeshType>());
    };
    defForAllMeshTypes(c, fun);

    py::enum_<MeshInfo::MeshType> mt(c, "MeshType");
    mt.value("TRIANGLE_MESH", TRIANGLE_MESH);
    mt.value("QUAD_MESH", QUAD_MESH);
    mt.value("POLYGON_MESH", POLYGON_MESH);
    mt.value("UNKNOWN", UNKNOWN);
    mt.export_values();

    py::enum_<MeshInfo::Element> el(c, "Element");
    el.value("VERTEX", VERTEX);
    el.value("FACE", FACE);
    el.value("EDGE", EDGE);
    el.value("MESH", MESH);
    el.export_values();

    py::enum_<MeshInfo::Component> cpt(c, "Component");
    cpt.value("POSITION", POSITION);
    cpt.value("VREFS", VREFS);
    cpt.value("NORMAL", NORMAL);
    cpt.value("COLOR", COLOR);
    cpt.value("QUALITY", QUALITY);
    cpt.value("TEXCOORD", TEXCOORD);
    cpt.value("WEDGE_TEXCOORDS", WEDGE_TEXCOORDS);
    cpt.value("CUSTOM_COMPONENTS", CUSTOM_COMPONENTS);
    cpt.value("TEXTURES", TEXTURES);
    cpt.export_values();

    py::enum_<MeshInfo::DataType> dt(c, "DataType");
    dt.value("CHAR", MeshInfo::DataType::CHAR);
    dt.value("UCHAR", MeshInfo::DataType::UCHAR);
    dt.value("SHORT", MeshInfo::DataType::SHORT);
    dt.value("USHORT", MeshInfo::DataType::USHORT);
    dt.value("INT", MeshInfo::DataType::INT);
    dt.value("UINT", MeshInfo::DataType::UINT);
    dt.value("FLOAT", MeshInfo::DataType::FLOAT);
    dt.value("DOUBLE", MeshInfo::DataType::DOUBLE);
    dt.value("NONE", MeshInfo::DataType::NONE);
    dt.export_values();

    py::class_<MeshInfo::CustomComponent> cc(c, "CustomComponent");
    cc.def(py::init<std::string, MeshInfo::DataType>());
    cc.def_readwrite("name", &MeshInfo::CustomComponent::name);
    cc.def_readwrite("type", &MeshInfo::CustomComponent::type);

    defCopy(c);

    c.def("clear", &MeshInfo::clear);
    c.def("is_empty", &MeshInfo::isEmpty);
    c.def("mesh_type", &MeshInfo::meshType);
    c.def("is_unknown_mesh", &MeshInfo::isUnkownMesh);
    c.def("is_triangle_mesh", &MeshInfo::isTriangleMesh);
    c.def("is_quad_mesh", &MeshInfo::isQuadMesh);
    c.def("is_polygon_mesh", &MeshInfo::isPolygonMesh);
    c.def("has_element", &MeshInfo::hasElement);
    c.def("has_per_element_component", &MeshInfo::hasPerElementComponent);

    c.def("has_vertices", &MeshInfo::hasVertices);
    c.def("has_per_vertex_position", &MeshInfo::hasPerVertexPosition);
    c.def("has_per_vertex_normal", &MeshInfo::hasPerVertexNormal);
    c.def("has_per_vertex_color", &MeshInfo::hasPerVertexColor);
    c.def("has_per_vertex_quality", &MeshInfo::hasPerVertexQuality);
    c.def("has_per_vertex_tex_coord", &MeshInfo::hasPerVertexTexCoord);
    c.def(
        "has_per_vertex_custom_components",
        &MeshInfo::hasPerVertexCustomComponents);
    c.def("has_faces", &MeshInfo::hasFaces);
    c.def(
        "has_per_face_vertex_references",
        &MeshInfo::hasPerFaceVertexReferences);
    c.def("has_per_face_normal", &MeshInfo::hasPerFaceNormal);
    c.def("has_per_face_color", &MeshInfo::hasPerFaceColor);
    c.def("has_per_face_quality", &MeshInfo::hasPerFaceQuality);
    c.def("has_per_face_wedge_tex_coords", &MeshInfo::hasPerFaceWedgeTexCoords);
    c.def(
        "has_per_face_custom_components",
        &MeshInfo::hasPerFaceCustomComponents);
    c.def("has_edges", &MeshInfo::hasEdges);
    c.def(
        "has_per_edge_vertex_references",
        &MeshInfo::hasPerEdgeVertexReferences);
    c.def("has_per_edge_color", &MeshInfo::hasPerEdgeColor);
    c.def("has_per_edge_normal", &MeshInfo::hasPerEdgeNormal);
    c.def("has_per_edge_quality", &MeshInfo::hasPerEdgeQuality);
    c.def(
        "has_per_edge_custom_components",
        &MeshInfo::hasPerEdgeCustomComponents);
    c.def("has_textures", &MeshInfo::hasTextures);

    c.def("update_mesh_type", &MeshInfo::updateMeshType);

    c.def("set_unknown_mesh", &MeshInfo::setUnknownMesh);
    c.def("set_triangle_mesh", &MeshInfo::setTriangleMesh);
    c.def("set_quad_mesh", &MeshInfo::setQuadMesh);
    c.def("set_polygon_mesh", &MeshInfo::setPolygonMesh);
    c.def("set_mesh_type", &MeshInfo::setMeshType);
    c.def("set_element", &MeshInfo::setElement, "el"_a, "b"_a = true);
    c.def("set_per_element_component", &MeshInfo::setPerElementComponent);
    c.def("set_vertices", &MeshInfo::setVertices, "b"_a = true);
    c.def(
        "set_per_vertex_position",
        &MeshInfo::setPerVertexPosition,
        "b"_a = true,
        "t"_a = DOUBLE);
    c.def(
        "set_per_vertex_normal",
        &MeshInfo::setPerVertexNormal,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_per_vertex_color",
        &MeshInfo::setPerVertexColor,
        "b"_a = true,
        "t"_a = UCHAR);
    c.def(
        "set_per_vertex_quality",
        &MeshInfo::setPerVertexQuality,
        "b"_a = true,
        "t"_a = DOUBLE);
    c.def(
        "set_per_vertex_tex_coord",
        &MeshInfo::setPerVertexTexCoord,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_per_vertex_custom_components",
        &MeshInfo::setPerVertexCustomComponents,
        "b"_a = true);
    c.def("set_faces", &MeshInfo::setFaces, "b"_a = true);
    c.def(
        "set_per_face_vertex_references",
        &MeshInfo::setPerFaceVertexReferences,
        "b"_a = true);
    c.def(
        "set_per_face_normal",
        &MeshInfo::setPerFaceNormal,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_per_face_color",
        &MeshInfo::setPerFaceColor,
        "b"_a = true,
        "t"_a = UCHAR);
    c.def(
        "set_per_face_quality",
        &MeshInfo::setPerFaceQuality,
        "b"_a = true,
        "t"_a = DOUBLE);
    c.def(
        "set_per_face_wedge_tex_coords",
        &MeshInfo::setPerFaceWedgeTexCoords,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_per_face_custom_components",
        &MeshInfo::setPerFaceCustomComponents,
        "b"_a = true);
    c.def("set_edges", &MeshInfo::setEdges, "b"_a = true);
    c.def(
        "set_per_edge_vertex_references",
        &MeshInfo::setPerEdgeVertexReferences,
        "b"_a = true);
    c.def(
        "set_per_edge_color",
        &MeshInfo::setPerEdgeColor,
        "b"_a = true,
        "t"_a = UCHAR);
    c.def(
        "set_per_edge_normal",
        &MeshInfo::setPerEdgeNormal,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_per_edge_quality",
        &MeshInfo::setPerEdgeQuality,
        "b"_a = true,
        "t"_a = QUALITY);
    c.def(
        "set_per_edge_custom_components",
        &MeshInfo::setPerEdgeCustomComponents,
        "b"_a = true);
    c.def("set_textures", &MeshInfo::setTextures, "b"_a = true);

    c.def(
        "add_per_element_custom_component",
        &MeshInfo::addPerElementCustomComponent);
    c.def(
        "clear_per_element_custom_components",
        &MeshInfo::clearPerElementCustomComponents);
    c.def(
        "add_per_vertex_custom_component",
        &MeshInfo::addPerVertexCustomComponent);
    c.def(
        "clear_per_vertex_custom_components",
        &MeshInfo::clearPerVertexCustomComponents);
    c.def(
        "add_per_face_custom_component", &MeshInfo::addPerFaceCustomComponent);
    c.def(
        "clear_per_face_custom_components",
        &MeshInfo::clearPerFaceCustomComponents);
    c.def(
        "add_per_edge_custom_component", &MeshInfo::addPerEdgeCustomComponent);
    c.def(
        "clear_per_edge_custom_components",
        &MeshInfo::clearPerEdgeCustomComponents);

    c.def("per_element_component_type", &MeshInfo::perElementComponentType);
    c.def("per_vertex_position_type", &MeshInfo::perVertexPositionType);
    c.def("per_vertex_normal_type", &MeshInfo::perVertexNormalType);
    c.def("per_vertex_color_type", &MeshInfo::perVertexColorType);
    c.def("per_vertex_quality_type", &MeshInfo::perVertexQualityType);
    c.def("per_vertex_tex_coord_type", &MeshInfo::perVertexTexCoordType);
    c.def("per_face_normal_type", &MeshInfo::perFaceNormalType);
    c.def("per_face_color_type", &MeshInfo::perFaceColorType);
    c.def("per_face_quality_type", &MeshInfo::perFaceQualityType);
    c.def(
        "per_face_wedge_tex_coords_type", &MeshInfo::perFaceWedgeTexCoordsType);
    c.def("per_edge_normal_type", &MeshInfo::perEdgeNormalType);
    c.def("per_edge_color_type", &MeshInfo::perEdgeColorType);
    c.def("per_edge_quality_type", &MeshInfo::perEdgeQualityType);

    c.def(
        "per_element_custom_components", &MeshInfo::perElementCustomComponents);
    c.def("per_vertex_custom_components", &MeshInfo::perVertexCustomComponents);
    c.def("per_face_custom_components", &MeshInfo::perFaceCustomComponents);
    c.def("per_edge_custom_components", &MeshInfo::perEdgeCustomComponents);

    c.def("intersect", &MeshInfo::intersect);
}

} // namespace vcl::bind
