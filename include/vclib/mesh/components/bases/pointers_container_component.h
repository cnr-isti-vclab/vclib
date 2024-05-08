/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_MESH_COMPONENTS_BASES_POINTERS_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_POINTERS_CONTAINER_COMPONENT_H

#include <vclib/iterators/const_pointer_iterator.h>
#include <vclib/iterators/mesh/components/index_from_pointer_iterator.h>

#include "container_component.h"

namespace vcl::comp {

/**
 * @brief The PointersContainerComponent is the base class for all the
 * components of VCLib that store a container of pointers to an Element.
 *
 * A component that inherits from this class will have all the features of the
 * @ref Component and @ref ContainerComponent classes, and will have access to
 * the implementations of updateElementPointers member functions, that should be
 * used by the Derived Component.
 *
 * For further details , please refer to the page @ref implement_component page.
 *
 * @tparam DerivedComponent: The type of the Derived Component. It is used to
 * implement the CRTP pattern.
 * @tparam COMP_ID: The id of the component. It is a value of the enum
 * ComponentIDEnum, or an integer value that is not already used by any other
 * component. It is used to identify the component at compile time.
 * @tparam Elem: The type of the Element of which the pointers are stored. E.g.
 * a VertexPointers component would have VertexType as Elem.
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
 * when the number of vertices changes. E.g. a AdjacentFaces component, if part
 * of a Face element, should be tied to the vertex number of the Face, meaning
 * that the number of face pointers should be always be the same of the number
 * of vertices. Setting this value to true will tie the size of this container
 * to the number of vertices.
 */
template<
    typename DerivedComponent, // CRTP pattern, derived class
    uint COMP_ID,              // component id
    typename Elem,             // element type for which the pointers are stored
    int N,                     // container size
    typename ParentElemType,   // parent element type
    bool VERT,                 // true if component vertical
    bool OPT,                  // true if component vertical and optional
    bool TTVN>                 // true if container size tied to vertex number
class PointersContainerComponent :
        public ContainerComponent<
            DerivedComponent,
            COMP_ID,
            Elem*,
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
        Elem*,
        N,
        void,
        ParentElemType,
        VERT,
        OPT,
        TTVN,
        Elem>;

protected:
    using Base::container;

    using Iterator      = Base::ConstIterator;
    using ConstIterator = ConstPointerIterator<typename Base::ConstIterator>;
    using ConstIndexIterator = IndexFromPointerIterator<ConstIterator>;

    /*
     * This member function is called when we need to update the pointers in
     * this container after a reallocation (the pointer of the first element of
     * the container is changed from oldBase to newBase).
     *
     * This is necessary when, for example, the original container of Elements
     * has been reallocated. When this happens, the all the Elements have been
     * moved in another portion of memory, and all the pointers to that Elements
     * must be updated. Since in this container are stored pointers to Elements,
     * we need to update them.
     *
     * To update them, we need to know the oldBase (the pointer to the first
     * Element of the reallocated Container before the reallocation) and the
     * newBase (the pointer to the first Element of the reallocated Container
     * after the reallocation. We can then compute, for each pointer, the
     * difference w.r.t. the first element of the Container, and update the the
     * pointer accordingly using the newBase.
     *
     * When we perform an append operation, we need to update the pointers
     * taking into account also the offset: when we append a new element in a
     * container, only its pointers must be updated. To update from the old
     * pointers to the new ones, we need to know how many elements were in the
     * container BEFORE the append operation, and this becomes the offset to
     * be applied to the pointers of the newly appended elements.
     */
    void updatePointers(
        const Elem* oldBase,
        const Elem* newBase,
        std::size_t offset = 0)
    {
        auto& baseContainer = Base::container();

        for (uint j = 0; j < baseContainer.size(); ++j)
        { // for each pointer in this container
            if (baseContainer.at(j) != nullptr) {
                size_t diff =
                    baseContainer.at(j) - oldBase; // offset w.r.t. the old base
                baseContainer.at(j) =
                    (Elem*) newBase + diff +
                    offset; // update the pointer using newBase
            }
        }
    }

    /*
     * This member function is called when we need to update the pointers in
     * this containers, usually after a compaction of the container (but not
     * always).
     *
     * In this case, the address of the first element in the container is not
     * changed, but may change the position of each element inside the
     * container. The function takes the base pointer of the first element of
     * the container, and a vector that stores, for each old element position,
     * the new position in the container (UINT_NULL if the element has been
     * removed and must be left unreferenced).
     */
    void updatePointers(const Elem* base, const std::vector<uint>& newIndices)
    {
        auto& baseContainer = Base::container();

        for (uint j = 0; j < baseContainer.size(); ++j) {
            if (baseContainer.at(j) != nullptr) {
                size_t diff = baseContainer.at(j) - base;
                if (newIndices[diff] == UINT_NULL) { // element has been removed
                    baseContainer.at(j) = nullptr;
                }
                else { // the new pointer will be base + newIndices[diff]
                    baseContainer.at(j) = (Elem*) base + newIndices[diff];
                }
            }
        }
    }

public:
    /**
     * @brief Exposes the pointers in the container as a View.
     *
     * This function is templated in order to force the user to specify the type
     * of the pointers that are stored in the container.
     * This is necessary when a component that stores pointers to different
     * types of elements is used.
     */
    template<typename T>
    auto pointers() const requires std::is_same_v<T, Elem>
    {
        return vcl::View(
            ConstIterator(Base::container().begin()),
            ConstIterator(Base::container().end()));
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASES_POINTERS_CONTAINER_COMPONENT_H
