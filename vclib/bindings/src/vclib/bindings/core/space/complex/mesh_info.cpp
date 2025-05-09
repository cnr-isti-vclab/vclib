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

#include <vclib/space/complex.h>

namespace vcl::bind {

void initMeshInfo(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<MeshInfo> c(m, "MeshInfo");
    c.def(py::init<>());

    auto fun = []<MeshConcept MeshType>(
                   pybind11::class_<MeshInfo>& c, MeshType = MeshType()) {
        c.def(py::init<MeshType>());
    };
    defForAllMeshTypes(c, fun);

    py::enum_<MeshInfo::MeshType> mt(c, "MeshType");
    mt.value("TRIANGLE_MESH", MeshInfo::MeshType::TRIANGLE_MESH);
    mt.value("QUAD_MESH", MeshInfo::MeshType::QUAD_MESH);
    mt.value("POLYGON_MESH", MeshInfo::MeshType::POLYGON_MESH);
    mt.value("UNKNOWN", MeshInfo::MeshType::UNKNOWN);
    mt.export_values();

    py::enum_<MeshInfo::Element> el(c, "Element");
    el.value("VERTEX", MeshInfo::Element::VERTEX);
    el.value("FACE", MeshInfo::Element::FACE);
    el.value("EDGE", MeshInfo::Element::EDGE);
    el.value("MESH", MeshInfo::Element::MESH);
    el.export_values();

    py::enum_<MeshInfo::Component> cpt(c, "Component");
    cpt.value("COORD", MeshInfo::Component::COORD);
    cpt.value("VREFS", MeshInfo::Component::VREFS);
    cpt.value("NORMAL", MeshInfo::Component::NORMAL);
    cpt.value("COLOR", MeshInfo::Component::COLOR);
    cpt.value("QUALITY", MeshInfo::Component::QUALITY);
    cpt.value("TEXCOORD", MeshInfo::Component::TEXCOORD);
    cpt.value("WEDGE_TEXCOORDS", MeshInfo::Component::WEDGE_TEXCOORDS);
    cpt.value("CUSTOM_COMPONENTS", MeshInfo::Component::CUSTOM_COMPONENTS);
    cpt.value("TEXTURES", MeshInfo::Component::TEXTURES);
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
    c.def("has_per_vertex_coordinate", &MeshInfo::hasPerVertexCoordinate);
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
    c.def("has_per_face_custom_components", &MeshInfo::hasPerFaceCustomComponents);
    c.def("has_edges", &MeshInfo::hasEdges);
    c.def("has_per_edge_vertex_references", &MeshInfo::hasPerEdgeVertexReferences);
    c.def("has_per_edge_color", &MeshInfo::hasPerEdgeColor);
    c.def("has_per_edge_normal", &MeshInfo::hasPerEdgeNormal);
    c.def("has_per_edge_quality", &MeshInfo::hasPerEdgeQuality);
    c.def("has_per_edge_custom_components", &MeshInfo::hasPerEdgeCustomComponents);
    c.def("has_textures", &MeshInfo::hasTextures);

    c.def("update_mesh_type", &MeshInfo::updateMeshType);
    c.def("set_unknown_mesh", &MeshInfo::setUnknownMesh);
    c.def("set_triangle_mesh", &MeshInfo::setTriangleMesh);
    c.def("set_quad_mesh", &MeshInfo::setQuadMesh);
    c.def("set_polygon_mesh", &MeshInfo::setPolygonMesh);
    c.def("set_mesh_type", &MeshInfo::setMeshType);
    c.def("set_element", &MeshInfo::setElement);
    c.def("set_per_element_component", &MeshInfo::setPerElementComponent);
    c.def("add_per_element_custom_component", &MeshInfo::addPerElementCustomComponent);
    c.def(
        "clear_per_element_custom_components",
        &MeshInfo::clearPerElementCustomComponents);

    c.def("per_element_component_type", &MeshInfo::perElementComponentType);

    c.def("intersect", &MeshInfo::intersect);
}

} // namespace vcl::bind
