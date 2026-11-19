// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_MESH_COMPONENTS_BOUNDING_BOX_H
#define VCL_MESH_MESH_COMPONENTS_BOUNDING_BOX_H

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/base/component_predicates.h>

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <istream>
#include <ostream>

namespace vcl::mesh {

/**
 * @brief The BoundingBox component class represents an axis-aligned bounding
 * box that is part of a Mesh.
 *
 * The member functions of this class will be available in the instance of any
 * Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the BoundingBox component, you can
 * access and modify its bounding box:
 *
 * @code{.cpp}
 * m.boundingBox().setNull();
 * auto bb = m.boundingBox();
 * @endcode
 *
 * @tparam Point: The type of the point that will be used to represent the
 * bounding box corners (default: Point3f). This template argument must satisfy
 * the PointConcept.
 *
 * @ingroup mesh_components
 */
template<PointConcept Point = Point3f>
class BoundingBox : public comp::Component<CompId::BOUNDING_BOX, Box<Point>>
{
    Box<Point> mBoundingBox;

public:
    /**
     * @brief Expose the point type used by the bounding box.
     */
    using PointType = Point;

    /**
     * @brief Expose the scalar type of the bounding box.
     */
    using ScalarType = typename Point::ScalarType;

    /**
     * @brief Expose the type of the bounding box.
     */
    using BoundingBoxType = Box<Point>;

    /**
     * @brief Default constructor. Initializes the bounding box to an invalid/null bounding box.
     */
    BoundingBox() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the bounding box of the mesh.
     * @return A reference to the bounding box of the mesh.
     */
    decltype(auto) boundingBox(this auto&& self)
    {
        return (self.mBoundingBox);
    }

protected:
    /**
     * @brief Serializes the bounding box to the given output stream.
     * @param[out] os: The output stream.
     */
    void serialize(std::ostream& os) const
    {
        mBoundingBox.serialize(os);
    }

    /**
     * @brief Deserializes the bounding box from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is)
    {
        mBoundingBox.deserialize(is);
    }

    template<typename OtherMesh>
    void importFrom(const OtherMesh& m);
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * BoundingBox component (COMP_ID = CompId::BOUNDING_BOX).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasBoundingBox = comp::HasComponent<CompId::BOUNDING_BOX, T>;

/* Implementation of member functions */

template<PointConcept Point>
template<typename OtherMesh>
void BoundingBox<Point>::importFrom(const OtherMesh& m)
{
    if constexpr (HasBoundingBox<OtherMesh>) {
        mBoundingBox = m.boundingBox().template cast<ScalarType>();
    }
}

/* Aliases */

template<typename Scalar>
using BoundingBox3 = BoundingBox<Point3<Scalar>>;

using BoundingBox3f = BoundingBox<Point3f>;
using BoundingBox3d = BoundingBox<Point3d>;

} // namespace vcl::mesh

#endif // VCL_MESH_MESH_COMPONENTS_BOUNDING_BOX_H
