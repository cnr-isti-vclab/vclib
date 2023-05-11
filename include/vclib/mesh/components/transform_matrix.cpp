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

template<typename Scalar, typename El, bool O>
TransformMatrix<Scalar, El, O>::TransformMatrix()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

template<typename Scalar, typename El, bool O>
void TransformMatrix<Scalar, El, O>::init()
{
	transformMatrix().setIdentity();
}

template<typename Scalar, typename El, bool O>
bool TransformMatrix<Scalar, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename Scalar, typename El, bool O>
const typename TransformMatrix<Scalar, El, O>::TransformMatrixType&
TransformMatrix<Scalar, El, O>::transformMatrix() const
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool O>
typename TransformMatrix<Scalar, El, O>::TransformMatrixType&
TransformMatrix<Scalar, El, O>::transformMatrix()
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

} // namespace vcl::comp
