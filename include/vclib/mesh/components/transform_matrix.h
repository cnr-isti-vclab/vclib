/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include <vclib/math/matrix.h>

#include "concepts/transform_matrix.h"
#include "internal/component_data.h"

namespace vcl::comp {

template<typename Scalar, typename ElementType = void, bool optional = false>
class TransformMatrix
{
	using ThisType = TransformMatrix<Scalar, ElementType, optional>;
public:
	// data that the component stores internally (or vertically)
	using DataValueType = Matrix44<Scalar>;
	using TransformMatrixComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

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

private:
	// members that allow to access the matrix, trough data (horizontal) or trough parent (vertical)
	Matrix44<Scalar>& tr();
	const Matrix44<Scalar>& tr() const;

	// contians the actual data of the component, if the component is horizontal
	internal::ComponentData<DataValueType, IS_VERTICAL> data;
};

} // namespace vcl::comp

#include "transform_matrix.cpp"

#endif // VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H