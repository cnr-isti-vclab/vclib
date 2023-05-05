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

#include "triangle_bit_flags.h"

namespace vcl::comp {

/**
 * @brief Returns the value of the bit given in input. The bit is checked to be less than the
 * total number of assigned user bits, which in this class is 21.
 *
 * @param[in] bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
template<typename El, bool o>
bool TriangleBitFlags<El, o>::userBitFlag(uint bit) const
{
	// using FIRST_USER_BIT of this class
	return Base::userBitFlag(bit, FIRST_USER_BIT);
}

/**
 * @brief Sets to true the value of the bit given in input. The bit is
 * checked to be less than the total number of assigned user bits, which in this class is 21.
 *
 * @param bit: the position of the bit that will be set.
 */
template<typename El, bool o>
void TriangleBitFlags<El, o>::setUserBit(uint bit)
{
	// using FIRST_USER_BIT of this class
	Base::setUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief Sets to false the value of the bit given in input. The bit
 * is checked to be less than the total number of assigned user bits, which in this class is 21.
 *
 * @param bit: the position of the bit that will be reset.
 */
template<typename El, bool o>
void TriangleBitFlags<El, o>::unsetUserBit(uint bit)
{
	// using FIRST_USER_BIT of this class
	Base::unsetUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief Returns whether the ith Edge of the Triangle is marked as
 * on border.
 *
 * @param[in] i: id of the edge, must be < 3.
 * @return true if the ith Edge of the Triangle is on border, false otherwise.
 */
template<typename El, bool o>
bool TriangleBitFlags<El, o>::isEdgeOnBorder(uint i) const
{
	assert(i < 3);
	return Base::flagValue(Base::BORDER + i);
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::isAnyEdgeOnBorder() const
{
	return isEdgeOnBorder(0) || isEdgeOnBorder(1) || isEdgeOnBorder(2);
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::isOnBorder() const
{
	return isAnyEdgeOnBorder();
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::isEdgeSelected(uint i) const
{
	assert(i < 3);
	return Base::flagValue(EDGESEL0 + i);
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::isAnyEdgeSelected() const
{
	return isEdgeSelected(0) || isEdgeSelected(1) || isEdgeSelected(2);
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::isEdgeFaux(uint i) const
{
	assert(i < 3);
	return Base::flagValue(FAUX0 + i);
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::isAnyEdgeFaux() const
{
	return isEdgeFaux(0) || isEdgeFaux(1) || isEdgeFaux(2);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::setEdgeOnBorder(uint i)
{
	assert(i < 3);
	Base::setFlag(Base::BORDER + i);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::setEdgeSelected(uint i)
{
	assert(i < 3);
	Base::setFlag(EDGESEL0 + i);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::setEdgeFaux(uint i)
{
	assert(i < 3);
	Base::setFlag(FAUX0 + i);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::unsetEdgeOnBorder(uint i)
{
	assert(i < 3);
	Base::unsetFlag(Base::BORDER << i);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::unsetAllEdgesOnBorder()
{
	unsetEdgeOnBorder(0);
	unsetEdgeOnBorder(1);
	unsetEdgeOnBorder(2);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::unsetEdgeSelected(uint i)
{
	assert(i < 3);
	Base::unsetFlag(EDGESEL0 << i);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::unsetAllEdgesSelected()
{
	unsetEdgeSelected(0);
	unsetEdgeSelected(1);
	unsetEdgeSelected(2);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::unsetEdgeFaux(uint i)
{
	assert(i < 3);
	Base::unsetFlag(FAUX0 << i);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::unsetAllEdgesFaux()
{
	unsetEdgeFaux(0);
	unsetEdgeFaux(1);
	unsetEdgeFaux(2);
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::importFromVCGFlags(int f)
{
	if (f & 0x00000020)
		Base::selected() = true;
	if (f & 0x00000040)
		setEdgeOnBorder(0);
	if (f & 0x00000080)
		setEdgeOnBorder(1);
	if (f & 0x00000100)
		setEdgeOnBorder(2);
	if (f & 0x00008000)
		setEdgeSelected(0);
	if (f & 0x00010000)
		setEdgeSelected(1);
	if (f & 0x00020000)
		setEdgeSelected(2);
	if (f & 0x00040000)
		setEdgeFaux(0);
	if (f & 0x00080000)
		setEdgeFaux(1);
	if (f & 0x00100000)
		setEdgeFaux(2);
}

template<typename El, bool o>
int TriangleBitFlags<El, o>::exportToVCGFlags() const
{
	int f = 0;
	if (Base::selected())
		f &= 0x00000020;
	if (isEdgeOnBorder(0))
		f &= 0x00000040;
	if (isEdgeOnBorder(1))
		f &= 0x00000080;
	if (isEdgeOnBorder(2))
		f &= 0x00000100;
	if (isEdgeSelected(0))
		f &= 0x00008000;
	if (isEdgeSelected(1))
		f &= 0x00010000;
	if (isEdgeSelected(2))
		f &= 0x00020000;
	if (isEdgeFaux(0))
		f &= 0x00040000;
	if (isEdgeFaux(1))
		f &= 0x00080000;
	if (isEdgeFaux(2))
		f &= 0x00100000;
	return f;
}

template<typename El, bool o>
template<typename Element>
void TriangleBitFlags<El, o>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		Base::unsetAllFlags();
		if constexpr (HasPolygonBitFlags<Element>) {
			// todo
		}
		else if constexpr (HasTriangleBitFlags<Element>) {
			Base::flags() = e.flags();
		}
		else {
			// todo
		}
	}
}

} // namespace vcl::comp
