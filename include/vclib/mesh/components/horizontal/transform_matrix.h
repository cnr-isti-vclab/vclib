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

#include "../concepts/transform_matrix.h"

namespace vcl::comp {

template<typename Scalar>
class TransformMatrix
{
public:
	using TransformMatrixType = Matrix44<Scalar>;

	TransformMatrix();

	const TransformMatrixType& transformMatrix() const;
	TransformMatrixType&       transformMatrix();

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	Matrix44<Scalar> tr;
};

} // namespace vcl::comp

#include "transform_matrix.cpp"

#endif // VCL_MESH_COMPONENTS_TRANSFORM_MATRIX_H
