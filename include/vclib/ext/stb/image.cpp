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

namespace vcl::stb {

inline Image::Image()
{
}

inline Image::Image(const Image &oth)
{
	dataImage = new unsigned char[oth.sizeInBytes()];
	std::copy(oth.dataImage, oth.dataImage + oth.sizeInBytes(), dataImage);
}

Image::Image(Image &&oth)
{
	swap(oth);
}

inline Image::Image(const std::string &filename)
{
	unsigned char* tmp = stbi_load(filename.c_str(), &w, &h, nullptr, 4);
	std::size_t size = w * h * 4;
	dataImage = new unsigned char[size];
	std::copy(tmp, tmp + size, dataImage);
	stbi_image_free(tmp);
}

inline Image::~Image()
{
	delete[] dataImage;
}

inline bool Image::isNull() const
{
	return dataImage == nullptr;
}

inline int Image::height() const
{
	return h;
}

inline int Image::width() const
{
	return w;
}

inline std::size_t Image::sizeInBytes() const
{
	return h * w * 4;
}

inline const unsigned char* Image::data() const
{
	return dataImage;
}

inline void Image::swap(Image &oth)
{
	stb::swap(*this, oth);
}

inline Image &Image::operator=(Image oth)
{
	swap(oth);
	return *this;
}

inline void swap(Image &i1, Image &i2)
{
	using std::swap;
	swap(i1.dataImage, i2.dataImage);
	swap(i1.h, i2.h);
	swap(i1.w, i2.w);
}

} // namespace vcl::stb
