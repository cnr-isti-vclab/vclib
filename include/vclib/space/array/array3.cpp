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

#include "array3.h"

namespace vcl {

template<class T>
Array3D<T>::Array3D() : Array<T, 3>()
{
}

template<class T>
Array3D<T>::Array3D(const Array<T, 3>& a) : Array<T, 3>(a)
{
}

template<class T>
inline Array3D<T>::Array3D(
	unsigned long int sizeX,
	unsigned long int sizeY,
	unsigned long int sizeZ) :
		Array<T, 3>(sizeX, sizeY, sizeZ)
{
}

template<class T>
inline Array3D<T>::Array3D(
	unsigned long int sizeX,
	unsigned long int sizeY,
	unsigned long int sizeZ,
	const T&          value)
{
	Array<T, 3>::v.resize(sizeX * sizeY * sizeZ, value);
	Array<T, 3>::sizes[0] = sizeX;
	Array<T, 3>::sizes[1] = sizeY;
	Array<T, 3>::sizes[2] = sizeZ;
}

template<class T>
inline Array3D<T>::Array3D(vcl::NestedInitializerLists<T, 3> values)
{
	Array<T, 3>::initializeNestedLists(values);
}

template<class T>
inline unsigned long int Array3D<T>::sizeX() const
{
	return Array<T, 3>::sizes[0];
}

template<class T>
inline unsigned long int Array3D<T>::sizeY() const
{
	return Array<T, 3>::sizes[1];
}

template<class T>
inline unsigned long int Array3D<T>::sizeZ() const
{
	return Array<T, 3>::sizes[2];
}

} // namespace vcl
