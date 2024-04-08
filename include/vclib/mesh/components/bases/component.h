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

#ifndef VCL_MESH_COMPONENTS_BASES_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_COMPONENT_H

#include <vclib/concepts/mesh/components/component.h>
#include <vclib/concepts/mesh/elements/element.h>
#include <vclib/concepts/mesh/mesh_concept.h>

#include "../detail/component_data.h"

namespace vcl::comp {

/**
 * @brief The Component class is the base class for almost all the components of
 * VCLib (with the exception of CustomComponents).
 *
 * Inheriting from this class, a component will have some necessary definitions
 * to be used smoothly by the vcl::Mesh class, managing all the possible
 * features of a component automatically.
 *
 * The features that a Component could have are:
 *
 * - possibility to be horizontal, vertical or optional:
 *   - a component is horizontal when its data is stored in the memory frame of
 *     the Element that has the component;
 *   - a component is vertical when its data is not stored in the memory frame
 *     of the Element, but in a separated Conainer; in this case, the data will
 *     be in a contiguous array;
 *   - a component is optional if it is vertical and can be enabled/disabled at
 *     runtime;
 * - possibility to store pointers to other Elements that must be updated when a
 *   reallocation happens
 *   - An example is the VertexPointers component: it stores the pointers to the
 *     Vertices of an Element (e.g. a Face). When a a reallocation of the
 *     VertexContainer happens, all the pointers to the vertices must be
 *     updated, and this operation will be made automatically if the Vertex type
 *     will be part of the PointedTypes.
 *
 * There are also some additional features that are given by the @ref
 * ContainerComponent and @ref PointersContainerComponent classes. If you need
 * to implement a Component that stores a Container of data or a Container of
 * Pointers, take a look to that classes.
 *
 * All the features of a Component can be defined trough its template
 * parameters, and implementing some protected member functions. To properly
 * implement a Component class, must define the following protected member
 * function:
 *
 * ```cpp
 * template <typename Element>
 * void importFrom(const Element& e);
 * ```
 *
 * Moreover, if the component has at least one PointedType, it must define the
 * following protected member functions:
 *
 * ```cpp
 * template<typename Element, typename ElEType>
 * void importPointersFrom(
 *     const Element& e, PointedType* base, const ElEType* ebase);
 *
 * void updatePointers(const PointedType* oldBase, const PointedType* newBase);
 *
 * void updatePointers(
 *     const PointedType* base, const std::vector<uint>& newIndices);
 * ```
 *
 * If your component stores a Container of pointers, look for the @ref
 * PointersContainerComponent class, that provides the implementation of these
 * functions.
 *
 * For further details , please refer to the page @ref implement_component page.
 *
 * @tparam DerivedComponent: The type of the Derived Component. It is used to
 * implement the CRTP pattern.
 * @tparam COMP_ID: The id of the component. It is a value of the enum
 * ComponentIDEnum, or an integer value that is not already used by any other
 * component. It is used to identify the component at compile time.
 * @tparam DataType: The type of the data that the component needs to store.
 * E.g. a Normal component would store a vcl::Point3d.
 * @tparam ElementType: This type is used to discriminate between horizontal and
 * vertical components. When a component is horizontal, this type must be void.
 * When a component is vertical, this type must be the type of the Element that
 * has the component, and it will be used by the vcl::Mesh to access to the data
 * stored vertically.
 * @tparam OPT: When a component is vertical, it could be optional, that means
 * that could be enabled/disabled at runtime. To make the component optional,
 * this template parameter must be true.
 * @tparam PointedTypes: Variadic Template types of all the Pointer types that
 * the component stores, and that need to be updated when some reallocation
 * happens.
 */
template<
    typename DerivedComponent, // CRTP pattern, derived class
    uint COMP_ID,              // component id
    typename DataType,         // data stored by the component
    typename ElementType,      // element type, void if horizontal
    bool OPT,                  // true if component vertical and optional
    typename... PointedTypes>  // types of the pointers stored by the component
class Component : public PointersComponentTriggerer<PointedTypes>...
{
public:
    /**
     * @brief The Data that the component will store (e.g. a vcl::Point3d for a
     * Normal component).
     */
    using DataValueType = DataType;

    /**
     * @brief Boolean that tells if this component type stores its data
     * vertically (not in the Element frame memory, but in another vector).
     */
    static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;

    /**
     * @brief The ID of the component.
     */
    static const uint COMPONENT_ID = COMP_ID;

    /**
     * @brief Boolean that tells if this component is optional. Makes sense only
     * when the component is vertical.
     */
    static const bool IS_OPTIONAL = OPT;

private:
    detail::ComponentData<DataValueType, IS_VERTICAL> mData;

public:
    /**
     * @private
     * @brief Returns `true` if the component is available, `false` otherwise.
     *
     * This member function can return `false` only if the component is
     * optional, and it is not enabled.
     *
     * This member function is hidden by the element that inherits this class.
     *
     * @return `true` if the component is available, `false` otherwise.
     */
    bool isAvailable() const
    {
        return mData.template isComponentAvailable<ElementType>(
            static_cast<const DerivedComponent*>(this));
    }

protected:
    DataValueType& data()
    {
        return mData.template get<ElementType>(
            static_cast<DerivedComponent*>(this));
    }

    const DataValueType& data() const
    {
        return mData.template get<ElementType>(
            static_cast<const DerivedComponent*>(this));
    }
};

/**
 * @brief Checks if the given Element or Mesh has the component having
 * COMPONENT_ID available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element/mesh. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] obj: The element/mesh to check. Must be of a type that satisfies
 * the ElementOrMeshConcept.
 * @return `true` if the element/mesh has the component available, `false`
 * otherwise.
 */
template<uint COMPONENT_ID, ElementOrMeshConcept T>
bool isComponentAvailableOn(const T& obj)
{
    if constexpr (HasOptionalComponentOfType<T, COMPONENT_ID>) {
        using ComponentType =
            ComponentOfType<COMPONENT_ID, typename T::Components>;
        const ComponentType& c = static_cast<const ComponentType&>(obj);
        return c.isAvailable();
    }
    else
        return HasComponentOfType<T, COMPONENT_ID>;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASES_COMPONENT_H
