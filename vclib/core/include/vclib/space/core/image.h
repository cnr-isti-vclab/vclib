/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SPACE_CORE_IMAGE_H
#define VCL_SPACE_CORE_IMAGE_H

#include <vclib/serialization.h>
#include <vclib/space/core/array.h>
#include <vclib/space/core/color.h>

#include <string>

namespace vcl {

/**
 * @brief The Image class stores an Image in 4 bytes RGBA format.
 *
 * Each pixel is stored as 4 bytes, with the first byte representing RGBA in
 * a `uint32_t`.
 *
 * @ingroup space_core
 */
class Image
{
    Array2<uint> mImg;

public:
    Image() {}

    /**
     * @brief Construct an Image from a raw buffer, which is assumed to be in
     * the given format (default: ABGR).
     *
     * @param[in] data: the raw buffer.
     * @param[in] w: the width of the image.
     * @param[in] h: the height of the image.
     * @param[in] yFlip: if true, the image is flipped along the y axis.
     * @param[in] format: the format of the buffer, that is the way each pixel
     * (4 bytes) is stored.
     */
    Image(
        const void*   data,
        uint          w,
        uint          h,
        bool          yFlip  = false,
        Color::Format format = Color::Format::ABGR)
    {
        if (data) {
            mImg.resize(h, w);
            std::size_t size = w * h;

            auto* cdata = reinterpret_cast<const uint32_t*>(data);

            if (format == Color::Format::ABGR) {
                std::copy(cdata, cdata + size, mImg.data());
            }
            else {
                for (uint i = 0; i < h; i++) {
                    for (uint j = 0; j < w; j++) {
                        Color c(cdata[i * w + j], format);
                        mImg(i, j) = c.abgr();
                    }
                }
            }

            if (yFlip) {
                mirror();
            }
        }
    }

    Image(const Array2<uint>& img) : mImg(img) {}

    Image(Array2<uint>&& img) : mImg(std::move(img)) {}

    bool isNull() const { return mImg.empty(); }

    int height() const { return mImg.rows(); }

    int width() const { return mImg.cols(); }

    std::size_t sizeInBytes() const { return mImg.rows() * mImg.cols() * 4; }

    Color pixel(uint i, uint j) const
    {
        return Color(static_cast<Color::ColorABGR>(mImg(i, j)));
    }

    const unsigned char* data() const
    {
        return reinterpret_cast<const unsigned char*>(mImg.data());
    }

    void mirror(bool horizontal = false, bool vertical = true)
    {
        if (horizontal) {
            for (uint i = 0; i < mImg.rows(); i++) {
                std::reverse(mImg.data(i), mImg.data(i) + mImg.cols());
            }
        }
        if (vertical) {
            for (uint i = 0; i < mImg.rows() / 2; i++) {
                uint mir = mImg.rows() - i - 1;
                std::swap_ranges(
                    mImg.data(i), mImg.data(i) + mImg.cols(), mImg.data(mir));
            }
        }
    }

    void serialize(std::ostream& os) const { mImg.serialize(os); }

    void deserialize(std::istream& is) { mImg.deserialize(is); }
};

} // namespace vcl

#endif // VCL_SPACE_CORE_IMAGE_H
