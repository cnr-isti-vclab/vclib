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

#include "polygon_bit_flags.h"

namespace vcl::comp {

/**
 * @brief  Returns the value of the bit given in input. The bit is checked to be less than the total
 * number of assigned user bits, which in this class is 3.
 *
 * @param[in] bit: The position of the bit that will be returned.
 * @return `true` if the required bit is enabled, false otherwise.
 */
template<typename El, bool h, bool o>
bool PolygonBitFlags<El, h, o>::userBitFlag(uint bit) const
{
	// using FIRST_USER_BIT of this class
	return Base::userBitFlag(bit, FIRST_USER_BIT);
}

/**
 * @brief Sets to `true` the value of the bit given in input. The bit is checked to be less than the
 * total number of assigned user bits, which in this class is 3.
 *
 * @param[in] bit: The position of the bit that will be set.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::setUserBit(uint bit)
{
	// using FIRST_USER_BIT of this class
	Base::setUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief Sets to `false` the value of the bit given in input. The bit is checked to be less than
 * the total number of assigned user bits, which in this class is 3.
 *
 * @param[in] bit: The position of the bit that will be reset.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::unsetUserBit(uint bit)
{
	// using FIRST_USER_BIT of this class
	Base::unsetUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief Returns whether the ith edge of this polygon is marked as on border.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 * @return `true` if the ith edge of the Polygon is on border, false otherwise.
 */
template<typename El, bool h, bool o>
bool PolygonBitFlags<El, h, o>::isEdgeOnBorder(uint i) const
{
	assert(i < 12);
	return Base::flagValue(Base::BORDER << i);
}

/**
 * @brief Returns `true` if at least one edge of this polygon is marked as on border.
 *
 * @return `true` if any edge is on border.
 */
template<typename El, bool h, bool o>
bool PolygonBitFlags<El, h, o>::isAnyEdgeOnBorder() const
{
	for (uint i = 0; i < 12; ++i)
		if (isEdgeOnBorder(i))
			return true;
	return false;
}

template<typename El, bool h, bool o>
bool PolygonBitFlags<El, h, o>::isOnBorder() const
{
	return isAnyEdgeOnBorder();
}

/**
 * @brief Returns whether the ith edge of this polygon is selected.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 * @return `true` if the ith edge is selected.
 */
template<typename El, bool h, bool o>
bool PolygonBitFlags<El, h, o>::isEdgeSelected(uint i) const
{
	assert(i < 12);
	return Base::flagValue(EDGESEL0 << i);
}

/**
 * @brief Returns `true` if at least one edge of this polygon is selected.
 *
 * @return `true` if any edge is selected.
 */
template<typename El, bool h, bool o>
bool PolygonBitFlags<El, h, o>::isAnyEdgeSelected() const
{
	for (uint i = 0; i < 12; ++i)
		if (isEdgeSelected(i))
			return true;
	return false;
}

/**
 * @brief Returns whether the ith edge of this polygon is marked as faux.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 3. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 * @return `true` if the ith edge is faux.
 */
template<typename El, bool h, bool o>
bool PolygonBitFlags<El, h, o>::isEdgeFaux(uint i) const
{
	assert(i < 3);
	return Base::flagValue(FAUX0 << i);
}

/**
 * @brief Returns `true` if at least one edge of this polygon is marked as faux.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 *
 * @return `true` if any edge is selected.
 */
template<typename El, bool h, bool o>
bool PolygonBitFlags<El, h, o>::isAnyEdgeFaux() const
{
	return isEdgeFaux(0) || isEdgeFaux(1) || isEdgeFaux(2);
}

/**
 * @brief Sets the ith edge of this polygon as on border.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::setEdgeOnBorder(uint i)
{
	assert(i < 12);
	Base::setFlag(Base::BORDER << i);
}

/**
 * @brief Sets the ith edge of this polygon as selected.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::setEdgeSelected(uint i)
{
	assert(i < 12);
	Base::setFlag(EDGESEL0 << i);
}

/**
 * @brief Sets the ith edge of this polygon as faux.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 3. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::setEdgeFaux(uint i)
{
	assert(i < 3);
	Base::setFlag(FAUX0 << i);
}

/**
 * @brief Marks as non-on-border the ith edge of this polygon.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::unsetEdgeOnBorder(uint i)
{
	assert(i < 12);
	Base::unsetFlag(Base::BORDER << i);
}

/**
 * @brief Marks as non-on-border alle the edges of this polygon.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::unsetAllEdgesOnBorder()
{
	for (uint i = 0; i < 12; ++i)
		unsetEdgeOnBorder(i);
}

/**
 * @brief Marks as non-selected the ith edge of this polygon.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::unsetEdgeSelected(uint i)
{
	assert(i < 12);
	Base::unsetFlag(EDGESEL0 << i);
}

/**
 * @brief Marks as non-selected alle the edges of this polygon.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::unsetAllEdgesSelected()
{
	for (uint i = 0; i < 12; ++i)
		unsetEdgeSelected(i);
}

/**
 * @brief Marks as non-faux the ith edge of this polygon.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 3. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::unsetEdgeFaux(uint i)
{
	assert(i < 3);
	Base::unsetFlag(FAUX0 << i);
}

/**
 * @brief Marks as non-faux alle the edges of this polygon.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 */
template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::unsetAllEdgesFaux()
{
	unsetEdgeFaux(0);
	unsetEdgeFaux(1);
	unsetEdgeFaux(2);
}

template<typename El, bool h, bool o>
void PolygonBitFlags<El, h, o>::importFromVCGFlags(int f)
{
	if (f & 0x00000020)
		Base::setSelected();
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

template<typename El, bool h, bool o>
int PolygonBitFlags<El, h, o>::exportToVCGFlags() const
{
	int f = 0;
	if (Base::isSelected())
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

template<typename El, bool h, bool o>
template<typename Element>
void PolygonBitFlags<El, h, o>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		Base::unsetAllFlags();
		if constexpr (HasPolygonBitFlags<Element>) {
			Base::flags() = e.flags();
		}
		else if constexpr (HasTriangleBitFlags<Element>) {
			// todo
		}
		else {
			// todo
		}
	}
}

} // namespace vcl::comp
