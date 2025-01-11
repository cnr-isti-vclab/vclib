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

#ifndef VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H

#include "component.h"

#include <vclib/space/core/vector.h>

namespace vcl::comp {

namespace detail {

// small alias to differentiate between components having only a container or
// components having a container and an additional data type:
// - if AD is void, no additional data and the component will have only a Vector
// - if AD is not void, there is additional data, and the component will have
//   a tuple containing a Vector and the additional data
template<
    typename DC,
    uint CT,
    typename T,
    int N,
    typename AD,
    typename El,
    bool v,
    bool o,
    bool TT,
    typename... PT>
using ContCompBase = std::conditional_t<
    std::is_same_v<AD, void>,
    Component<DC, CT, Vector<T, N>, El, v, o, PT...>,
    Component<DC, CT, std::tuple<Vector<T, N>, AD>, El, v, o, PT...>>;

} // namespace detail

/**
 * @brief The ContainerComponent class is the base class for all the components
 * of VCLib that store a container of objects of a given type T.
 *
 * A component that inherits from this class will have all the features of the
 * @ref Component class, plus automatic storage of the data in a Container of
 * objects that could have static or dynamic size, and the possibility to tie
 * the size of the Container to the number of Vertices of the Element (E.g.
 * useful for some Face components like AdjacentFaces or WedgeTexCoords).
 *
 * For further details , please refer to the page @ref implement_component page.
 *
 * @tparam DerivedComponent: The type of the Derived Component. It is used to
 * implement the CRTP pattern.
 * @tparam COMPONENT_ID: The id of the component. It is a value of the enum
 * ComponentIDEnum, or an integer value that is not already used by any other
 * component. It is used to identify the component at compile time.
 * @tparam T: The type of the data that the component needs to store in a
 * Container. E.g. a WedgeTexCoord component would have vcl::TexCoordd as
 * DataType.
 * @tparam N: The size of the container: if >= 0 the size is static, if < 0 the
 * size is dynamic.
 * @tparam AdditionalData: the type of additional data that could be stored
 * outside the Container. If you don't have additional data. set AdditionalData
 * = void. E.g. a WedgeTexCoord component stores a short as AdditionalData that
 * indicates the texture index used by all the Wedge TexCoords.
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
 * when the number of vertices changes. E.g. a WedgeTexCoord component is a
 * Container that should be tied to the vertex number, meaning that the number
 * of texture coordinates should be always be the same of the number of
 * vertices. Setting this value to true will tie the size of this container to
 * the number of vertices.
 * @tparam PointedTypes: Variadic Template types of all the Pointer types that
 * the component stores, and that need to be updated when some reallocation
 * happens.
 */
template<
    typename DerivedComponent, // CRTP pattern, derived class
    uint COMP_ID,              // component id
    typename T,                // data stored in container
    int N,                     // container size
    typename AdditionalData,   // additional data outside container
    typename ParentElemType,   // parent element type
    bool VERT,                 // true if component vertical
    bool OPT,                  // true if component vertical and optional
    bool TTVN,                 // true if container size tied to vertex number
    typename... PointedTypes>  // types of pointers stored by the component
class ContainerComponent :
        public detail::ContCompBase<
            DerivedComponent,
            COMP_ID,
            T,
            N,
            AdditionalData,
            ParentElemType,
            VERT,
            OPT,
            TTVN,
            PointedTypes...>
{
    static constexpr bool HAS_ADDITIONAL_DATA =
        !std::is_same_v<AdditionalData, void>;

    using Base = detail::ContCompBase<
        DerivedComponent,
        COMP_ID,
        T,
        N,
        AdditionalData,
        ParentElemType,
        VERT,
        OPT,
        TTVN,
        PointedTypes...>;

public:
    /**
     * @brief Boolean that tells if this component stores a container having its
     * size tied to the number of the vertices of the Element.
     *
     * E.g. suppose to have a Polygonal Face f (dynamic number of vertices),
     * having 5 vertices (and 5 edges). This means that if the Face has the
     * AdjacentFaces component, then it should store 5 adjacent faces (the same
     * number of the vertices). In this case, the AdjacentFaces component will
     * have the boolean TIED_TO_VERTEX_NUMBER set to true.
     */
    static const bool TIED_TO_VERTEX_NUMBER = TTVN;

    static const int SIZE = N;

protected:
    /* Iterator Types declaration */

    using Iterator      = Vector<T, N>::Iterator;
    using ConstIterator = Vector<T, N>::ConstIterator;

    /* Constructor */

    /*
     * Create a container of T objects.
     * If this Container is a static array, all its element will be initialized
     * to T(). If this Container is a dynamic vector, it will be an empty
     * container.
     */
    ContainerComponent()
    {
        if constexpr (!Base::IS_VERTICAL) {
            if constexpr (N >= 0) {
                Base::data().fill(T());
            }
        }
    }

    /*
     * Create a container of Objects.
     * If this Container is a static array, all its element will be initialized
     * to T(). If this Container is a dynamic vector, it will be an empty
     * container.
     */
    void init()
    {
        if constexpr (N >= 0) {
            // I'll use the array, N is >= 0.
            // There will be a static number of objects.
            container().fill(T());
        }
        else {
            // I'll use the vector, because N is < 0.
            // There will be a dynamic number of objects.
            container().clear();
        }
    }

    Vector<T, N>& container()
    {
        if constexpr (HAS_ADDITIONAL_DATA) {
            return std::get<0>(Base::data());
        }
        else {
            return Base::data();
        }
    }

    const Vector<T, N>& container() const
    {
        if constexpr (HAS_ADDITIONAL_DATA) {
            return std::get<0>(Base::data());
        }
        else {
            return Base::data();
        }
    }

    template<typename AdDt = AdditionalData>
    AdDt& additionalData() requires (HAS_ADDITIONAL_DATA)
    {
        return std::get<1>(Base::data());
    }

    template<typename AdDt = AdditionalData>
    const AdDt& additionalData() const requires (HAS_ADDITIONAL_DATA)
    {
        return std::get<1>(Base::data());
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H
