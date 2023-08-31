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
 * @brief Constructor that initializes the bits to false.
 */
template<typename El, bool O>
TriangleBitFlags<El, O>::TriangleBitFlags()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

/**
 * @private
 * @brief Initializes the bits to false.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename El, bool O>
void TriangleBitFlags<El, O>::init()
{
	flags().reset();
}

/**
 * @brief Returns whether the current Triangle is deleted or not.
 * @return true if the Triangle is deleted, false otherwise.
 */
template<typename El, bool O>
bool TriangleBitFlags<El, O>::deleted() const
{
	return flags()[DELETED];
}

/**
 * @brief Accesses the 'selected' bit of this Triangle, returning a reference to
 * it.
 * @return a reference to the 'selected' bit of this Triangle.
 */
template<typename El, bool O>
BitProxy<typename TriangleBitFlags<El, O>::FT>
TriangleBitFlags<El, O>::selected()
{
	return flags()[SELECTED];
}

/**
 * @brief Returns whether the current Triangle is selected or not.
 * @return true if the Triangle is selected, false otherwise.
 */
template<typename El, bool O>
bool TriangleBitFlags<El, O>::selected() const
{
	return flags()[SELECTED];
}

/**
 * @brief Accesses the 'visited' bit of this Triangle, returning a reference to
 * it.
 * @return a reference to the 'visited' bit of this Triangle.
 */
template<typename El, bool O>
BitProxy<typename TriangleBitFlags<El, O>::FT>
TriangleBitFlags<El, O>::visited()
{
	return flags()[VISITED];
}

/**
 * @brief Returns whether the current Triangle has been visited or not.
 * @return true if the Triangle has been visited, false otherwise.
 */
template<typename El, bool O>
bool TriangleBitFlags<El, O>::visited() const
{
	return flags()[VISITED];
}

/**
 * @brief Returns whether the current Triangle is on border or not, by checking
 * whether at least one of its three edges are on border or not.
 * @return true if at least one edge of the Triangle is on border, false
 * otherwise.
 */
template<typename El, bool O>
bool TriangleBitFlags<El, O>::onBorder() const
{
	return edgeOnBorder(0) || edgeOnBorder(1) || edgeOnBorder(2);
}

/**
 * @brief Accesses the 'onBorder' bit of the i-th edge of the triangle,
 * returning a reference to it.
 * @param[in] i: the index of the edge, it must be less than 3.
 * @return a reference to the 'onBorder' bit of the i-th edge of the triangle.
 */
template<typename El, bool O>
BitProxy<typename TriangleBitFlags<El, O>::FT>
TriangleBitFlags<El, O>::edgeOnBorder(uint i)
{
	assert(i < 3);
	return flags()[BORDER0 + i];
}

/**
 * @brief Returns whether the i-th edge of the current triangle is on border or
 * not.
 * @param[in] i: the index of the edge, it must be less than 3.
 * @return true if the i-th edge of the triangle is on border, false otherwise.
 */
template<typename El, bool O>
bool TriangleBitFlags<El, O>::edgeOnBorder(uint i) const
{
	assert(i < 3);
	return flags()[BORDER0 + i];
}

/**
 * @brief Accesses the 'selected' bit of the i-th edge of the triangle,
 * returning a reference to it.
 * @param[in] i: the index of the edge, it must be less than 3.
 * @return a reference to the 'selected' bit of the i-th edge of the triangle.
 */
template<typename El, bool O>
BitProxy<typename TriangleBitFlags<El, O>::FT>
TriangleBitFlags<El, O>::edgeSelected(uint i)
{
	assert(i < 3);
	return flags()[EDGESEL0 + i];
}

/**
 * @brief Returns whether the i-th edge of the current triangle is selected or
 * not.
 * @param[in] i: the index of the edge, it must be less than 3.
 * @return true if the i-th edge of the triangle is selected, false otherwise.
 */
template<typename El, bool O>
bool TriangleBitFlags<El, O>::edgeSelected(uint i) const
{
	assert(i < 3);
	return flags()[EDGESEL0 + i];
}

