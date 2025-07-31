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

#ifndef VCL_MESH_COMPONENTS_NORMAL_H
#define VCL_MESH_COMPONENTS_NORMAL_H

#include "base/component.h"
#include "concepts/predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The Normal class represents a N-Dimensional normal vector that will be
 * part of an Element (e.g. Vertex, Face...).
 *
 * Allows to get and set an object that represents a normal. The type of this
 * object must satisfy the PointConcept.
 *
 * For example, if you have a Vertex Element `v` with the Normal component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto n = v.normal();
 * @endcode
 *
 * @tparam P: The type of the normal. This template argument must be a type that
 * satisfies the PointConcept.
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<PointConcept P, typename ParentElemType = void, bool OPT = false>
class Normal :
        public Component<
            Normal<P, ParentElemType, OPT>,
            CompId::NORMAL,
            P,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        Normal<P, ParentElemType, OPT>,
        CompId::NORMAL,
        P,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /**
     * @brief Expose the type of the Normal.
     */
    using NormalType = P;

    /* Constructors */

    /**
     * @brief Initilizes the Normal to (0, 0, 0).
     */
    Normal() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference of the normal of the element.
     * @return a const reference of the normal of the element.
     */
    const P& normal() const { return Base::data(); }

    /**
     * @brief Returns a reference of the normal of the element.
     * @return a reference of the normal of the element.
     */
    P& normal() { return Base::data(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const { normal().serialize(os); }

    void deserialize(std::istream& is) { normal().deserialize(is); }
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the Normal component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::Normal, with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal
 * Normal component and the vertical OptionalNormal component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides Normal component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasNormal.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasNormal = TTB::IsDerivedFromSpecializationOfV<T, Normal>;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the Normal component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the
 * HasOptionalNormal.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalNormal =
    HasNormal<T> && IsOptionalComponent<typename RemoveRef<T>::Normal>;

/* importFrom function */

template<PointConcept P, typename ParentElemType, bool OPT>
template<typename Element>
inline void Normal<P, ParentElemType, OPT>::importFrom(const Element& e, bool)
{
    using ScalarType = NormalType::ScalarType;
    if constexpr (HasNormal<Element>) {
        if (isNormalAvailableOn(e)) {
            normal() = e.normal().template cast<ScalarType>();
        }
    }
}

/* Detector function to check if a class has Normal available */

/**
 * @brief Checks if the given Element has Normal component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has Normal component available, `false`
 * otherwise.
 */
bool isNormalAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::NORMAL>(element);
}

/* Specialization Aliases */

/**
 * @brief The Normal3 class is an alias of the Normal component that uses 3
 * dimensional Points.
 *
 * @tparam Scalar: The scalar type of the point.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Scalar, typename ElementType = void, bool OPT = false>
using Normal3 = Normal<Point3<Scalar>, ElementType, OPT>;

/**
 * @brief The Normal3f class is an alias of the Normal component that uses
 * 3 dimensional Points with float precision.
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
using Normal3f = Normal3<float, ElementType, OPT>;

/**
 * @brief The Normal3d class is an alias of the Normal component that uses
 * 3 dimensional Points with double precision.
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
using Normal3d = Normal3<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_NORMAL_H
