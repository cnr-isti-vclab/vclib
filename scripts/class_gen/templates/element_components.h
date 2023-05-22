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

#ifndef VCL_MESH_ELEMENTS_%EL_U%_COMPONENTS_H
#define VCL_MESH_ELEMENTS_%EL_U%_COMPONENTS_H

%INCLUDE_COMPONENTS%

/**
 * @defgroup %EL%_components %EL_UC% Components
 * @ingroup components
 *
 * @brief This group lists all the Components that may be used by the %EL_UC% element.
 *
 * All these components are listed inside the vcl::%EL_NS% namespace.
 */

namespace vcl::%EL_NS% {

/**
 * @addtogroup %EL%_components
 * @{
 */

%ELEMENT_COMPONENTS%
/** @} */ // end of %EL%_components group

} // namespace vcl::%EL_NS%

#endif // VCL_MESH_ELEMENTS_%EL_U%_COMPONENTS_H
