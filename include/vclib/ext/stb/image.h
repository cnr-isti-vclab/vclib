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

#ifndef VCLIB_EXT_STB_IMAGE_H
#define VCLIB_EXT_STB_IMAGE_H

#include <string>

#include <stb/stb_image.h>

namespace vcl::stb {

class Image
{
public:
	Image();
	Image(const Image& oth);
	Image(Image&& oth);
	Image(const std::string& filename);

	~Image();

	bool isNull() const;

	int height() const;
	int width() const;
	std::size_t sizeInBytes() const;

	const unsigned char* data() const;

	/// @private
	friend void swap(Image& i1, Image& i2);
	void swap(Image& oth);

	Image& operator=(Image oth);

private:
	unsigned char* dataImage = nullptr;
	int h = 0, w = 0;
};

inline void swap(Image& i1, Image& i2);

} // namespace vcl::stb

#include "image.cpp"

#endif // VCLIB_EXT_STB_IMAGE_H
