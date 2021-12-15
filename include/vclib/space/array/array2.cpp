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

#include "array2.h"

namespace vcl {

template<class T>
Array2<T>::Array2() : Array<T, 2>()
{
}

template<class T>
Array2<T>::Array2(const Array<T, 2>& a) : Array<T, 2>(a)
{
}

template<class T>
Array2<T>::Array2(unsigned long int sizeX, unsigned long int sizeY) :
		Array<T, 2>(sizeX, sizeY)
{
}

/**
 * @brief Initializes a 2D array with sizeX rows and sizeY columns.
 * All the values of the array will be initialized to the value passed as a parameter
 *
 * @param sizeX
 * @param sizeY
 * @param value
 */
template<class T>
Array2<T>::Array2(unsigned long int sizeX, unsigned long int sizeY, const T& value)
{
	Array<T, 2>::v.resize(sizeX * sizeY, value);
	Array<T, 2>::sizes[0] = sizeX;
	Array<T, 2>::sizes[1] = sizeY;
}

template<class T>
Array2<T>::Array2(NestedInitializerLists<T, 2> values)
{
	Array<T, 2>::initializeNestedLists(values);
}

/**
 * @brief Returns the number of rows of the array
 *
 * @return number of rows
 */
template<class T>
unsigned long int Array2<T>::sizeX() const
{
	return Array<T, 2>::sizes[0];
}

/**
 * @brief Returns the number of columns of the array
 *
 * @return number of columns
 */
template<class T>
unsigned long int Array2<T>::sizeY() const
{
	return Array<T, 2>::sizes[1];
}

/**
 * @brief Returns the number of rows of the array
 *
 * @return number of rows
 */
template<class T>
unsigned long Array2<T>::rows() const
{
	return Array<T, 2>::sizes[0];
}

/**
 * @brief Returns the number of columns of the array
 *
 * @return number of columns
 */
template<class T>
unsigned long Array2<T>::cols() const
{
	return Array<T, 2>::sizes[1];
}

template<class T>
std::ostream& operator<<(std::ostream& inputStream, const Array2<T>& a)
{
	for (unsigned int i = 0; i < a.sizeX(); i++) {
		for (unsigned int j = 0; j < a.sizeY(); j++) {
			inputStream << std::setw(4) << a(i, j) << " ";
		}
		inputStream << "\n";
	}
	return inputStream;
}

} // namespace vcl
