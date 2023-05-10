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

template<typename El, bool o>
PolygonBitFlags<El, o>::PolygonBitFlags()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

template<typename El, bool o>
void PolygonBitFlags<El, o>::init()
{
	flags().reset();
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::isEnabled() const
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns whether the current Polygon is deleted or not.
 * @return true if the Polygon is deleted, false otherwise.
 */
template<typename El, bool o>
bool PolygonBitFlags<El, o>::deleted() const
{
	return flags()[DELETED];
}

/**
 * @brief Accesses the 'selected' bit of this Polygon, returning a reference to it.
 * @return a reference to the 'selected' bit of this Polygon.
 */
template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::selected()
{
	return flags()[SELECTED];
}

/**
 * @brief Returns whether the current Polygon is selected or not.
 * @return true if the Polygon is selected, false otherwise.
 */
template<typename El, bool o>
bool PolygonBitFlags<El, o>::selected() const
{
	return flags()[SELECTED];
}

/**
 * @brief Accesses the 'visited' bit of this Polygon, returning a reference to it.
 * @return a reference to the 'visited' bit of this Polygon.
 */
template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::visited()
{
	return flags()[VISITED];
}

/**
 * @brief Returns whether the current Polygon has been visited or not.
 * @return true if the Polygon has been visited, false otherwise.
 */
template<typename El, bool o>
bool PolygonBitFlags<El, o>::visited() const
{
	return flags()[VISITED];
}

/**
 * @brief Returns whether the current Polygon is on border or not, by checking whether at least one
 * of its edges are on border or not.
 * @return true if at least one edge of the Polygon is on border, false otherwise.
 */
template<typename El, bool o>
bool PolygonBitFlags<El, o>::onBorder() const
{
	for (uint i = 0; i < 12; ++i)
		if (edgeOnBorder(i)) return true;
	return false;
}

template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::edgeOnBorder(uint i)
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGEBORD];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::edgeOnBorder(uint i) const
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGEBORD];
}

template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::edgeSelected(uint i)
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGESEL];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::edgeSelected(uint i) const
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGESEL];
}

template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::edgeVisited(uint i)
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGEVIS];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::edgeVisited(uint i) const
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGEVIS];
}

