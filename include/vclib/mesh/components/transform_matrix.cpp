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

template<typename Scalar, typename El, bool o>
TransformMatrix<Scalar, El, o>::TransformMatrix()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

template<typename Scalar, typename El, bool o>
void TransformMatrix<Scalar, El, o>::init()
{
	transformMatrix().setIdentity();
}

template<typename Scalar, typename El, bool o>
bool TransformMatrix<Scalar, El, o>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename Scalar, typename El, bool o>
const typename TransformMatrix<Scalar, El, o>::TransformMatrixType&
TransformMatrix<Scalar, El, o>::transformMatrix() const
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool o>
typename TransformMatrix<Scalar, El, o>::TransformMatrixType&
TransformMatrix<Scalar, El, o>::transformMatrix()
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool o>
template<typename Element>
void TransformMatrix<Scalar, El, o>::importFrom(const Element& e)
{
	if constexpr(HasTransformMatrix<Element>) {
		transformMatrix() = e.transformMatrix().template cast<Scalar>();
	}
}

} // namespace vcl::comp
