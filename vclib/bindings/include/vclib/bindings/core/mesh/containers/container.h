// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_CONTAINERS_CONTAINER_H
#define VCL_BINDINGS_CORE_MESH_CONTAINERS_CONTAINER_H

#include "component_view.h"

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

namespace detail {

template<
    ElemId      ELEM_ID,
    CompId      COMP_ID,
    MeshConcept MeshType,
    typename... Options>
void addOptionalComponentFunctions(
    pybind11::class_<MeshType, Options...>& c,
    const std::string&                      name,
    const std::string&                      compName)
{
    if constexpr (MeshType::template hasElementOptionalComponent<
                      ELEM_ID,
                      COMP_ID>()) {
        c.def(
            ("is_" + name + "_" + compName + "_available").c_str(),
            [](const MeshType& t) {
                return t
                    .template isElementComponentAvailable<ELEM_ID, COMP_ID>();
            });
        c.def(("enable_" + name + "_" + compName).c_str(), [](MeshType& t) {
            t.template enableElementComponent<ELEM_ID, COMP_ID>();
        });
        c.def(("disable_" + name + "_" + compName).c_str(), [](MeshType& t) {
            t.template disableElementComponent<ELEM_ID, COMP_ID>();
        });
    }
}

} // namespace detail

template<ElementConcept Element, MeshConcept MeshType, typename... Options>
void initContainer(
    pybind11::class_<MeshType, Options...>& c,
    const std::string&                      name,
    std::string                             namePlural = "")
{
    namespace py = pybind11;

    static const ElemId ELEM_ID = Element::ELEMENT_ID;

    std::string capitalName = name;
    capitalName[0]          = std::toupper(capitalName[0]);

    if (namePlural.empty())
        namePlural = name + "s";

    c.def(
        name.c_str(),
        [](MeshType& t, uint i) -> Element {
            if (i >= t.template containerSize<ELEM_ID>()) {
                throw py::index_error();
            }
            return t.template element<ELEM_ID>(i);
        },
        py::keep_alive<0, 1>());

    c.def((name + "_count").c_str(), [](const MeshType& t) {
        return t.template count<ELEM_ID>();
    });
    c.def((name + "_container_size").c_str(), [](const MeshType& t) {
        return t.template containerSize<ELEM_ID>();
    });
    c.def(("deleted_" + name + "_count").c_str(), [](const MeshType& t) {
        return t.template deletedCount<ELEM_ID>();
    });

    c.def(("add_" + name).c_str(), [](MeshType& t) {
        return t.template add<ELEM_ID>();
    });

    c.def(("add_" + namePlural).c_str(), [](MeshType& t, uint n) {
        return t.template add<ELEM_ID>(n);
    });

    c.def(("clear_" + namePlural).c_str(), [](MeshType& t) {
        t.template clearElements<ELEM_ID>();
    });
    c.def(("resize_" + namePlural).c_str(), [](MeshType& t, uint n) {
        t.template resize<ELEM_ID>(n);
    });
    c.def(("reserve_" + namePlural).c_str(), [](MeshType& t, uint n) {
        t.template reserve<ELEM_ID>(n);
    });

    c.def(("compact_" + namePlural).c_str(), [](MeshType& t) {
        return t.template compactElements<ELEM_ID>();
    });

    c.def(("compact_" + name + "_indices").c_str(), [](const MeshType& t) {
        return t.template compactIndices<ELEM_ID>();
    });

    c.def(("delete_" + name).c_str(), [](MeshType& t, uint i) {
        t.template deleteElement<ELEM_ID>(i);
    });
    c.def(("delete_" + name).c_str(), [](MeshType& t, const Element& e) {
        t.template deleteElement<ELEM_ID>(e.index());
    });

    using ElemView =
        decltype(std::declval<MeshType&>().template elements<ELEM_ID>(true));

    if (!registeredTypes.contains(typeid(ElemView))) {
        // inner class that allows to iterate over elements
        pybind11::class_<ElemView> v(c, ("_" + capitalName + "Range").c_str());

        v.def(
            "__iter__",
            [](ElemView& t) { return py::make_iterator(t.begin(), t.end()); },
            py::keep_alive<0, 1>());

        v.def("__len__", [](const ElemView& t) { return t.size(); });

        v.def(
            "__getitem__",
            [](ElemView& t, int i) {
                int s = static_cast<int>(t.size());
                if (i < 0) {
                    i += s;
                }
                if (i < 0 || i >= s) {
                    throw py::index_error();
                }
                return *(t.begin() + i);
            },
            py::keep_alive<0, 1>());

        v.def("__bool__", [](const ElemView& t) { return !t.empty(); });
        v.def("is_compact", [](const ElemView& t) { return t.isCompact(); });

        using Container = typename ElemView::ContainerType;

        if constexpr (comp::HasPosition<Container>) {
            initComponentView<CompId::POSITION, Container>(
                c, "_" + capitalName + "PositionsView");
            v.def(
                "positions",
                [](ElemView& self) { return self.positions(); },
                py::keep_alive<0, 1>());
        }

        if constexpr (comp::HasNormal<Container>) {
            initComponentView<CompId::NORMAL, Container>(
                c, "_" + capitalName + "NormalsView");
            v.def(
                "normals",
                [](ElemView& self) { return self.normals(); },
                py::keep_alive<0, 1>());
        }

        if constexpr (comp::HasColor<Container>) {
            initComponentView<CompId::COLOR, Container>(
                c, "_" + capitalName + "ColorsView");
            v.def(
                "colors",
                [](ElemView& self) { return self.colors(); },
                py::keep_alive<0, 1>());
        }

        if constexpr (comp::HasQuality<Container>) {
            initComponentView<CompId::QUALITY, Container>(
                c, "_" + capitalName + "QualitiesView");
            v.def(
                "qualities",
                [](ElemView& self) { return self.qualities(); },
                py::keep_alive<0, 1>());
        }

        if constexpr (comp::HasTexCoord<Container>) {
            initComponentView<CompId::TEX_COORD, Container>(
                c, "_" + capitalName + "TexCoordsView");
            v.def(
                "tex_coords",
                [](ElemView& self) { return self.texCoords(); },
                py::keep_alive<0, 1>());
        }

        if constexpr (comp::HasTangent<Container>) {
            initComponentView<CompId::TANGENT, Container>(
                c, "_" + capitalName + "TangentsView");
            v.def(
                "tangents",
                [](ElemView& self) { return self.tangents(); },
                py::keep_alive<0, 1>());
        }

        if constexpr (comp::HasPrincipalCurvature<Container>) {
            initComponentView<CompId::PRINCIPAL_CURVATURE, Container>(
                c, "_" + capitalName + "PrincipalCurvaturesView");
            v.def(
                "principal_curvatures",
                [](ElemView& self) { return self.principalCurvatures(); },
                py::keep_alive<0, 1>());
        }

        if constexpr (comp::HasMaterialIndex<Container>) {
            initComponentView<CompId::MATERIAL_INDEX, Container>(
                c, "_" + capitalName + "MaterialIndicesView");
            v.def(
                "material_indices",
                [](ElemView& self) { return self.materialIndices(); },
                py::keep_alive<0, 1>());
        }

        if constexpr (comp::HasStaticVertexReferences<Container>) {
            initComponentView<CompId::VERTEX_REFERENCES, Container>(
                c, "_" + capitalName + "VertexIndicesView");
            v.def(
                "vertex_indices",
                [](ElemView& self) { return self.vertexIndices(); },
                py::keep_alive<0, 1>());
        }

        registeredTypes.insert(typeid(ElemView));
    }

    c.def(
        namePlural.c_str(),
        [](MeshType& t, bool jumpDeleted) {
            return t.template elements<ELEM_ID>(jumpDeleted);
        },
        py::arg("jump_deleted") = true,
        py::keep_alive<0, 1>());

    // optional components

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::ADJACENT_EDGES>(
        c, name, "adjacent_edges");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::ADJACENT_FACES>(
        c, name, "adjacent_faces");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::ADJACENT_VERTICES>(
        c, name, "adjacent_vertices");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::COLOR>(
        c, name, "color");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::MATERIAL_INDEX>(
        c, name, "material_index");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::NORMAL>(
        c, name, "normal");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::PRINCIPAL_CURVATURE>(
        c, name, "principal_curvature");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::QUALITY>(
        c, name, "quality");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::TANGENT>(
        c, name, "tangent");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::TEX_COORD>(
        c, name, "tex_coord");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::WEDGE_COLORS>(
        c, name, "wedge_colors");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::WEDGE_NORMALS>(
        c, name, "wedge_normals");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::WEDGE_TEX_COORDS>(
        c, name, "wedge_tex_coords");
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_CONTAINERS_CONTAINER_H
