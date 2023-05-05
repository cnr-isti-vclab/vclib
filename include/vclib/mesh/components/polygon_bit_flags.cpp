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
	if constexpr (!IS_VERTICAL) {
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
	return data.template isComponentEnabled<El>(this);
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::deleted() const
{
	return flags()[DELETED];
}

template<typename El, bool o>
BitProxy<int> PolygonBitFlags<El, o>::selected()
{
	return flags()[SELECTED];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::selected() const
{
	return flags()[SELECTED];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::onBorder() const
{
	for (uint i = 0; i < 12; ++i)
		if (edgeOnBorder(i)) return true;
	return false;
}

template<typename El, bool o>
BitProxy<int> PolygonBitFlags<El, o>::edgeOnBorder(uint i)
{
	assert(i < 12);
	return flags()[BORDER0 + i];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::edgeOnBorder(uint i) const
{
	assert(i < 12);
	return flags()[BORDER0 + i];
}

template<typename El, bool o>
BitProxy<int> PolygonBitFlags<El, o>::edgeSelected(uint i)
{
	assert(i < 12);
	return flags()[EDGESEL0 + i];
}

template<typename El, bool o>
bool PolygonBitFlags<El, o>::edgeSelected(uint i) const
{
	assert(i < 12);
	return flags()[EDGESEL0 + i];
}

template<typename El, bool o>
BitProxy<int> PolygonBitFlags<El, o>::edgeFaux(uint i)
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

template<typename El, bool o>
bool PolygonBitFlags<El, o>::userBit(uint bit) const
{
	return flags()[bit + FIRST_USER_BIT];
}

template<typename El, bool o>
BitProxy<int> PolygonBitFlags<El, o>::userBit(uint bit)
{
	return flags()[bit + FIRST_USER_BIT];
}

template<typename El, bool o>
void PolygonBitFlags<El, o>::resetBitFlags()
{
	bool isD = deleted();
	flags().reset();
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

template<typename El, bool o>
BitProxy<int> PolygonBitFlags<El, o>::deleted()
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
		}
		else if constexpr (HasTriangleBitFlags<Element>) {
			// todo
		}
		else {
			// todo
		}
	}
}

template<typename El, bool o>
BitSet<int>& PolygonBitFlags<El, o>::flags()
{
	return data.template get<El>(this);
}

template<typename El, bool o>
BitSet<int> PolygonBitFlags<El, o>::flags() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
