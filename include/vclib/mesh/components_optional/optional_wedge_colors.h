/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_COLORS_H

#include <array>
#include <assert.h>
#include <vector>

#include <vclib/misc/vcl_types.h>

#include "optional_info.h"

#include <vclib/space/color.h>

#include "../iterators/range_iterator.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalWedgeColorsVector;
}

namespace vcl::comp {

class OptionalWedgeColorsTriggerer
{
};

template<int N, typename T>
class OptionalWedgeColors
{
	template<typename, typename>
	friend class OptionalWedgeColorsVector;

private:
	using B = OptionalInfo<T>;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// the AdjFacesContainer type will be array or vector, depending on N value
	using WedgeColorsContainer = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::Color, ARRAY_SIZE>,
		typename std::vector<vcl::Color>>::type;

	static const int WEDGE_COLOR_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using WedgeColorsIterator = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::Color, ARRAY_SIZE>::iterator,
		typename std::vector<vcl::Color>::iterator>::type;

	using ConstWedgeColorsIterator = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::Color, ARRAY_SIZE>::const_iterator,
		typename std::vector<vcl::Color>::const_iterator>::type;

	using WedgeColorsRangeIterator = RangeIterator<OptionalWedgeColors, WedgeColorsIterator>;
	using ConstWedgeColorsRangeIterator =
		ConstRangeIterator<OptionalWedgeColors, ConstWedgeColorsIterator>;

	/** Constructor **/

	OptionalWedgeColors();

	/** Member functions **/

	vcl::Color&       wedgeColor(uint i);
	const vcl::Color& wedgeColor(uint i) const;

	vcl::Color&       wedgeColorMod(int i);
	const vcl::Color& wedgeColorMod(int i) const;

	void setWedgeColor(const vcl::Color& t, uint i);
	void setWedgeColors(const std::vector<vcl::Color>& list);

	/** Iterator Member functions **/

	WedgeColorsIterator           wedgeColorBegin();
	WedgeColorsIterator           wedgeColorEnd();
	ConstWedgeColorsIterator      wedgeColorBegin() const;
	ConstWedgeColorsIterator      wedgeColorEnd() const;
	WedgeColorsRangeIterator      wedgeColorIterator();
	ConstWedgeColorsRangeIterator wedgeColorIterator() const;

protected:
	/** Member functions specific for vector **/

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeWedgeColors(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushWedgeColor(const vcl::Color& t);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) insertWedgeColor(uint i, const vcl::Color& t);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseWedgeColor(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearWedgeColor();

	/** Utility member functions **/

	bool wedgeColorsEnabled() const;

private:
	uint thisId() const { return B::index((T*) this); }

	uint wedgeColorsNumber() const;
};

/**
 * Detector to check if a class has (inherits) OptionalWedgeColors
 */

template<typename T>
using hasOptionalWedgeColorsT = std::is_base_of<OptionalWedgeColorsTriggerer, T>;

template<typename T>
bool constexpr hasOptionalWedgeColors()
{
	return hasOptionalWedgeColorsT<T>::value;
}

template<typename T>
bool constexpr sanityCheckOptionalWedgeColors()
{
	if constexpr (hasOptionalWedgeColors<T>()) {
		return T::VERTEX_NUMBER == T::WEDGE_COLOR_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#include "optional_wedge_colors.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_COLORS_H
