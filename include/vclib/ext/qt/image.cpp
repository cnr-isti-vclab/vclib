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

namespace vcl::qt {

inline Image::Image()
{
}

inline Image::Image(const std::string &filename)
{
	img = QImage(QString::fromStdString(filename)).convertToFormat(QImage::Format_RGBA8888);
}

inline bool Image::isNull() const
{
	return img.isNull();
}

inline int Image::height() const
{
	return img.height();
}

inline int Image::width() const
{
	return img.width();
}

inline std::size_t Image::sizeInBytes() const
{
	return img.sizeInBytes();
}

inline const unsigned char* Image::data() const
{
	return img.constBits();
}

} // namespace vcl::qt
