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

#ifndef VCL_MESH_COMPONENTS_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_WEDGE_COLORS_H

#include "detection/wedge_colors_detection.h"
#include "internal/generic_container.h"

#include <vclib/space/color.h>

namespace vcl::comp {

template<int N>
class WedgeColors : protected internal::GenericContainer<vcl::Color, N>, public WedgeColorsTriggerer
{
	using Base = internal::GenericContainer<vcl::Color, N>;

public:
	static const int WEDGE_COLOR_NUMBER = Base::CONTAINER_SIZE;

	/* Iterator Types declaration */

	using WedgeColorsIterator           = typename Base::GCIterator;
	using ConstWedgeColorsIterator      = typename Base::ConstGCIterator;
	using WedgeColorsRangeIterator      = typename Base::GCRangeIterator;
	using ConstWedgeColorsRangeIterator = typename Base::ConstGCRangeIterator;

	WedgeColors();

	/* Member functions */

	vcl::Color&       wedgeColor(uint i);
	const vcl::Color& wedgeColor(uint i) const;

	vcl::Color&       wedgeColorMod(int i);
	const vcl::Color& wedgeColorMod(int i) const;

	void setWedgeColor(const vcl::Color& t, uint i);
	void setWedgeColors(const std::vector<vcl::Color>& list);

	/* Iterator Member functions */

	WedgeColorsIterator           wedgeColorBegin();
	WedgeColorsIterator           wedgeColorEnd();
	ConstWedgeColorsIterator      wedgeColorBegin() const;
	ConstWedgeColorsIterator      wedgeColorEnd() const;
	WedgeColorsRangeIterator      wedgeColors();
	ConstWedgeColorsRangeIterator wedgeColors() const;

protected:
	/* Member functions specific for vector */

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeWedgeColors(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushWedgeColor(const vcl::Color& c);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void)
	insertWedgeColor(uint i, const vcl::Color& c);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseWedgeColor(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearWedgeColor();

	template <typename Element>
	void importFrom(const Element& e);

private:
	template<typename Element>
	void importWedgeColorsFrom(const Element& e);
};

} // namespace vcl::comp

#include "wedge_colors.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_COLORS_H
