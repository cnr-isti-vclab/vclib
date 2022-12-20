/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCLIB_EXT_IMAGE_H
#define VCLIB_EXT_IMAGE_H

// prefer stb
#ifdef VCLIB_WITH_STB
#include "stb/image.h"

namespace vcl {

using Image = stb::Image;

} // namespace vcl
#else
#ifdef VCLIB_WITH_QT
#include "qt/image.h"

namespace vcl {

using Image = qt::Image;

} // namespace vcl
#else
static_assert("To use vcl::Image class, STB or Qt must be found.");
#endif // VCLIB_WITH_QT
#endif // VCLIB_WITH_STB

#endif // VCLIB_EXT_IMAGE_H