template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::edgeFaux(uint i)
{
	assert(i < 3);
	return flags()[FAUX0 + i];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::edgeFaux(uint i) const
{
	assert(i < 3);
	return flags()[FAUX0 + i];
}

/**
 * @brief Returns a reference to the value of the user bit of this Polygon given in input. The bit
 * is checked to be less than the total number of assigned user bits, which in this class is 2.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 1].
 * @return a reference to the desired user bit.
 */
template<typename El, bool o>
bool PolygonBitFlags<El, o>::userBit(uint bit) const
{
	assert(bit < N_USER_BITS);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Returns the boolean value of the user bit of this Polygon given in input. The bit
 * is checked to be less than the total number of assigned user bits, which in this class is 2.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 1], that will be returned by
 * reference.
 * @return `true` if the required bit is enabled, `false` otherwise.
 */
template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::userBit(uint bit)
{
	assert(bit < N_USER_BITS);
	return flags()[bit + FIRST_USER_BIT];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::edgeUserBit(uint i, uint bit) const
{
	assert(bit < N_EDGE_USER_BITS);
	return edgeFlags()[i][bit + FIRST_EDGE_USER_BIT];
}

template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::edgeUserBit(uint i, uint bit)
{
	assert(bit < N_EDGE_USER_BITS);
	return edgeFlags()[i][bit + FIRST_EDGE_USER_BIT];
}

/**
 * @brief Unsets all the flags of this Polygon and sets them to `false`, **except the deleted
 * flag**, which needs to be manually reset.
 */
template<typename El, bool o>
void PolygonBitFlags<El, o>::resetBitFlags()
{
	bool isD = deleted();
	flags().reset();
	for (uint i = 0; i < edgeFlags().size(); ++i)
		edgeFlags()[i].reset();
	deleted() = isD;
}

template<typename El, bool o>
void PolygonBitFlags<El, o>::importFromVCGFlags(int f)
{
	if (f & 0x00000020)
		selected() = true;
	if (f & 0x00000040)
		edgeOnBorder(0) = true;
	if (f & 0x00000080)
		edgeOnBorder(1) = true;
	if (f & 0x00000100)
		edgeOnBorder(2) = true;
	if (f & 0x00008000)
		edgeSelected(0) = true;
	if (f & 0x00010000)
		edgeSelected(1) = true;
	if (f & 0x00020000)
		edgeSelected(2) = true;
	if (f & 0x00040000)
		edgeFaux(0) = true;
	if (f & 0x00080000)
		edgeFaux(1) = true;
	if (f & 0x00100000)
		edgeFaux(2) = true;
}

template<typename El, bool o>
int PolygonBitFlags<El, o>::exportToVCGFlags() const
{
	int f = 0;
	if (selected())
		f &= 0x00000020;
	if (edgeOnBorder(0))
		f &= 0x00000040;
	if (edgeOnBorder(1))
		f &= 0x00000080;
	if (edgeOnBorder(2))
		f &= 0x00000100;
	if (edgeSelected(0))
		f &= 0x00008000;
	if (edgeSelected(1))
		f &= 0x00010000;
	if (edgeSelected(2))
		f &= 0x00020000;
	if (edgeFaux(0))
		f &= 0x00040000;
	if (edgeFaux(1))
		f &= 0x00080000;
	if (edgeFaux(2))
		f &= 0x00100000;
	return f;
}

template<typename ElementType, bool optional>
void PolygonBitFlags<ElementType, optional>::resizeBitFlags(uint n)
{
	edgeFlags().resize(n);
}

template<typename ElementType, bool optional>
void PolygonBitFlags<ElementType, optional>::pushBitFlag(BitSet<FT> f)
{
	edgeFlags().pushBack(f);
}

template<typename ElementType, bool optional>
void PolygonBitFlags<ElementType, optional>::insertBitFlag(uint i, BitSet<FT> f)
{
	edgeFlags().insert(edgeFlags().begin() + i, f);
}

template<typename ElementType, bool optional>
void PolygonBitFlags<ElementType, optional>::eraseBitFlag(uint i)
{
	edgeFlags().erase(edgeFlags().begin() + i);
}

template<typename ElementType, bool optional>
void PolygonBitFlags<ElementType, optional>::clearBitFlags()
{
	edgeFlags().clear();
}

template<typename El, bool o>
BitProxy<typename PolygonBitFlags<El, o>::FT> PolygonBitFlags<El, o>::deleted()
{
	return flags()[DELETED];
}

template<typename El, bool o>
template<typename Element>
void PolygonBitFlags<El, o>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		resetBitFlags();
		if constexpr (HasPolygonBitFlags<Element>) {
			flags() = e.flags();
			edgeFlags() = e.edgeFlags();
		}
		else if constexpr (HasTriangleBitFlags<Element>) {
			// TODO
		}
		else {
			// TODO
		}
	}
}

template<typename El, bool o>
BitSet<typename PolygonBitFlags<El, o>::FT>& PolygonBitFlags<El, o>::flags()
{
	return Base::additionalData(this);
}

template<typename El, bool o>
const BitSet<typename PolygonBitFlags<El, o>::FT>& PolygonBitFlags<El, o>::flags() const
{
	return Base::additionalData(this);
}

template<typename El, bool o>
Vector<BitSet<typename PolygonBitFlags<El, o>::FT>, -1>& PolygonBitFlags<El, o>::edgeFlags()
{
	return Base::container(this);
}

template<typename El, bool o>
const Vector<BitSet<typename PolygonBitFlags<El, o>::FT>, -1>&
PolygonBitFlags<El, o>::edgeFlags() const
{
	return Base::container(this);
}

} // namespace vcl::comp
