// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
    cpt.value("MATERIAL_INDEX", MATERIAL_INDEX);
    cpt.value("WEDGE_TEXCOORDS", WEDGE_TEXCOORDS);
    cpt.value("CUSTOM_COMPONENTS", CUSTOM_COMPONENTS);
    cpt.value("MATERIALS", MATERIALS);
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
    c.def("has_element_component", &MeshInfo::hasElementComponent);

    c.def("has_vertices", &MeshInfo::hasVertices);
    c.def("has_vertex_position", &MeshInfo::hasVertexPosition);
    c.def("has_vertex_normal", &MeshInfo::hasVertexNormal);
    c.def("has_vertex_color", &MeshInfo::hasVertexColor);
    c.def("has_vertex_quality", &MeshInfo::hasVertexQuality);
    c.def("has_vertex_tex_coord", &MeshInfo::hasVertexTexCoord);
    c.def(
        "has_vertex_material_index", &MeshInfo::hasVertexMaterialIndex);
    c.def(
        "has_vertex_custom_components",
        &MeshInfo::hasVertexCustomComponents);
    c.def("has_faces", &MeshInfo::hasFaces);
    c.def(
        "has_face_vertex_references",
        &MeshInfo::hasFaceVertexReferences);
    c.def("has_face_normal", &MeshInfo::hasFaceNormal);
    c.def("has_face_color", &MeshInfo::hasFaceColor);
    c.def("has_face_quality", &MeshInfo::hasFaceQuality);
    c.def("has_face_wedge_tex_coords", &MeshInfo::hasFaceWedgeTexCoords);
    c.def("has_face_material_index", &MeshInfo::hasFaceMaterialIndex);
    c.def(
        "has_face_custom_components",
        &MeshInfo::hasFaceCustomComponents);
    c.def("has_edges", &MeshInfo::hasEdges);
    c.def(
        "has_edge_vertex_references",
        &MeshInfo::hasEdgeVertexReferences);
    c.def("has_edge_color", &MeshInfo::hasEdgeColor);
    c.def("has_edge_normal", &MeshInfo::hasEdgeNormal);
    c.def("has_edge_quality", &MeshInfo::hasEdgeQuality);
    c.def(
        "has_edge_custom_components",
        &MeshInfo::hasEdgeCustomComponents);
    c.def("has_materials", &MeshInfo::hasMaterials);

    c.def("update_mesh_type", &MeshInfo::updateMeshType);

    c.def("set_unknown_mesh", &MeshInfo::setUnknownMesh);
    c.def("set_triangle_mesh", &MeshInfo::setTriangleMesh);
    c.def("set_quad_mesh", &MeshInfo::setQuadMesh);
    c.def("set_polygon_mesh", &MeshInfo::setPolygonMesh);
    c.def("set_mesh_type", &MeshInfo::setMeshType);
    c.def("set_element", &MeshInfo::setElement, "el"_a, "b"_a = true);
    c.def("set_element_component", &MeshInfo::setElementComponent);
    c.def("set_vertices", &MeshInfo::setVertices, "b"_a = true);
    c.def(
        "set_vertex_position",
        &MeshInfo::setVertexPosition,
        "b"_a = true,
        "t"_a = DOUBLE);
    c.def(
        "set_vertex_normal",
        &MeshInfo::setVertexNormal,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_vertex_color",
        &MeshInfo::setVertexColor,
        "b"_a = true,
        "t"_a = UCHAR);
    c.def(
        "set_vertex_quality",
        &MeshInfo::setVertexQuality,
        "b"_a = true,
        "t"_a = DOUBLE);
    c.def(
        "set_vertex_tex_coord",
        &MeshInfo::setVertexTexCoord,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_vertex_material_index",
        &MeshInfo::setVertexMaterialIndex,
        "b"_a = true,
        "t"_a = USHORT);
    c.def(
        "set_vertex_custom_components",
        &MeshInfo::setVertexCustomComponents,
        "b"_a = true);
    c.def("set_faces", &MeshInfo::setFaces, "b"_a = true);
    c.def(
        "set_face_vertex_references",
        &MeshInfo::setFaceVertexReferences,
        "b"_a = true);
    c.def(
        "set_face_normal",
        &MeshInfo::setFaceNormal,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_face_color",
        &MeshInfo::setFaceColor,
        "b"_a = true,
        "t"_a = UCHAR);
    c.def(
        "set_face_quality",
        &MeshInfo::setFaceQuality,
        "b"_a = true,
        "t"_a = DOUBLE);
    c.def(
        "set_face_wedge_tex_coords",
        &MeshInfo::setFaceWedgeTexCoords,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_face_material_index",
        &MeshInfo::setFaceMaterialIndex,
        "b"_a = true,
        "t"_a = USHORT);
    c.def(
        "set_face_custom_components",
        &MeshInfo::setFaceCustomComponents,
        "b"_a = true);
    c.def("set_edges", &MeshInfo::setEdges, "b"_a = true);
    c.def(
        "set_edge_vertex_references",
        &MeshInfo::setEdgeVertexReferences,
        "b"_a = true);
    c.def(
        "set_edge_color",
        &MeshInfo::setEdgeColor,
        "b"_a = true,
        "t"_a = UCHAR);
    c.def(
        "set_edge_normal",
        &MeshInfo::setEdgeNormal,
        "b"_a = true,
        "t"_a = FLOAT);
    c.def(
        "set_edge_quality",
        &MeshInfo::setEdgeQuality,
        "b"_a = true,
        "t"_a = QUALITY);
    c.def(
        "set_edge_custom_components",
        &MeshInfo::setEdgeCustomComponents,
        "b"_a = true);
    c.def("set_materials", &MeshInfo::setMaterials, "b"_a = true);

    c.def(
        "add_element_custom_component",
        &MeshInfo::addElementCustomComponent);
    c.def(
        "clear_element_custom_components",
        &MeshInfo::clearElementCustomComponents);
    c.def(
        "add_vertex_custom_component",
        &MeshInfo::addVertexCustomComponent);
    c.def(
        "clear_vertex_custom_components",
        &MeshInfo::clearVertexCustomComponents);
    c.def(
        "add_face_custom_component", &MeshInfo::addFaceCustomComponent);
    c.def(
        "clear_face_custom_components",
        &MeshInfo::clearFaceCustomComponents);
    c.def(
        "add_edge_custom_component", &MeshInfo::addEdgeCustomComponent);
    c.def(
        "clear_edge_custom_components",
        &MeshInfo::clearEdgeCustomComponents);

    c.def("element_component_type", &MeshInfo::elementComponentType);
    c.def("vertex_position_type", &MeshInfo::vertexPositionType);
    c.def("vertex_normal_type", &MeshInfo::vertexNormalType);
    c.def("vertex_color_type", &MeshInfo::vertexColorType);
    c.def("vertex_quality_type", &MeshInfo::vertexQualityType);
    c.def("vertex_tex_coord_type", &MeshInfo::vertexTexCoordType);
    c.def(
        "vertex_material_index_type",
        &MeshInfo::vertexMaterialIndexType);
    c.def("face_normal_type", &MeshInfo::faceNormalType);
    c.def("face_color_type", &MeshInfo::faceColorType);
    c.def("face_quality_type", &MeshInfo::faceQualityType);
    c.def(
        "face_wedge_tex_coords_type", &MeshInfo::faceWedgeTexCoordsType);
    c.def("face_material_index_type", &MeshInfo::faceMaterialIndexType);
    c.def("edge_normal_type", &MeshInfo::edgeNormalType);
    c.def("edge_color_type", &MeshInfo::edgeColorType);
    c.def("edge_quality_type", &MeshInfo::edgeQualityType);

    c.def(
        "element_custom_components", &MeshInfo::elementCustomComponents);
    c.def("vertex_custom_components", &MeshInfo::vertexCustomComponents);
    c.def("face_custom_components", &MeshInfo::faceCustomComponents);
    c.def("edge_custom_components", &MeshInfo::edgeCustomComponents);

    c.def("intersect", &MeshInfo::intersect);
}

} // namespace vcl::bind
