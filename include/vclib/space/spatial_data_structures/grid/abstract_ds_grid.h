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

#ifndef VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_ABSTRACT_DS_GRID_H
#define VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_ABSTRACT_DS_GRID_H

#include <vclib/mesh/requirements.h>

#include "functions.h"

namespace vcl {

/**
 * @brief The AbstractDSGrid class describes a generic Spatial Data Structure organized on a regular
 * grid, that allows to store elements (ValueType) in a particular way that will be managed by its
 * derived classes (e.g. StaticGrid, HashTableGrid).
 *
 * This class cannot be instantiated. You can only instantiate a derived class.
 *
 * This class provides only constructor and member functions that are common with all the derived
 * classes.
 * Note that derived class can hide some of the member exposed in this class (e.g. they could
 * disallow the possibility to remove elements from a grid).
 *
 * Each derived class must implement the following *protected/private* members:
 * - bool insertInCell(const KeyType&, const ValueType&) -> inserts the element in a Grid Cell
 * - bool eraseInCell(const KeyType&, const ValueType&) -> erases the element from a Grid Cell
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
class AbstractDSGrid : public GridType
{
public:
	using KeyType = typename GridType::CellCoord;

	bool insert(const ValueType& v);

	template<typename ObjIterator>
	uint insert(ObjIterator begin, ObjIterator end);

protected:
	AbstractDSGrid();

	AbstractDSGrid(const GridType& grid);

	template<PointConcept PointType>
	AbstractDSGrid(const PointType& min, const PointType& max, const KeyType& sizes);

	template<typename BoxType>
	AbstractDSGrid(const BoxType& bbox, const KeyType& sizes);

	// this constructor **does not insert elements**.
	template<typename ObjIterator>
	AbstractDSGrid(ObjIterator begin, ObjIterator end);
};

} // namespace vcl

#include "abstract_ds_grid.cpp"

#endif // VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_ABSTRACT_DS_GRID_H
