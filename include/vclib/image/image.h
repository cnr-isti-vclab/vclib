/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_IMAGE_IMAGE_H
#define VCL_IMAGE_IMAGE_H

#include <string>

#if __has_include(<stb/stb_image.h>)
#include <stb/stb_image.h>
#else
// inclusion for usage of vclib without CMake - not ideal but necessary for header-only
#define STB_IMAGE_STATIC // make stb usable in header-only libraries
#include "../../../external/stb-master/stb/stb_image.h"
#endif

#include <vclib/space/array.h>
#include <vclib/space/color.h>

namespace vcl {

class Image
{
public:
	Image();
	Image(const std::string& filename);

	bool isNull() const;

	int height() const;
	int width() const;
	std::size_t sizeInBytes() const;

	vcl::Color pixel(uint i, uint j) const;

	const unsigned char* data() const;

	bool load(const std::string& filename);

	void mirror(bool horizontal = false, bool vertical = true);

private:
	unsigned char* dataImage = nullptr;
	int h = 0, w = 0;

	vcl::Array2<uint32_t> img;
};

} // namespace vcl

#include "image.cpp"

#endif // VCL_IMAGE_IMAGE_H
