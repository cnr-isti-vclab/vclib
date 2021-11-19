/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_COMPONENTS_TEX_COORD_H
#define VCL_MESH_COMPONENTS_TEX_COORD_H

#include <vclib/space/tex_coord.h>

namespace vcl::comp {

class TexCoordTriggerer
{
};

template<typename Scalar>
class TexCoord : public TexCoordTriggerer
{
public:
	using TexCoordType = vcl::TexCoord<Scalar>;

	const TexCoordType& texCoord() const { return t; }
	TexCoordType        texCoord() { return t; }

private:
	vcl::TexCoord<Scalar> t;
};

using TexCoordf = TexCoord<float>;
using TexCoordd = TexCoord<double>;

/**
 * Detector to check if a class has (inherits) TexCoord
 */

template<typename T>
using hasTexCoordT = std::is_base_of<TexCoordTriggerer, T>;

template<typename T>
bool constexpr hasTexCoord()
{
	return hasTexCoordT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TEXCOORD_H
