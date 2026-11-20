// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_CHECK_REFERENCES_H
#define VCL_ALGORITHMS_MESH_CHECK_REFERENCES_H

#include <vclib/mesh.h>

#include <string>

namespace vcl {

namespace detail {

template<MeshConcept MeshType, typename TargetCont, typename SourceCont>
void checkContainerReferences(const MeshType& mesh)
{
    constexpr ElemId TARGET_ELEM_ID = TargetCont::ELEMENT_ID;
    constexpr ElemId SOURCE_ELEM_ID = SourceCont::ELEMENT_ID;
    using SourceElem                = typename SourceCont::ElementType;
    using SourceComponents          = typename SourceElem::Components;

    auto checkComp = [&]<typename Comp>() {
        if constexpr (comp::HasReferencesTo<TARGET_ELEM_ID, Comp>) {
            // Check if component is enabled if optional
            bool isAvailable = true;
            if constexpr (comp::IsOptionalComponent<Comp>) {
                isAvailable = mesh.template isElementComponentAvailable<
                    SOURCE_ELEM_ID,
                    Comp::COMPONENT_ID>();
            }
            if (isAvailable) {
                const auto targetContainerSize =
                    mesh.template containerSize<TARGET_ELEM_ID>();

                for (const auto& el :
                     mesh.template elements<SOURCE_ELEM_ID>()) {
                    const uint count = Comp::refCount(el);
                    for (uint i = 0; i < count; ++i) {
                        const auto ref = Comp::refIndex(el, i);
                        if (ref == MeshType::NULL_INDEX) {
                            if constexpr (
                                Comp::COMPONENT_ID ==
                                CompId::VERTEX_REFERENCES) {
                                throw InconsistentMeshException(
                                    elementEnumString<SOURCE_ELEM_ID>() + " " +
                                    std::to_string(el.index()) + " at vertex " +
                                    std::to_string(i) +
                                    " references an unassigned/null vertex "
                                    "(NULL_INDEX).");
                            }
                        }
                        else {
                            if (ref >= targetContainerSize) {
                                throw InconsistentMeshException(
                                    elementEnumString<SOURCE_ELEM_ID>() + " " +
                                    std::to_string(el.index()) +
                                    " has a reference to " +
                                    elementEnumString<TARGET_ELEM_ID>() +
                                    " out of range (" + std::to_string(ref) +
                                    " >= container size " +
                                    std::to_string(targetContainerSize) + ").");
                            }
                            if (mesh.template element<TARGET_ELEM_ID>(ref)
                                    .deleted()) {
                                throw InconsistentMeshException(
                                    elementEnumString<SOURCE_ELEM_ID>() + " " +
                                    std::to_string(el.index()) +
                                    " references a deleted " +
                                    elementEnumString<TARGET_ELEM_ID>() +
                                    " (index " + std::to_string(ref) + ").");
                            }
                        }
                    }
                }
            }
        }
    };

    [&]<typename... Comps>(TypeWrapper<Comps...>) {
        (checkComp.template operator()<Comps>(), ...);
    }(SourceComponents {});
}

} // namespace detail

/**
 * @brief Checks the consistency of all topological references and adjacencies
 * in the mesh.
 *
 * It generically inspects every element container present in the Mesh
 * (e.g. vertices, faces, edges, etc.) and for every component that references
 * another element type:
 * 1. Vertex references (in faces, edges, etc.) do not point to NULL_INDEX,
 *    are within [0, containerSize), and do not reference deleted vertices.
 * 2. Adjacency references (adjacent faces, edges, vertices), when not
 * NULL_INDEX, are within their valid target container size and do not reference
 * deleted elements.
 *
 * If an inconsistency is detected, an InconsistentMeshException is thrown.
 *
 * @tparam MeshType: The type of the mesh to check.
 * @param[in] mesh: The mesh to check.
 * @return true if all references in the mesh are consistent.
 * @throws InconsistentMeshException if an invalid reference or adjacency is
 * found.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
bool checkMeshReferences(const MeshType& mesh)
{
    using Containers = typename MeshType::ElementContainers;

    auto checkTarget = [&]<typename TargetCont>() {
        auto checkSource = [&]<typename SourceCont>() {
            detail::checkContainerReferences<MeshType, TargetCont, SourceCont>(
                mesh);
        };

        [&]<typename... SConts>(TypeWrapper<SConts...>) {
            (checkSource.template operator()<SConts>(), ...);
        }(Containers {});
    };

    [&]<typename... TConts>(TypeWrapper<TConts...>) {
        (checkTarget.template operator()<TConts>(), ...);
    }(Containers {});

    return true;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CHECK_REFERENCES_H
