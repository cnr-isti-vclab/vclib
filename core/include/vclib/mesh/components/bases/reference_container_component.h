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

#ifndef VCL_MESH_COMPONENTS_BASES_REFERENCE_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_REFERENCE_CONTAINER_COMPONENT_H

#include "index_container_component.h"
#include "pointer_container_component.h"

namespace vcl::comp {

/**
 * @brief The ReferenceContainerComponent is a class that inherits from the
 * @ref IndexContainerComponent or @ref PointerContainerComponent classes,
 * depending on the STORE_INDICES template parameter.
 *
 * @tparam STORE_INDICES: If true, the component will store indices, otherwise
 * pointers to Elem.
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
    bool STORE_INDICES,
    typename DerivedComponent, // CRTP pattern, derived class
    uint COMP_ID,              // component id
    typename Elem,             // element type for which the pointers are stored
    int N,                     // container size
    typename ParentElemType,   // parent element type
    bool VERT,                 // true if component vertical
    bool OPT,                  // true if component vertical and optional
    bool TTVN>                 // true if container size tied to vertex number
class ReferenceContainerComponent :
        public std::conditional_t<
            STORE_INDICES,
            IndexContainerComponent<
                DerivedComponent,
                COMP_ID,
                Elem,
                N,
                ParentElemType,
                VERT,
                OPT,
                TTVN>,
            PointerContainerComponent<
                DerivedComponent,
                COMP_ID,
                Elem,
                N,
                ParentElemType,
                VERT,
                OPT,
                TTVN>>
{
protected:
    using Base = std::conditional_t<
        STORE_INDICES,
        IndexContainerComponent<
            DerivedComponent,
            COMP_ID,
            Elem,
            N,
            ParentElemType,
            VERT,
            OPT,
            TTVN>,
        PointerContainerComponent<
            DerivedComponent,
            COMP_ID,
            Elem,
            N,
            ParentElemType,
            VERT,
            OPT,
            TTVN>>;

    uint size() const { return Base::container().size(); }

    Elem* element(uint i)
    {
        if constexpr (STORE_INDICES)
            return elemFromParent(elementIndex(i));
        else
            return Base::container().at(i);
    }

    const Elem* element(uint i) const
    {
        if constexpr (STORE_INDICES)
            return elemFromParent(elementIndex(i));
        else
            return Base::container().at(i);
    }

    uint elementIndex(uint i) const
    {
        if constexpr (STORE_INDICES)
            return Base::container().at(i);
        else
            return indexFromPointer(Base::container().at(i));
    }

    Elem* elementMod(int i)
    {
        if constexpr (STORE_INDICES)
            return elemFromParent(elementIndexMod(i));
        else
            return Base::container().atMod(i);
    }

    const Elem* elementMod(int i) const
    {
        if constexpr (STORE_INDICES)
            return elemFromParent(elementIndexMod(i));
        else
            return Base::container().atMod(i);
    }

    uint elementIndexMod(int i) const
    {
        if constexpr (STORE_INDICES)
            return Base::container().atMod(i);
        else
            return indexFromPointer(elementMod(i));
    }

    void setElement(uint i, Elem* e)
    {
        if constexpr (STORE_INDICES)
            Base::container().set(i, indexFromPointer(e));
        else
            Base::container().set(i, e);
    }

    void setElement(uint i, uint ei)
    {
        if constexpr (STORE_INDICES)
            Base::container().set(i, ei);
        else
            Base::container().set(i, elemFromParent(ei));
    }

    void setElement(Base::ConstIterator it, Elem* v)
    {
        setElement(it - elementBegin(), v);
    }

    void setElement(Base::ConstIterator it, uint vi)
    {
        setElement(it - elementBegin(), vi);
    }

    void setElement(Base::ConstIndexIterator it, Elem* v)
    {
        setElement(it - elementBegin(), v);
    }

    void setElement(Base::ConstIndexIterator it, uint vi)
    {
        setElement(it - elementBegin(), vi);
    }

    void setElementMod(int i, Elem* e)
    {
        if constexpr (STORE_INDICES)
            Base::container().atMod(i) = indexFromPointer(e);
        else
            Base::container().atMod(i) = e;
    }

    void setElementMod(int i, uint ei)
    {
        if constexpr (STORE_INDICES)
            Base::container().atMod(i) = elemFromParent(ei);
        else
            Base::container().atMod(i) = ei;
    }

    template<Range Rng>
    void setElements(Rng&& r) requires RangeOfConvertibleTo<Rng, Elem*>
    {
        if constexpr (STORE_INDICES) {
            auto conv = [&](auto v) {
                return indexFromPointer(v);
            };

            Base::container().set(r | std::views::transform(conv));
        }
        else {
            Base::container().set(r);
        }
    }

    template<Range Rng>
    void setElements(Rng&& r) requires RangeOfConvertibleTo<Rng, uint>
    {
        if constexpr (STORE_INDICES) {
            Base::container().set(r);
        }
        else {
            auto conv = [&](auto i) {
                return elemFromParent(i);
            };

            Base::container().set(r | std::views::transform(conv));
        }
    }

    bool containsElement(const Elem* e) const
    {
        if constexpr (STORE_INDICES)
            return Base::container().contains(indexFromPointer(e));
        else
            return Base::container().contains(e);
    }

    bool containsElement(uint ei) const
    {
        if constexpr (STORE_INDICES)
            return Base::container().contains(ei);
        else
            return Base::container().contains(elemFromParent(ei));
    }

    uint indexOfElement(const Elem* e) const
    {
        if constexpr (STORE_INDICES)
            return Base::container().indexOf(indexFromPointer(e));
        else
            return Base::container().indexOf(e);
    }

    uint indexOfElement(uint ei) const
    {
        if constexpr (STORE_INDICES)
            return Base::container().indexOf(ei);
        else
            return Base::container().indexOf(elemFromParent(ei));
    }

    Base::Iterator elementBegin()
    {
        if constexpr (STORE_INDICES)
            return typename Base::Iterator(
                Base::container().begin(), Base::parentElement());
        else
            return Base::container().begin();
    }

    Base::Iterator elementEnd()
    {
        if constexpr (STORE_INDICES)
            return typename Base::Iterator(Base::container().end());
        else
            return Base::container().end();
    }

    Base::ConstIterator elementBegin() const
    {
        if constexpr (STORE_INDICES)
            return typename Base::ConstIterator(
                Base::container().begin(), Base::parentElement());
        else
            return Base::container().begin();
    }

    Base::ConstIterator elementEnd() const
    {
        if constexpr (STORE_INDICES)
            return typename Base::ConstIterator(Base::container().end());
        else
            return Base::container().end();
    }

    Base::ConstIndexIterator elementIndexBegin() const
    {
        if constexpr (STORE_INDICES)
            return Base::container().begin();
        else
            return typename Base::ConstIndexIterator(elementBegin());
    }

    Base::ConstIndexIterator elementIndexEnd() const
    {
        if constexpr (STORE_INDICES)
            return Base::container().end();
        else
            return typename Base::ConstIndexIterator(elementEnd());
    }

    View<typename Base::Iterator> elements()
    {
        return View(elementBegin(), elementEnd());
    }

    View<typename Base::ConstIterator> elements() const
    {
        return View(elementBegin(), elementEnd());
    }

    View<typename Base::ConstIndexIterator> elementIndices() const
    {
        return View(elementIndexBegin(), elementIndexEnd());
    }

    // ContainerComponent interface functions

    void resize(uint n) requires (N < 0)
    {
        if constexpr (STORE_INDICES)
            Base::container().resize(n, UINT_NULL);
        else
            Base::container().resize(n);
    }

    void pushBack(Elem* e = nullptr) requires (N < 0)
    {
        if constexpr (STORE_INDICES)
            Base::container().pushBack(indexFromPointer(e));
        else
            Base::container().pushBack(e);
    }

    void pushBack(uint ei) requires (N < 0)
    {
        if constexpr (STORE_INDICES)
            Base::container().pushBack(ei);
        else
            Base::container().pushBack(elemFromParent(ei));
    }

    void insert(uint i, Elem* e = nullptr) requires (N < 0)
    {
        if constexpr (STORE_INDICES)
            Base::container().insert(i, indexFromPointer(e));
        else
            Base::container().insert(i, e);
    }

    void insert(uint i, uint ei) requires (N < 0)
    {
        if constexpr (STORE_INDICES)
            Base::container().insert(i, ei);
        else
            Base::container().insert(i, elemFromParent(ei));
    }

    void erase(uint i) requires (N < 0) { Base::container().erase(i); }

    void clear() requires (N < 0) { Base::container().clear(); }

    // Utility functions

    uint indexFromPointer(const Elem* v) const
    {
        if (v == nullptr) [[unlikely]]
            return UINT_NULL;
        else
            return v->index();
    }

    Elem* elemFromParent(uint vi)
    {
        if (vi == UINT_NULL) [[unlikely]]
            return nullptr;
        else
            return &Base::parentElement()
                        ->parentMesh()
                        ->template element<Elem::ELEMENT_ID>(vi);
    }

    const Elem* elemFromParent(uint vi) const
    {
        if (vi == UINT_NULL) [[unlikely]]
            return nullptr;
        else
            return &Base::parentElement()
                        ->parentMesh()
                        ->template element<Elem::ELEMENT_ID>(vi);
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASES_REFERENCE_CONTAINER_COMPONENT_H
