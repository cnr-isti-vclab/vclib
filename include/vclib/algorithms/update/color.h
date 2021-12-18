/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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

#ifndef VCL_ALGORITHMS_COLOR_H
#define VCL_ALGORITHMS_COLOR_H

#include <vclib/space/color.h>

namespace vcl {

template<typename MeshType>
void setPerVertexColor(MeshType& m, vcl::Color c = vcl::Color::White, bool onlySelected = false);

template<typename MeshType>
void setPerFaceColor(MeshType& m, vcl::Color c = vcl::Color::White, bool onlySelected = false);

template<typename MeshType>
void setPerVertexColorFromFaceColor(MeshType& m);

template<typename MeshType>
void setPerFaceColorFromVertexColor(MeshType& m);

template<typename MeshType>
void setPerVertexColorFromScalar(
	MeshType&                                 m,
	vcl::Color::ColorMap                      colorMap = vcl::Color::RedBlue,
	typename MeshType::VertexType::ScalarType minScalar = 0,
	typename MeshType::VertexType::ScalarType maxScalar = 0);

template<typename MeshType>
void setPerFaceColorFromScalar(
	MeshType&                                 m,
	vcl::Color::ColorMap                      colorMap = vcl::Color::RedBlue,
	typename MeshType::FaceType::ScalarType minScalar = 0,
	typename MeshType::FaceType::ScalarType maxScalar = 0);
}

#include "color.cpp"

#endif // VCL_ALGORITHMS_COLOR_H
