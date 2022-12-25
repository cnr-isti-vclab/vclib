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

#ifndef VCL_SPACE_ARRAY_ARRAY2_H
#define VCL_SPACE_ARRAY_ARRAY2_H

#include <iomanip>

#include "array_bool.h"

namespace vcl {

/**
 * @brief The Array2 class
 * Specialization of vcl::Array class with 2 dimensions.
 */
template<typename T>
class Array2 : public Array<T, 2>
{
public:
	Array2();
	Array2(const Array<T, 2>& a);
	Array2(unsigned long int sizeX, unsigned long int sizeY);
	Array2(unsigned long int sizeX, unsigned long int sizeY, const T& value);
	Array2(vcl::NestedInitializerLists<T, 2> values);

	unsigned long int sizeX() const;
	unsigned long int sizeY() const;
	unsigned long int rows() const;
	unsigned long int cols() const;
};

template<class T>
std::ostream& operator<<(std::ostream& inputStream, const Array2<T>& a);

} // namespace vcl

#include "array2.cpp"

#endif // VCL_SPACE_ARRAY_ARRAY2_H