/**
 * @brief Accesses the 'faux' bit of the i-th edge of the triangle, returning
 * a reference to it.
 * @param[in] i: the index of the edge, it must be less than 3.
 * @return a reference to the 'faux' bit of the i-th edge of the triangle.
 */
template<typename El, bool O>
BitProxy<typename TriangleBitFlags<El, O>::FT>
TriangleBitFlags<El, O>::edgeFaux(uint i)
{
	assert(i < 3);
	return flags()[FAUX0 + i];
}

/**
 * @brief Returns whether the i-th edge of the current triangle is faux or
 * not.
 * @param[in] i: the index of the edge, it must be less than 3.
 * @return true if the i-th edge of the triangle is faux, false otherwise.
 */
template<typename El, bool O>
bool TriangleBitFlags<El, O>::edgeFaux(uint i) const
{
	assert(i < 3);
	return flags()[FAUX0 + i];
}

/**
 * @brief Returns a reference to the value of the user bit of this Triangle
 * given in input. The bit is checked to be less than the total number of
 * assigned user bits, which in this class is 4.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 3].
 * @return a reference to the desired user bit.
 */
template<typename El, bool O>
bool TriangleBitFlags<El, O>::userBit(uint bit) const
{
	assert(bit < USER_BITS_NUMBER);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Returns the boolean value of the user bit of this Triangle given in
 * input. The bit is checked to be less than the total number of assigned user
 * bits, which in this class is 4.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 3], that will
 * be returned by reference.
 * @return `true` if the required bit is enabled, `false` otherwise.
 */
template<typename El, bool O>
BitProxy<typename TriangleBitFlags<El, O>::FT>
TriangleBitFlags<El, O>::userBit(uint bit)
{
	assert(bit < USER_BITS_NUMBER);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Unsets all the flags of this Triangle and sets them to `false`,
 * **except the deleted flag**, which needs to be manually reset.
 */
template<typename El, bool O>
void TriangleBitFlags<El, O>::resetBitFlags()
{
	bool isD = deleted();
	flags().reset();
	deletedBit() = isD;
}

/**
 * @brief Sets all the flags of this element to the values contained in the
 * integer input parameter, that represents the bit flags of the VCG library.
 * @param[in] f: bit flags in the format of the VCG library.
 */
template<typename El, bool O>
void TriangleBitFlags<El, O>::importFromVCGFlags(int f)
{
	if (f & 0x00000010)
		visited() = true;
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

/**
 * @brief Returns the bit flags of this element in the format of the VCG
 * library.
 *
 * @return an integer representing the bit flags of this element in the format
 * of the VCG library.
 */
template<typename El, bool O>
int TriangleBitFlags<El, O>::exportToVCGFlags() const
{
	int f = 0;
	if (visited())
		f &= 0x00000010;
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

template<typename El, bool O>
BitProxy<typename TriangleBitFlags<El, O>::FT>
TriangleBitFlags<El, O>::deletedBit()
{
	return flags()[DELETED];
}

template<typename El, bool O>
template<typename Element>
void TriangleBitFlags<El, O>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		resetBitFlags();
		if constexpr (HasTriangleBitFlags<Element>) {
			flags() = e.flags();
		}
		else {
			// BitFlags
			deletedBit() = e.deleted();
			selected() = e.selected();
			visited() = e.visited();
			const uint UM = std::min(USER_BITS_NUMBER, e.USER_BITS_NUMBER);
			for (uint i = 0; i < UM; ++i)
				userBit(i) = e.userBit(i);
			if constexpr (HasPolygonBitFlags<Element>) {
				// PolygonBitFlags
				for (uint i = 0; i < 3; ++i) {
					edgeOnBorder(i) = e.edgeOnBorder(i);
					edgeSelected(i) = e.edgeSelected(i);
					edgeFaux(i)     = e.edgeFaux(i);
				}
			}

		}
	}
}

template<typename El, bool O>
BitSet<typename TriangleBitFlags<El, O>::FT>& TriangleBitFlags<El, O>::flags()
{
	return Base::data();
}

template<typename El, bool O>
BitSet<typename TriangleBitFlags<El, O>::FT>
TriangleBitFlags<El, O>::flags() const
{
	return Base::data();
}

} // namespace vcl::comp
