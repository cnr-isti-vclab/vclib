/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_SPACE_IMAGE_H
#define VCL_SPACE_IMAGE_H

#include <string>

#include <vclib/io/image.h>
#include <vclib/space/array.h>
#include <vclib/space/color.h>

namespace vcl {

/**
 * @brief The Image class stores an Image in 8 bit RGBA format.
 */
class Image
{
    vcl::Array2<uint32_t> img;

public:
    Image() {}

    Image(const std::string& filename) { load(filename); }

    bool isNull() const { return img.empty(); }

    int height() const { return img.rows(); }

    int width() const { return img.cols(); }

    std::size_t sizeInBytes() const { return img.rows() * img.cols() * 4; }

    vcl::Color pixel(uint i, uint j) const
    {
        return vcl::Color(static_cast<vcl::Color::ColorABGR>(img(i, j)));
    }

    const unsigned char* data() const
    {
        return reinterpret_cast<const unsigned char*>(img.data());
    }

    bool load(const std::string& filename)
    {
        int w, h;
        // we first load the data, then we copy it into our array2d, and then we
        // free it.
        std::shared_ptr<unsigned char> tmp = loadImageData(filename, w, h);
        if (tmp) {
            std::size_t size = w * h * 4;

            img.resize(w, h);
            std::copy(
                tmp.get(),
                tmp.get() + size,
                reinterpret_cast<unsigned char*>(img.data()));
            return true;
        }
        else {
            return false;
        }
    }

    void mirror(bool horizontal = false, bool vertical = true)
    {
        if (horizontal) {
            for (uint i = 0; i < img.rows(); i++) {
                std::reverse(img.data(i), img.data(i) + img.cols());
            }
        }
        if (vertical) {
            for (uint i = 0; i < img.rows() / 2; i++) {
                uint mir = img.rows() - i - 1;
                std::swap_ranges(
                    img.data(i), img.data(i) + img.cols(), img.data(mir));
            }
        }
    }
};

} // namespace vcl

#endif // VCL_SPACE_IMAGE_H
