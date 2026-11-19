// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_MESH_COMPONENTS_TRANSFORM_MATRIX_H
#define VCL_MESH_MESH_COMPONENTS_TRANSFORM_MATRIX_H

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/base/component_predicates.h>

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <istream>
#include <ostream>

namespace vcl::mesh {

/**
 * @brief The TransformMatrix class represents a component that stores a 4x4
 * matrix that can be used for an affine transformation. This class is used
 * as a component of a Mesh.
 *
 * The member functions of this class will be available in the instance of any
 * Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the TransformMatrix component, you can
 * access and modify its transform matrix:
 *
 * @code{.cpp}
 * m.transformMatrix().setIdentity();
 * auto mat = m.transformMatrix();
 * @endcode
 *
 * @tparam Scalar: The type of the scalar that will be used to represent the
 * matrix (default: float).
 *
 * @ingroup mesh_components
 */
template<typename Scalar = float>
class TransformMatrix :
        public comp::Component<CompId::TRANSFORM_MATRIX, Matrix44<Scalar>>
{
    Matrix44<Scalar> mTransformMatrix = Matrix44<Scalar>::Identity();

public:
    /**
     * @brief Expose the scalar type of the transform matrix.
     */
    using ScalarType = Scalar;

    /**
     * @brief Expose the type of the transform matrix.
     */
    using TransformMatrixType = Matrix44<Scalar>;

    /**
     * @brief Default constructor. Initializes the transform matrix to identity.
     */
    TransformMatrix() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the transform matrix of the mesh.
     * @return A reference to the transform matrix of the mesh.
     */
    decltype(auto) transformMatrix(this auto&& self)
    {
        return (self.mTransformMatrix);
    }

protected:
    /**
     * @brief Serializes the transform matrix to the given output stream.
     * @param[out] os: The output stream.
     */
    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mTransformMatrix);
    }

    /**
     * @brief Deserializes the transform matrix from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mTransformMatrix);
    }

    template<typename OtherMesh>
    void importFrom(const OtherMesh& m);
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * TransformMatrix component (COMP_ID = CompId::TRANSFORM_MATRIX).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTransformMatrix = comp::HasComponent<CompId::TRANSFORM_MATRIX, T>;

/* Implementation of member functions */

template<typename Scalar>
template<typename OtherMesh>
void TransformMatrix<Scalar>::importFrom(const OtherMesh& m)
{
    if constexpr (HasTransformMatrix<OtherMesh>) {
        mTransformMatrix = m.transformMatrix().template cast<Scalar>();
    }
}

/* Aliases */

using TransformMatrixf = TransformMatrix<float>;
using TransformMatrixd = TransformMatrix<double>;

} // namespace vcl::mesh

#endif // VCL_MESH_MESH_COMPONENTS_TRANSFORM_MATRIX_H
