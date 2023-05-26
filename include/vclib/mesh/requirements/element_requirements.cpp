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

#include "element_requirements.h"

namespace vcl {

/**
 * @brief Returns `true` if the given mesh has its Container of the given Element compact.
 *
 * @tparam ELEMENT_TYPE: the ElementEnumType of the ElementContainer to check.
 * @tparam MeshType: the type of the Mesh to check.
 *
 * @param m: the mesh on which check the compactness of the Container of the given Element.
 * @return `true` if the Container of the given Element is compact.
 */
template<uint ELEMENT_TYPE, typename MeshType>
bool isElementContainerCompact(const MeshType& m)
{
	return (m.template elementNumber<ELEMENT_TYPE>() == m.template elementContainerSize<ELEMENT_TYPE>());
}

} // namespace vcl
