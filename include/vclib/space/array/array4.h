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

#ifndef VCL_ARRAY4_H
#define VCL_ARRAY4_H

#include "array_bool.h"

namespace vcl {

/**
 * @brief The Array4 class
 * Specialization of vcl::Array class with 4 dimensions.
 */
template<typename T>
class Array4D : public Array<T, 4>
{
public:
	Array4D();
	Array4D(const Array<T, 4>& a);
	Array4D(
		unsigned long int sizeX,
		unsigned long int sizeY,
		unsigned long int sizeZ,
		unsigned long int sizeW);
	Array4D(
		unsigned long int sizeX,
		unsigned long int sizeY,
		unsigned long int sizeZ,
		unsigned long int sizeW,
		const T&          value);
	Array4D(vcl::NestedInitializerLists<T, 4> values);
	unsigned long int sizeX() const;
	unsigned long int sizeY() const;
	unsigned long int sizeZ() const;
	unsigned long int sizeW() const;
};

} // namespace vcl

#include "array4.cpp"

#endif // VCL_ARRAY4_H
