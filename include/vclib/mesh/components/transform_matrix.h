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

#ifndef VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H
#define VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H

#include <vclib/concepts/mesh/components/transform_matrix.h>
#include <vclib/space/matrix.h>

#include "component.h"

namespace vcl::comp {

/**
 * @brief The TransformMatrix class
 *
 * @ingroup components
 */
template<typename Scalar, typename ElementType = void, bool optional = false>
class TransformMatrix : public Component<Matrix44<Scalar>, ElementType, optional>
{
	using Base = Component<Matrix44<Scalar>, ElementType, optional>;
	using ThisType = TransformMatrix<Scalar, ElementType, optional>;

public:
	using TransformMatrixComponent = ThisType; // expose the type to allow access to this component

	using TransformMatrixType = Matrix44<Scalar>;

	/* Constructor and isEnabled */

	TransformMatrix();

	void init();

	bool isEnabled() const;

	/* Member functions */

	const TransformMatrixType& transformMatrix() const;
	TransformMatrixType&       transformMatrix();

protected:
	template<typename Element>
	void importFrom(const Element& e);
};

} // namespace vcl::comp

#include "transform_matrix.cpp"

#endif // VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H
