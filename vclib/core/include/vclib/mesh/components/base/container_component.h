// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BASE_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASE_CONTAINER_COMPONENT_H

#include <vclib/mesh/components/base/component.h>
#include <vclib/space/core/vector/vector.h>

namespace vcl::comp {

/**
 * @brief Base class for all components that store a collection of elements
 * (statically sized with std::array if N >= 0, or dynamically with std::vector
 * if N < 0).
 *
 * It wraps vcl::Vector<T, N> and provides type information and access helpers
 * for derived components, without polluting the ElementProxy interface with
 * generic methods that would cause collisions in multiple inheritance.
 *
 * ### ContainerComponent vs ElementContainer Indexing
 *
 * Classes deriving from ContainerComponent (such as VertexReferences,
 * AdjacentFaces, AdjacentEdges, AdjacentVertices, and Wedges) store collections
 * of items per mesh element. In this context, it is crucial to distinguish:
 *
 * - **Local Container Positions (`uint`)**:
 *   Indices used to access, insert, or erase items in this container (e.g.,
 *   `pos` in `insert(pos, ...)` or `erase(pos)`) represent the 0-based offset
 *   or position within this local collection. They are always typed as `uint`
 *   (`uint32_t`) with `vcl::UINT_NULL` as the invalid/not-found sentinel value.
 *
 * - **Global Element Indices (`IndexType`)**:
 *   When the stored items `T` are references to other mesh elements (as in
 *   ContainerReferencesComponent), the *values* stored inside this container
 *   are global indices pointing to elements in the mesh's `ElementContainer`
 *   (e.g. `VertexContainer`). These global indices are typed as `IndexType` (32
 *   or 64 bit) with `vcl::NULL_INDEX<IndexType>` as the invalid/null sentinel
 *   value.
 *
 * @tparam COMP_ID: The unique identifier of the component.
 * @tparam T: The type of the elements stored in the container.
 * @tparam N: The size of the container. If N >= 0, static size. If N < 0,
 * dynamic size.
 * @tparam TTVC: True if the component's container size is tied to the vertex
 * count of the parent element.
 * @tparam Container: The container type to use. Defaults to vcl::Vector<T, N>.
 */
template<
    CompId COMP_ID,
    typename T,
    int  N,
    bool TTVC,
    bool OPT           = false,
    typename Container = vcl::Vector<T, N>>
class ContainerComponent : public Component<COMP_ID, Container, OPT>
{
    using Base = Component<COMP_ID, Container, OPT>;

public:
    static constexpr bool TIED_TO_VERTEX_COUNT = TTVC;

protected:
    using ValueType     = T;
    using ContainerType = Container;

    static decltype(auto) container(auto&& self)
    {
        return Base::compData(self);
    }

    void resize(this auto&& self, uint n) requires (N < 0)
    {
        container(self).resize(n);
    }

    void pushBack(this auto&& self) requires (N < 0)
    {
        container(self).pushBack();
    }

    void pushBack(this auto&& self, const T& v) requires (N < 0)
    {
        container(self).pushBack(v);
    }

    void pushBack(this auto&& self, T&& v) requires (N < 0)
    {
        container(self).pushBack(std::move(v));
    }

    void insert(this auto&& self, uint pos) requires (N < 0)
    {
        container(self).insert(pos);
    }

    void insert(this auto&& self, uint pos, const T& v) requires (N < 0)
    {
        container(self).insert(pos, v);
    }

    void insert(this auto&& self, uint pos, T&& v) requires (N < 0)
    {
        container(self).insert(pos, std::move(v));
    }

    void erase(this auto&& self, uint pos) requires (N < 0)
    {
        container(self).erase(pos);
    }

    void clear(this auto&& self) requires (N < 0) { container(self).clear(); }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASE_CONTAINER_COMPONENT_H
