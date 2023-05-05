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

template<typename El, bool o>
TriangleBitFlags<El, o>::TriangleBitFlags()
{
	if constexpr (!IS_VERTICAL) {
		init();
	}
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::init()
{
	flags().reset();
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::isEnabled() const
{
	return data.template isComponentEnabled<El>(this);
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::deleted() const
{
	return flags()[DELETED];
}

template<typename El, bool o>
BitProxy<int> TriangleBitFlags<El, o>::selected()
{
	return flags()[SELECTED];
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::selected() const
{
	return flags()[SELECTED];
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::onBorder() const
{
	return edgeOnBorder(0) || edgeOnBorder(1) || edgeOnBorder(2);
}

template<typename El, bool o>
BitProxy<int> TriangleBitFlags<El, o>::edgeOnBorder(uint i)
{
	assert(i < 3);
	return flags()[BORDER0 + i];
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::edgeOnBorder(uint i) const
{
	assert(i < 3);
	return flags()[BORDER0 + i];
}

template<typename El, bool o>
BitProxy<int> TriangleBitFlags<El, o>::edgeSelected(uint i)
{
	assert(i < 3);
	return flags()[EDGESEL0 + i];
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::edgeSelected(uint i) const
{
	assert(i < 3);
	return flags()[EDGESEL0 + i];
}

template<typename El, bool o>
BitProxy<int> TriangleBitFlags<El, o>::edgeFaux(uint i)
{
	assert(i < 3);
	return flags()[FAUX0 + i];
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::edgeFaux(uint i) const
{
	assert(i < 3);
	return flags()[FAUX0 + i];
}

template<typename El, bool o>
bool TriangleBitFlags<El, o>::userBit(uint bit) const
{
	return flags()[bit + FIRST_USER_BIT];
}

template<typename El, bool o>
BitProxy<int> TriangleBitFlags<El, o>::userBit(uint bit)
{
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Unsets all the flags of this Element and sets them to `false`, **except the deleted
 * flag**, which needs to be manually reset.
 */
template<typename El, bool o>
void TriangleBitFlags<El, o>::resetBitFlags()
{
	bool isD = deleted();
	flags().reset();
	deleted() = isD;
}

template<typename El, bool o>
void TriangleBitFlags<El, o>::importFromVCGFlags(int f)
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
int TriangleBitFlags<El, o>::exportToVCGFlags() const
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

template<typename El, bool o>
BitProxy<int> TriangleBitFlags<El, o>::deleted()
{
	return flags()[DELETED];
}

template<typename El, bool o>
template<typename Element>
void TriangleBitFlags<El, o>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		resetBitFlags();
		if constexpr (HasPolygonBitFlags<Element>) {
			// todo
		}
		else if constexpr (HasTriangleBitFlags<Element>) {
			flags() = e.flags();
		}
		else {
			// todo
		}
	}
}

template<typename El, bool o>
BitSet<int>& TriangleBitFlags<El, o>::flags()
{
	return data.template get<El>(this);
}

template<typename El, bool o>
BitSet<int> TriangleBitFlags<El, o>::flags() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
