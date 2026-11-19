// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_H

#include "components/adjacent_edges.h"
#include "components/adjacent_faces.h"
#include "components/adjacent_vertices.h"
#include "components/bit_flags.h"
#include "components/materials.h"
#include "components/polygon_bit_flags.h"
#include "components/triangle_bit_flags.h"
#include "components/vertex_references.h"

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <pybind11/pybind11.h>

#include <stdexcept>

namespace vcl::bind {

template<typename T>
concept ElementOrMeshConcept = ElementConcept<T> || MeshConcept<T>;

template<CompId COMP_ID, typename ElementType>
void checkElementComponent(const ElementType& el)
{
    if constexpr (ElementConcept<ElementType>) {
        if (!el.isValid()) {
            throw std::runtime_error(
                "Cannot access component on an invalid or null element proxy.");
        }
        if (!el.template isComponentAvailable<COMP_ID>()) {
            throw MissingComponentException(
                std::string("Component '") +
                vcl::componentEnumString<COMP_ID>() +
                "' is not available on this element.");
        }
    }
}

template<ElementOrMeshConcept ElementType, typename... Options>
void initComponents(pybind11::class_<ElementType, Options...>& c)
{
    namespace py = pybind11;
    using enum py::return_value_policy;

    if constexpr (comp::HasAdjacentEdges<ElementType>) {
        initAdjacentEdges(c);
    }
    if constexpr (comp::HasAdjacentFaces<ElementType>) {
        initAdjacentFaces(c);
    }
    if constexpr (comp::HasAdjacentVertices<ElementType>) {
        initAdjacentVertices(c);
    }

    // bit flags
    if constexpr (comp::HasTriangleBitFlags<ElementType>) {
        initTriangleBitFlags(c);
    }
    else if constexpr (comp::HasPolygonBitFlags<ElementType>) {
        initPolygonBitFlags(c);
    }
    else if constexpr (comp::HasBitFlags<ElementType>) {
        initBitFlags(c);
    }

    if constexpr (HasBoundingBox<ElementType>) {
        c.def(
            "bounding_box",
            [](ElementType& v) -> auto& { return v.boundingBox(); },
            reference_internal);
        c.def("set_bounding_box", [](ElementType& v, const Box3d& b) {
            v.boundingBox() = b;
        });
    }
    if constexpr (comp::HasColor<ElementType> || HasMeshColor<ElementType>) {
        c.def(
            "color",
            [](ElementType& v) -> auto& {
                checkElementComponent<CompId::COLOR>(v);
                return v.color();
            },
            reference_internal);
        c.def("set_color", [](ElementType& v, const Color& c) {
            checkElementComponent<CompId::COLOR>(v);
            v.color() = c;
        });
    }
    if constexpr (comp::HasPosition<ElementType>) {
        c.def(
            "position",
            [](ElementType& v) -> auto& {
                checkElementComponent<CompId::POSITION>(v);
                return v.position();
            },
            reference_internal);
        c.def("set_position", [](ElementType& v, const Point3d& p) {
            checkElementComponent<CompId::POSITION>(v);
            v.position() = p;
        });
    }
    if constexpr (comp::HasMaterialIndex<ElementType>) {
        c.def("material_index", [](const ElementType& v) {
            checkElementComponent<CompId::MATERIAL_INDEX>(v);
            return v.materialIndex();
        });
        c.def("set_material_index", [](ElementType& v, ushort mi) {
            checkElementComponent<CompId::MATERIAL_INDEX>(v);
            v.materialIndex() = mi;
        });
    }
    if constexpr (HasName<ElementType>) {
        c.def("name", [](const ElementType& v) {
            if constexpr (ElementConcept<ElementType>) {
                if (!v.isValid()) {
                    throw std::runtime_error(
                        "Cannot access name on an invalid or null element "
                        "proxy.");
                }
            }
            return v.name();
        });
        c.def("set_name", [](ElementType& v, const std::string& n) {
            if constexpr (ElementConcept<ElementType>) {
                if (!v.isValid()) {
                    throw std::runtime_error(
                        "Cannot access name on an invalid or null element "
                        "proxy.");
                }
            }
            v.name() = n;
        });
    }
    if constexpr (comp::HasNormal<ElementType>) {
        c.def(
            "normal",
            [](ElementType& v) -> auto& {
                checkElementComponent<CompId::NORMAL>(v);
                return v.normal();
            },
            reference_internal);
        c.def("set_normal", [](ElementType& v, const Point3d& p) {
            checkElementComponent<CompId::NORMAL>(v);
            v.normal() = p;
        });
    }
    if constexpr (comp::HasPrincipalCurvature<ElementType>) {
        c.def(
            "principal_curvature",
            [](ElementType& v) -> auto& {
                checkElementComponent<CompId::PRINCIPAL_CURVATURE>(v);
                return v.principalCurvature();
            },
            reference_internal);
        c.def(
            "set_principal_curvature",
            [](ElementType& v, const PrincipalCurvatured& p) {
                checkElementComponent<CompId::PRINCIPAL_CURVATURE>(v);
                v.principalCurvature() = p;
            });
    }

    if constexpr (comp::HasQuality<ElementType>) {
        c.def("quality", [](const ElementType& v) {
            checkElementComponent<CompId::QUALITY>(v);
            return v.quality();
        });
        c.def("set_quality", [](ElementType& v, double q) {
            checkElementComponent<CompId::QUALITY>(v);
            v.quality() = q;
        });
    }
    if constexpr (comp::HasTangent<ElementType>) {
        c.def(
            "tangent",
            [](ElementType& v) -> auto& {
                checkElementComponent<CompId::TANGENT>(v);
                return v.tangent();
            },
            reference_internal);
        c.def("set_tangent", [](ElementType& v, const Point3d& p) {
            checkElementComponent<CompId::TANGENT>(v);
            v.tangent() = p;
        });
        c.def("tangent_right_handed", [](const ElementType& v) {
            checkElementComponent<CompId::TANGENT>(v);
            return v.tangentRightHanded();
        });
        c.def("set_tangent_right_handed", [](ElementType& v, bool rh) {
            checkElementComponent<CompId::TANGENT>(v);
            v.tangentRightHanded() = rh;
        });
        c.def("bitangent", [](ElementType& v) {
            checkElementComponent<CompId::TANGENT>(v);
            checkElementComponent<CompId::NORMAL>(v);
            return v.bitangent();
        });
    }
    if constexpr (comp::HasTexCoord<ElementType>) {
        c.def(
            "tex_coord",
            [](ElementType& v) -> auto& {
                checkElementComponent<CompId::TEX_COORD>(v);
                return v.texCoord();
            },
            reference_internal);
        c.def("set_tex_coord", [](ElementType& v, const TexCoordd& t) {
            checkElementComponent<CompId::TEX_COORD>(v);
            v.texCoord() = t;
        });
    }

    if constexpr (HasMaterials<ElementType>) {
        initMaterials(c);
    }

    if constexpr (HasTransformMatrix<ElementType>) {
        c.def(
            "transform_matrix",
            [](ElementType& v) -> auto& { return v.transformMatrix(); },
            reference_internal);
        c.def("set_transform_matrix", [](ElementType& v, const Matrix44d& m) {
            v.transformMatrix() = m;
        });
    }

    if constexpr (comp::HasVertexReferences<ElementType>) {
        initVertexReferences(c);
    }
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_H
