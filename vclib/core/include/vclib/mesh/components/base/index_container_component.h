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

#ifndef VCL_MESH_COMPONENTS_BASE_INDEX_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASE_INDEX_CONTAINER_COMPONENT_H

#include "container_component.h"

#include <vclib/mesh/iterators/components/pointer_from_index_iterator.h>

#include <vclib/space/core.h>

namespace vcl::comp {

/// @cond VCLIB_HIDDEN_DOCS

/**
 * @brief The IndexContainerComponent is the base class for all the
 * components of VCLib that store a container of indices of an Element.
 *
 * A component that inherits from this class will have all the features of the
 * @ref Component and @ref ContainerComponent classes, and will have access to
 * the implementations of updateElementPointers/Indices member functions, that
 * should be used by the Derived Component.
 *
 * For further details , please refer to the page @ref implement_component page.
 *
 * @tparam DerivedComponent: The type of the Derived Component. It is used to
 * implement the CRTP pattern.
 * @tparam COMP_ID: The id of the component. It is a value of the enum
 * ComponentIDEnum, or an integer value that is not already used by any other
 * component. It is used to identify the component at compile time.
 * @tparam Elem: The type of the Element of which the indices are stored. E.g.
 * a VertexIndices component would have VertexType as Elem.
 * @tparam N: The size of the container: if >= 0 the size is static, if < 0 the
 * size is dynamic.
 * @tparam ParentElemType: The Parent Element type is used to get access to the
 * Element that has the component (and, in case, to the Mesh that has the
 * Element). If the component doesn't need to access the Element, this type can
 * be void. Note: if the component is vertical (or optional), this type cannot
 * be void.
 * @tparam VERT: Boolean that tells if the component is vertical. If the
 * component is vertical, this parameter must be true. Note: to be vertical,
 * this parameter must be true, and ElementType must be the type of the Element
 * that has the component (the 'parent' Element Type).
 * @tparam OPT: When a component is vertical, it could be optional, that means
 * that could be enabled/disabled at runtime. To make the component optional,
 * this template parameter must be true.
 * @tparam TTVN: "Tied To Vertex Number", this boolean tells whether the
 * component should leave liberty to control the size of the Container to the
 * user, or it should be tied to the vertex number and changed automatically
 * when the number of vertices changes. E.g. a AdjacentFaceIndices component, if
 * part of a Face element, should be tied to the vertex number of the Face,
 * meaning that the number of face indices should be always be the same of the
 * number of vertices. Setting this value to true will tie the size of this
 * container to the number of vertices.
 */
template<
    typename DerivedComponent, // CRTP pattern, derived class
    uint COMP_ID,              // component id
    typename Elem,             // element type for which the indices are stored
    int N,                     // container size
    typename ParentElemType,   // parent element type
    bool VERT,                 // true if component vertical
    bool OPT,                  // true if component vertical and optional
    bool TTVN>                 // true if container size tied to vertex number
class IndexContainerComponent :
        public ContainerComponent<
            DerivedComponent,
            COMP_ID,
            uint,
            N,
            void,
            ParentElemType,
            VERT,
            OPT,
            TTVN,
            Elem>
{
    using Base = ContainerComponent<
        DerivedComponent,
        COMP_ID,
        uint,
        N,
        void,
        ParentElemType,
        VERT,
        OPT,
        TTVN,
        Elem>;

public:
    IndexContainerComponent()
    {
        if constexpr (!Base::IS_VERTICAL) {
            init();
        }
    }

    void init()
    {
        if constexpr (N >= 0) {
            container().fill(UINT_NULL);
        }
    }

    /**
     * @brief Exposes the indices of the container.
     *
     * This function is templated in order to force the user to specify the type
     * of the indices to element that are stored in the container.
     * This is necessary when a component that stores indices to different
     * types of elements is used.
     */
    template<typename T>
    const auto& indices() const requires std::is_same_v<T, Elem>
    {
        return container();
    }

protected:
    using Base::container;

    using ConstIndexIterator = typename Base::ConstIterator;
    using Iterator =
        PointerFromIndexIterator<ConstIndexIterator, Elem, ParentElemType>;
    using ConstIterator =
        ConstPointerFromIndexIterator<ConstIndexIterator, Elem, ParentElemType>;

    /*
     * This member function is called when we need to update the indices in
     * this container after a reallocation (the pointer of the first element of
     * the container of Elem is changed).
     *
     * With respect to the updateReferences function of the
     * PointerContainerComponent, we need to do something only when offset is
     * different from 0, because in this case the indices of the elements have
     * changed (append case).
     *
     * When we append elements, this function is called only on the elements
     * that have been appended, and the offset is the number of referenced
     * elements that were in the container before the append operation.
     */
    void updateReferences(const Elem*, std::size_t offset = 0)
    {
        if (offset != 0) {
            auto& baseContainer = Base::container();

            for (uint j = 0; j < baseContainer.size(); ++j) {
                if (baseContainer.at(j) != UINT_NULL) {
                    baseContainer.at(j) += offset;
                }
            }
        }
    }

    /*
     * This member function is called when we need to update the pointers or
     * indices in this containers, usually after a compaction of the container
     * (but not always).
     *
     * In this case, indices must be updated, because the indices of the
     * elements have changed.
     */
    void updateReferences(const std::vector<uint>& newIndices)
    {
        auto& baseContainer = Base::container();

        for (uint j = 0; j < baseContainer.size(); ++j) {
            if (baseContainer.at(j) != UINT_NULL) {
                baseContainer.at(j) = newIndices.at(baseContainer.at(j));
            }
        }
    }
};

/// @endcond

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASE_INDEX_CONTAINER_COMPONENT_H
