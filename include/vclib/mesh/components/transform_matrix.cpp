/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "transform_matrix.h"

namespace vcl::comp {

/**
 * @brief Constructor that initializes the transform matrix to identity.
 */
template<typename Scalar, typename El, bool O>
TransformMatrix<Scalar, El, O>::TransformMatrix()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

/**
 * @private
 * @brief Initializes transform matrix to identity.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename Scalar, typename El, bool O>
void TransformMatrix<Scalar, El, O>::init()
{
	transformMatrix().setIdentity();
}

/**
 * @private
 * @brief Returns `true` if the component is available, `false` otherwise.
 *
 * This member function can return `false` only if the component is optional,
 * and it is not enabled.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is available, `false` otherwise.
 */
template<typename Scalar, typename El, bool O>
bool TransformMatrix<Scalar, El, O>::isAvailable() const
{
	return Base::isAvailable(this);
}

/**
 * @brief Returns a const reference to the transform matrix.
 * @return A const reference to the transform matrix.
 */
template<typename Scalar, typename El, bool O>
auto TransformMatrix<Scalar, El, O>::transformMatrix() const
	-> const TransformMatrixType&
{
	return Base::data(this);
}

/**
 * @brief Returns a reference to the transform matrix.
 * @return A reference to the transform matrix.
 */
template<typename Scalar, typename El, bool O>
auto TransformMatrix<Scalar, El, O>::transformMatrix() -> TransformMatrixType&
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool O>
template<typename Element>
void TransformMatrix<Scalar, El, O>::importFrom(const Element& e)
{
	if constexpr(HasTransformMatrix<Element>) {
		transformMatrix() = e.transformMatrix().template cast<Scalar>();
	}
}

/**
 * @brief Checks if the given Element/Mesh has TransformMatrix component
 * available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element/mesh to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element/mesh has TransformMatrix component available,
 * `false` otherwise.
 */
bool isTransformMatrixAvailableOn(const ElementOrMeshConcept auto& element)
{
	return isComponentAvailableOn<TRANSFORM_MATRIX>(element);
}

} // namespace vcl::comp
