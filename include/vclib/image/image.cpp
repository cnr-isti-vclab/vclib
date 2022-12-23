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

#include "image.h"

namespace vcl {

inline Image::Image()
{
}

inline Image::Image(const std::string &filename)
{
	load(filename);
}

inline bool Image::isNull() const
{
	return img.empty();
}

inline int Image::height() const
{
	return img.rows();
}

inline int Image::width() const
{
	return img.cols();
}

inline std::size_t Image::sizeInBytes() const
{
	return img.rows() * img.cols() * 4;
}

inline vcl::Color Image::pixel(uint i, uint j) const
{
	return vcl::Color((vcl::Color::ColorRGBA)img(i,j));
}

inline const unsigned char* Image::data() const
{
	return (unsigned char* ) img.data();
}

inline bool Image::load(const std::string &filename)
{
	int w, h;
	// we first load the data, then we copy it into our array2d, and then we free it.
	unsigned char* tmp = stbi_load(filename.c_str(), &w, &h, nullptr, 4); // force 4 channels
	if (tmp) {
		std::size_t size = w * h * 4;

		img.resize(w, h);
		std::copy(tmp, tmp + size, (unsigned char*)img.data());
		stbi_image_free(tmp);
		return true;
	}
	else {
		return false;
	}
}

void Image::mirror(bool horizontal, bool vertical)
{
	if (horizontal) {
		for (uint i = 0; i < img.rows(); i++) {
			std::reverse(img.cArray(i), img.cArray(i) + img.cols());
		}
	}
	if (vertical) {
		for (uint i = 0; i < img.rows() / 2; i++) {
			uint mir = img.rows() - i - 1;
			std::swap_ranges(img.cArray(i), img.cArray(i) + img.cols(), img.cArray(mir));
		}
	}
}

} // namespace vcl
