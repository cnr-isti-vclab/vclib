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

#ifndef VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H
#define VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H

#include "base/component.h"
#include "concepts/predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The TransformMatrix class represents a component that stores a 4x4
 * matrix that can be used for a transformation. This class is usually used
 * as a component of a Mesh.
 *
 * The member functions of this class will be available in the instance of any
 * Element or Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the TransformMatrix component,
 * you'll be able to access to this component member functions from `m`:
 *
 * @code{.cpp}
 * m.transformMatrix();
 * @endcode
 *
 * @note This component can be both used for Elements and Meshes.
 *
 * @tparam Scalar: The type of the scalar that will be used to represent the
 * matrix.
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Scalar, typename ParentElemType = void, bool OPT = false>
class TransformMatrix :
        public Component<
            TransformMatrix<Scalar, ParentElemType, OPT>,
            CompId::TRANSFORM_MATRIX,
            Matrix44<Scalar>,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        TransformMatrix<Scalar, ParentElemType, OPT>,
        CompId::TRANSFORM_MATRIX,
        Matrix44<Scalar>,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /**
     * @brief Expose the type of the transform matrix.
     */
    using TransformMatrixType = Matrix44<Scalar>;

    /* Constructors */

    /**
     * @brief Initializes the transform matrix to identity.
     */
    TransformMatrix()
    {
        if constexpr (!Base::IS_VERTICAL) {
            init();
        }
    }

    /**
     * @private
     * @brief Initializes transform matrix to identity.
     *
     * It is made in the init function since the component could be not
     * available during construction (e.g. if the component is optional and not
     * enabled).
     *
     * This member function is hidden by the element that inherits this class.
     */
    void init() { transformMatrix().setIdentity(); }

    /* Member functions */

    /**
     * @brief Returns a const reference to the transform matrix.
     * @return A const reference to the transform matrix.
     */
    const TransformMatrixType& transformMatrix() const { return Base::data(); }

    /**
     * @brief Returns a reference to the transform matrix.
     * @return A reference to the transform matrix.
     */
    TransformMatrixType& transformMatrix() { return Base::data(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const { transformMatrix().serialize(os); }

    void deserialize(std::istream& is) { transformMatrix().deserialize(is); }
};

/* concept */

/**
 * @brief A concept that checks whether a type T (that should be a Mesh)
 * has the TransformMatrix component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::TransformMatrix, with any template arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasTransformMatrix.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTransformMatrix =
    TTB::IsDerivedFromSpecializationOfV<T, TransformMatrix>;

/* importFrom function */

template<typename Scalar, typename ParentElemType, bool OPT>
template<typename Element>
void TransformMatrix<Scalar, ParentElemType, OPT>::importFrom(
    const Element& e,
    bool)
{
    if constexpr (HasTransformMatrix<Element>) {
        transformMatrix() = e.transformMatrix().template cast<Scalar>();
    }
}

/* Detector function to check if a class has TransformMatrix available */

/**
 * @brief Checks if the given Element has TransformMatrix component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has TransformMatrix component available,
 * `false` otherwise.
 */
bool isTransformMatrixAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::TRANSFORM_MATRIX>(element);
}

/* Specialization Aliases */

/**
 * The TransformMatrixf class is an alias of the TransformMatrix component that
 * uses float as scalar type.
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
using TransformMatrixf = TransformMatrix<float, ElementType, OPT>;

/**
 * The TransformMatrixd class is an alias of the TransformMatrix component that
 * uses double as scalar type.
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
using TransformMatrixd = TransformMatrix<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H
