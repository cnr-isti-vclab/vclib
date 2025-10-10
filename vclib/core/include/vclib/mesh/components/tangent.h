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

#ifndef VCL_MESH_COMPONENTS_TANGENT_H
#define VCL_MESH_COMPONENTS_TANGENT_H

#include "base/component.h"
#include "base/predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The Tangent class represents a N-Dimensional tangent and bitangent
 * vectors that will be part of an Element (e.g. Vertex, Face...).
 *
 * Allows to get and set objects that represent a tangent and bitangent . The
 * type of these objects must satisfy the PointConcept.
 *
 * For example, if you have a Vertex Element `v` with the Tangent component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto tn = v.tangent();
 * auto btn = v.bitangent();
 * @endcode
 *
 * @tparam P: The type of the tangent/bitangent. This template argument must be
 * a type that satisfies the PointConcept.
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
class Tangent :
        public Component<
            Tangent<P, ParentElemType, OPT>,
            CompId::TANGENT,
            std::pair<P, P>,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        Tangent<P, ParentElemType, OPT>,
        CompId::TANGENT,
        std::pair<P, P>,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /**
     * @brief Expose the type of the Tangent.
     */
    using TangentType = P;

    /* Constructors */

    /**
     * @brief Initilizes the Tangent to (0, 0, 0).
     */
    Tangent() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference of the tangent of the element.
     * @return a const reference of the tangent of the element.
     */
    const P& tangent() const { return Base::data().first; }

    /**
     * @brief Returns a reference of the tangent of the element.
     * @return a reference of the tangent of the element.
     */
    P& tangent() { return Base::data().first; }

    /**
     * @brief Returns a const reference of the bitangent of the element.
     * @return a const reference of the bitangent of the element.
     */
    const P& bitangent() const { return Base::data().second; }

    /**
     * @brief Returns a reference of the bitangent of the element.
     * @return a reference of the bitangent of the element.
     */
    P& bitangent() { return Base::data().second; }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const
    {
        tangent().serialize(os);
        bitangent().serialize(os);
    }

    void deserialize(std::istream& is)
    {
        tangent().deserialize(is);
        bitangent().deserialize(is);
    }
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the Tangent component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::Tangent, with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal
 * Tangent component and the vertical OptionalTangent component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides Tangent component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasTangent.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTangent = TTB::IsDerivedFromSpecializationOfV<T, Tangent>;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the Tangent component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the
 * HasOptionalTangent.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalTangent =
    HasTangent<T> && IsOptionalComponent<typename RemoveRef<T>::Tangent>;

/* importFrom function */

template<PointConcept P, typename ParentElemType, bool OPT>
template<typename Element>
void Tangent<P, ParentElemType, OPT>::importFrom(const Element& e, bool)
{
    using ScalarType = TangentType::ScalarType;
    if constexpr (HasTangent<Element>) {
        if (isTangentAvailableOn(e)) {
            tangent() = e.tangent().template cast<ScalarType>();
            bitangent() = e.bitangent().template cast<ScalarType>();
        }
    }
}

/* Detector function to check if a class has Tangent available */

/**
 * @brief Checks if the given Element has Tangent component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has Tangent component available, `false`
 * otherwise.
 */
bool isTangentAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::TANGENT>(element);
}

/* Specialization Aliases */

/**
 * @brief The Tangent3 class is an alias of the Tangent component that uses 3
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
using Tangent3 = Tangent<Point3<Scalar>, ElementType, OPT>;

/**
 * @brief The Tangent3f class is an alias of the Tangent component that uses
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
using Tangent3f = Tangent3<float, ElementType, OPT>;

/**
 * @brief The Tangent3d class is an alias of the Tangent component that uses
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
using Tangent3d = Tangent3<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TANGENT_H
