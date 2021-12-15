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

#include "array4.h"

namespace vcl {

template<typename T>
Array4D<T>::Array4D() : Array<T, 4>()
{
}

template<typename T>
Array4D<T>::Array4D(const Array<T, 4>& a) : Array<T, 4>(a)
{
}

template<class T>
inline Array4D<T>::Array4D(
	unsigned long int sizeX,
	unsigned long int sizeY,
	unsigned long int sizeZ,
	unsigned long int sizeW) :
		Array<T, 4>(sizeX, sizeY, sizeZ, sizeW)
{
}

template<class T>
inline Array4D<T>::Array4D(
	unsigned long int sizeX,
	unsigned long int sizeY,
	unsigned long int sizeZ,
	unsigned long int sizeW,
	const T&          value)
{
	Array<T, 4>::v.resize(sizeX * sizeY * sizeZ * sizeW, value);
	Array<T, 4>::sizes[0] = sizeX;
	Array<T, 4>::sizes[1] = sizeY;
	Array<T, 4>::sizes[2] = sizeZ;
	Array<T, 4>::sizes[3] = sizeW;
}

template<class T>
inline Array4D<T>::Array4D(vcl::NestedInitializerLists<T, 4> values)
{
	Array<T, 4>::initializeNestedLists(values);
}

template<class T>
inline unsigned long int Array4D<T>::sizeX() const
{
	return Array<T, 4>::sizes[0];
}

template<class T>
inline unsigned long int Array4D<T>::sizeY() const
{
	return Array<T, 4>::sizes[1];
}

template<class T>
inline unsigned long int Array4D<T>::sizeZ() const
{
	return Array<T, 4>::sizes[2];
}

template<class T>
inline unsigned long int Array4D<T>::sizeW() const
{
	return Array<T, 4>::sizes[3];
}

} // namespace vcl
