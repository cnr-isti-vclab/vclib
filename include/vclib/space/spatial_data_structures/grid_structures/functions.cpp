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

#include "functions.h"

namespace vcl::grid {

template<typename GridStructure, typename ValueType>
bool insert(GridStructure& g, const ValueType& v)
{
	// ValueType could be anything. We need to understand first if it is a pointer or not,
	// and then insert the value only if it is not nullptr

	// VT is:
	// - ValueType if ValueType is not a pointer
	// - The Type pointed by ValueType, if ValueType is a pointer
	using TMPVT = typename std::remove_pointer<ValueType>::type;
	using VT = typename std::remove_reference<TMPVT>::type;
	const VT* vv = nullptr; // vv is a pointer to VT
	if constexpr(std::is_pointer<ValueType>::value) {
		vv = v;
	}
	else {
		vv = &v;
	}

	if (vv) { // if vv is a valid pointer (ValueType, or ValueType* if ValueType is not a pointer)
		if constexpr (PointConcept<VT>) { // if the ValueType was a Point (or Point*)
			typename GridStructure::CellCoord cell = g.cell(*vv);
			return g.insert(cell, v);
		}
		else if constexpr (VertexConcept<VT>) { // if the ValueType was a Vertex (or Vertex*)
			typename GridStructure::CellCoord cell = g.cell(vv->coord());
			return g.insert(cell, v);
		}
		else { // else, call the boundingBox function
			typename GridStructure::BBoxType bb = vcl::boundingBox(*vv);

			typename GridStructure::CellCoord bmin = g.cell(bb.min);
			typename GridStructure::CellCoord bmax = g.cell(bb.max);

			bool ins = false;
			for (const auto& cell : g.cells(bmin, bmax)) {
				ins |= g.insert(cell, v);
			}
			return ins;
		}
	}
	return false;
}


} // namespace vcl::grid
