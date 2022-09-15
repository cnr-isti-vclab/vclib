/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_ARRAY3_H
#define VCL_ARRAY3_H

#include "array_bool.h"

namespace vcl {

/**
 * @brief The Array3 class
 * Specialization of vcl::Array class with 3 dimensions.
 */
template<typename T>
class Array3D : public Array<T, 3>
{
public:
	Array3D();
	Array3D(const Array<T, 3>& a);
	Array3D(unsigned long int sizeX, unsigned long int sizeY, unsigned long int sizeZ);
	Array3D(
		unsigned long int sizeX,
		unsigned long int sizeY,
		unsigned long int sizeZ,
		const T&          value);
	Array3D(vcl::NestedInitializerLists<T, 3> values);
	unsigned long int sizeX() const;
	unsigned long int sizeY() const;
	unsigned long int sizeZ() const;
};

} // namespace vcl

#include "array3.cpp"

#endif // VCL_ARRAY3_H
