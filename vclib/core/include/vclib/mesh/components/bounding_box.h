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

#ifndef VCL_MESH_COMPONENTS_BOUNDING_BOX_H
#define VCL_MESH_COMPONENTS_BOUNDING_BOX_H

#include "base/component.h"
#include "base/predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The BoundingBox component class represents an axis aligned bounding
 * box. This class is usually used as a component of a Mesh.
 *
 * The member functions of this class will be available in the instance of any
 * Element or Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the BoundingBox component, you'll be
 * able to access to this component member functions from `m`:
 *
 * @code{.cpp}
 * m.boundingBox();
 * @endcode
 *
 * @note This component can be both used for Elements and Meshes.
 *
 * @tparam PointType: The type of the point that will be used to represent the
 * bounding box. This template argument must satisfy the PointConcept.
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<
    PointConcept PointType,
    typename ParentElemType = void,
    bool OPT                = false>
class BoundingBox :
        public Component<
            BoundingBox<PointType, ParentElemType, OPT>,
            CompId::BOUNDING_BOX,
            Box<PointType>,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        BoundingBox<PointType, ParentElemType, OPT>,
        CompId::BOUNDING_BOX,
        Box<PointType>,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /**
     * @brief Expose the type of the bounding box.
     */
    using BoundingBoxType = Box<PointType>;

    /* Constructors */

    /**
     * @brief Initilizes the bounding box to an invalid bounding box.
     */
    BoundingBox() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference to the bounding box of this object.
     * @return A const reference to the bounding box of this object.
     */
    const BoundingBoxType& boundingBox() const { return Base::data(); }

    /**
     * @brief Returns a reference to the bounding box of this object.
     * @return A reference to the bounding box of this object.
     */
    BoundingBoxType& boundingBox() { return Base::data(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const { boundingBox().serialize(os); }

    void deserialize(std::istream& is) { boundingBox().deserialize(is); }
};

/* concept */

/**
 * @brief A concept that checks whether a type T (that should be a Mesh)
 * has the BoundingBox component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::BoundingBox, with any template arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasBoundingBox.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasBoundingBox = TTB::IsDerivedFromSpecializationOfV<T, BoundingBox>;

/* importFrom function */

template<PointConcept PointType, typename ParentElemType, bool OPT>
template<typename Element>
void BoundingBox<PointType, ParentElemType, OPT>::importFrom(
    const Element& e,
    bool)
{
    if constexpr (HasBoundingBox<Element>) {
        using ScalarType = PointType::ScalarType;
        boundingBox()    = e.boundingBox().template cast<ScalarType>();
    }
}

/* Detector function to check if a class has BoundingBox available */

/**
 * @brief Checks if the given Element has BoundingBox component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has BoundingBox component available,
 * `false` otherwise.
 */
bool isBoundingBoxAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::BOUNDING_BOX>(element);
}

/* Specializations */

/**
 * @brief The BoundingBox3 component class is an alias of the BoundingBox
 * component that uses 3 dimensional Points.
 *
 * @tparam S: The scalar type of the point.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename S, typename ElementType = void, bool OPT = false>
using BoundingBox3 = BoundingBox<Point3<S>, ElementType, OPT>;

/**
 * @brief The BoundingBox3f component class is an alias of the BoundingBox
 * component that uses 3 dimensional Points with float precision.
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
using BoundingBox3f = BoundingBox<Point3f, ElementType, OPT>;

/**
 * @brief The BoundingBox3d component class is an alias of the BoundingBox
 * component that uses 3 dimensional Points with double precision.
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
using BoundingBox3d = BoundingBox<Point3d, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BOUNDING_BOX_H
