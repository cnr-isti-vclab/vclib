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

#ifndef VCL_SPACE_SPATIAL_DATA_STRUCTURES_STATIC_GRID_H
#define VCL_SPACE_SPATIAL_DATA_STRUCTURES_STATIC_GRID_H

#include <vector>

#include "grid.h"

#include <vclib/algorithms/stat/bounding_box.h>
#include <vclib/misc/comparators.h>

namespace vcl {

template<typename GridType, typename ValueType>
class StaticGrid : public GridType
{
public:
	StaticGrid();

	template<typename ObjIterator>
	StaticGrid(ObjIterator begin, ObjIterator end);


private:
	std::vector<std::pair<uint, ValueType>> values;

	std::vector<uint> grid;

	template<typename ObjIterator>
	void insertElements(ObjIterator begin, ObjIterator end);

	void insertNode(typename GridType::CellCoord& cell, const ValueType& v);
};

} // namespace vcl

#include "static_grid.cpp"

#endif // VCL_SPACE_SPATIAL_DATA_STRUCTURES_STATIC_GRID_H
