/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include "array.h"
#include "color.h"

#include <vclib/base.h>

namespace vcl {

/**
 * @brief A class for representing and manipulating 2D images.
 *
 * The Image class stores pixel data in a 2D array of 32-bit unsigned integers.
 * Each pixel is internally stored in ABGR (Alpha, Blue, Green, Red) format,
 * with 8 bits per channel.
 *
 * The class provides constructors to load images from raw data buffers with
 * various pixel formats, automatically converting them to the internal ABGR
 * representation. It also includes functionalities for color space management,
 * transformations like mirroring, and serialization.
 *
 * @ingroup space_core
 */
class Image
{
public:
    enum class ColorSpace { UNKNOWN, LINEAR, SRGB };

private:
    Array2<uint> mImg;

    ColorSpace mColorSpace = ColorSpace::UNKNOWN;

public:
    /**
     * @brief Default constructor. Creates an empty, null image.
     */
    Image() {}

    /**
     * @brief Constructs an Image from a raw pixel buffer.
     *
     * This constructor initializes the image with data from a provided buffer.
     * It can handle various input pixel formats and optionally flip the image
     * vertically.
     *
     * @param[in] data: A pointer to the raw pixel data.
     * @param[in] w: The width of the image in pixels.
     * @param[in] h: The height of the image in pixels.
     * @param[in] yFlip: If true, the image is flipped vertically after loading.
     * This is often needed for compatibility with graphics APIs like OpenGL
     * where the origin is at the bottom-left.
     * @param[in] format: The pixel format of the input `data` buffer. The data
     * will be converted from this format to the internal ABGR format.
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

    /**
     * @brief Constructs an Image by copying an existing Array2 of pixels.
     * @param[in] img: The Array2 containing pixel data in ABGR format.
     */
    Image(const Array2<uint>& img) : mImg(img) {}

    /**
     * @brief Constructs an Image by moving an existing Array2 of pixels.
     * @param[in] img: The Array2 to move, containing pixel data in ABGR format.
     */
    Image(Array2<uint>&& img) : mImg(std::move(img)) {}

    /**
     * @brief Checks if the image is null or empty.
     * @return True if the image has no pixels (width or height is zero), false
     * otherwise.
     */
    bool isNull() const { return mImg.empty(); }

    /**
     * @brief Gets the height of the image in pixels.
     * @return The image height.
     */
    int height() const { return mImg.rows(); }

    /**
     * @brief Gets the width of the image in pixels.
     * @return The image width.
     */
    int width() const { return mImg.cols(); }

    /**
     * @brief Calculates the total size of the image data in bytes.
     * @return The size in bytes (width * height * 4).
     */
    std::size_t sizeInBytes() const { return mImg.rows() * mImg.cols() * 4; }

    /**
     * @brief Gets the color space of the image.
     * @return The color space of the image.
     */
    ColorSpace colorSpace() const { return mColorSpace; }

    /**
     * @brief Gets a reference to the image's color space property.
     * @return A writable reference to the color space, allowing modification.
     */
    ColorSpace& colorSpace() { return mColorSpace; }

    /**
     * @brief Retrieves the color of a specific pixel.
     * @param[in] i: The row index (Y-coordinate) of the pixel.
     * @param[in] j: The column index (X-coordinate) of the pixel.
     * @return A vcl::Color object representing the pixel at the specified
     * coordinates.
     */
    Color pixel(uint i, uint j) const
    {
        return Color(static_cast<Color::ColorABGR>(mImg(i, j)));
    }

    /**
     * @brief Provides direct, read-only access to the raw pixel data.
     * @return A const pointer to the beginning of the pixel buffer.
     * @note The data is a contiguous block of memory where each pixel is a
     * 32-bit integer in ABGR format.
     */
    const unsigned char* data() const
    {
        return reinterpret_cast<const unsigned char*>(mImg.data());
    }

    /**
     * @brief Flips the image horizontally and/or vertically in-place.
     * @param[in] horizontal: If true, flips the image along the vertical axis
     * (mirror).
     * @param[in] vertical: If true, flips the image along the horizontal axis
     * (upside-down). Defaults to true.
     */
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

    /**
     * @brief Serializes the image data to an output stream.
     * @param[in] os: The output stream to write to.
     */
    void serialize(std::ostream& os) const
    {
        mImg.serialize(os);
        vcl::serialize(os, mColorSpace);
    }

    /**
     * @brief Deserializes image data from an input stream.
     * @param[in] is: The input stream to read from.
     */
    void deserialize(std::istream& is)
    {
        mImg.deserialize(is);
        vcl::deserialize(is, mColorSpace);
    }
};

/* Concepts */

/**
 * @brief A concept that is satisfied if a type T is or derives from vcl::Image.
 *
 * @tparam T: The type to be tested for conformity to the ImageConcept.
 * @ingroup space_core
 */
template<typename T>
concept ImageConcept = std::derived_from<std::remove_cvref_t<T>, Image>;

} // namespace vcl

#endif // VCL_SPACE_CORE_IMAGE_H
