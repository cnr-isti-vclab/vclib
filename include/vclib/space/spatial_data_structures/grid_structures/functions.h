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

#ifndef VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_STRUCTURES_FUNCTIONS_H
#define VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_STRUCTURES_FUNCTIONS_H

#include <vclib/algorithms/stat/bounding_box.h>
#include <vclib/misc/types.h>
#include <vclib/mesh/requirements.h>

namespace vcl::grid {

// The functions in this file must be declared as friend of the Grid Structure class that
// wants to use them, since they use private members of the Grid Structure.
//
// private member functions are, given a GridStructure g:
// - g.insert(const KeyType&, const ValueType&)
// - g.erase(const KeyType&, const ValueType&)
// - g.isMarked(const vcl::Markable<ValueType>&)
// - g.mark(const vcl::Markable<ValueType>&)
// - g.unMarkAll()

template<typename GridStructure, typename ValueType>
bool insert(GridStructure& g, const ValueType& v);

} // namespace vcl::grid

#include "functions.cpp"

#endif // VCL_SPACE_SPATIAL_DATA_STRUCTURES_GRID_STRUCTURES_FUNCTIONS_H
