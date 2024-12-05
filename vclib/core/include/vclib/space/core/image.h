/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/io/image.h>
#include <vclib/io/serialization.h>
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
    vcl::Array2<uint32_t> mImg;

public:
    Image() {}

    /**
     * @brief Load an image from a file.
     *
     * @param[in] filename: the name of the file.
     */
    Image(const std::string& filename) { load(filename); }

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
        const void*         data,
        uint                w,
        uint                h,
        bool                yFlip  = false,
        Color::Format::Enum format = Color::Format::ABGR)
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
                        vcl::Color c(cdata[i * w + j], format);
                        mImg(i, j) = c.abgr();
                    }
                }
            }

            if (yFlip) {
                mirror();
            }
        }
    }

    bool isNull() const { return mImg.empty(); }

    int height() const { return mImg.rows(); }

    int width() const { return mImg.cols(); }

    std::size_t sizeInBytes() const { return mImg.rows() * mImg.cols() * 4; }

    vcl::Color pixel(uint i, uint j) const
    {
        return vcl::Color(static_cast<vcl::Color::ColorABGR>(mImg(i, j)));
    }

    const unsigned char* data() const
    {
        return reinterpret_cast<const unsigned char*>(mImg.data());
    }

    bool load(const std::string& filename)
    {
        int w, h;
        // we first load the data, then we copy it into our array2d, and then we
        // free it.
        std::shared_ptr<unsigned char> tmp = loadImageData(filename, w, h);
        if (tmp) {
            std::size_t size = w * h * 4;

            mImg.resize(w, h);
            std::copy(
                tmp.get(),
                tmp.get() + size,
                reinterpret_cast<unsigned char*>(mImg.data()));
            return true;
        }
        else {
            return false;
        }
    }

    void save(const std::string& filename, uint quality = 90) const
    {
        auto* data = reinterpret_cast<const unsigned char*>(mImg.data());
        saveImageData(filename, mImg.cols(), mImg.rows(), data, quality);
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
