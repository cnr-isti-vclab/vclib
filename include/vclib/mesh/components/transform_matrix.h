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
#include "internal/get_vertical_component_data.h"

namespace vcl::comp {

namespace internal {

template<typename Scalar, bool>
struct  TransformMatrixData { Matrix44<Scalar> tr; };

template<typename Scalar>
struct TransformMatrixData<Scalar, false> { };

} // vcl::comp::internal

template<typename Scalar, typename ElementType, bool horizontal>
class TransformMatrix
{
	using ThisType = TransformMatrix<Scalar, ElementType, horizontal>;
public:
	// data that the component stores internally (or vertically)
	using DataValueType = Matrix44<Scalar>;
	using TransformMatrixComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !horizontal;

	using TransformMatrixType = Matrix44<Scalar>;

	TransformMatrix();

	const TransformMatrixType& transformMatrix() const;
	TransformMatrixType&       transformMatrix();

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the matrix, trough data (horizontal) or trough parent (vertical)
	Matrix44<Scalar>& tr();
	const Matrix44<Scalar>& tr() const;

	// contians the actual scalar, if the component is horizontal
	internal::TransformMatrixData<Scalar, horizontal> data;
};

} // namespace vcl::comp

#include "transform_matrix.cpp"

#endif // VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H
