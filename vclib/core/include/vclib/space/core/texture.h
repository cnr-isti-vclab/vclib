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

#ifndef VCL_SPACE_CORE_TEXTURE_H
#define VCL_SPACE_CORE_TEXTURE_H

#include "image.h"

#include <vclib/base.h>

namespace vcl {

class Texture
{
public:

    enum class ColorSpace {
        UNKNOWN,
        LINEAR,
        SRGB
    };

    /*Enums defined following gltf 2.0 specification*/

    enum class MinificationFilter {
        NONE = -1,
        NEAREST = 9728,
        LINEAR,
        NEAREST_MIPMAP_NEAREST = 9984,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR
    };

    enum class MagnificationFilter {
        NONE = -1,
        NEAREST = 9728,
        LINEAR
    };

    enum class WrapMode {
        REPEAT = 10497,
        CLAMP_TO_EDGE = 33071,
        MIRRORED_REPEAT = 33648
    };


private:

    Image       mImg;
    std::string mPath;
    ColorSpace mColorSpace = ColorSpace::UNKNOWN;
    MinificationFilter mMinFilter = MinificationFilter::NONE;
    MagnificationFilter mMagFilter = MagnificationFilter::NONE;
    WrapMode mWrapU = WrapMode::REPEAT;
    WrapMode mWrapV = WrapMode::REPEAT;

public:
    Texture() {}

    /**
     * @brief Load a texture from a file.
     *
     * @param[in] path: the path of the texture file.
     */
    Texture(const std::string& path) : mPath(path) {}

    /**
     * @brief Creates a Texture object, with the given image and its path.
     *
     * @param[in] img: the image.
     * @param[in] path: the path of the texture file.
     */
    Texture(const Image& img, const std::string& path) : mImg(img), mPath(path)
    {
    }

    /**
     * @brief Creates a Texture object, with the given image and its path.
     *
     * @param[in] img: the image.
     * @param[in] path: the path of the texture file.
     */
    Texture(Image&& img, const std::string& path) :
            mImg(std::move(img)), mPath(path)
    {
    }

    /**
     * @brief Checks whether the texture is null (no image and no path).
     *
     * @return true if the texture is null, false otherwise.
     */
    bool isNull() const { return mImg.isNull() && mPath.empty(); }

    /**
     * @brief Get the path of the texture.
     *
     * @return the path of the texture.
     */
    const std::string& path() const { return mPath; }

    /**
     * @brief Get the path of the texture.
     *
     * @return the path of the texture.
     */
    std::string& path() { return mPath; }

    /**
     * @brief Get the image of the texture.
     *
     * @return the image of the texture.
     */
    const Image& image() const { return mImg; }

    /**
     * @brief Get the image of the texture.
     *
     * @return the image of the texture.
     */
    Image& image() { return mImg; }

    /**
     * @brief Get the minification filter of the texture.
     *
     * @return the minification filter of the texture.
     */
    MinificationFilter minFilter() const { return mMinFilter; }

    /**
     * @brief Get the minification filter of the texture.
     *
     * @return the minification filter of the texture.
     */
    MinificationFilter& minFilter() { return mMinFilter; }

    /**
     * @brief Get the magnification filter of the texture.
     *
     * @return the magnification filter of the texture.
     */
    MagnificationFilter magFilter() const { return mMagFilter; }

    /**
     * @brief Get the magnification filter of the texture.
     *
     * @return the magnification filter of the texture.
     */
    MagnificationFilter& magFilter() { return mMagFilter; }

    /**
     * @brief Get the U wrap mode of the texture.
     *
     * @return the U wrap mode of the texture.
     */
    WrapMode wrapU() const { return mWrapU; }

    /**
     * @brief Get the U wrap mode of the texture.
     *
     * @return the U wrap mode of the texture.
     */
    WrapMode& wrapU() { return mWrapU; }

    /**
     * @brief Get the V wrap mode of the texture.
     *
     * @return the V wrap mode of the texture.
     */
    WrapMode wrapV() const { return mWrapV; }

    /**
     * @brief Get the V wrap mode of the texture.
     *
     * @return the V wrap mode of the texture.
     */
    WrapMode& wrapV() { return mWrapV; }

    /**
     * @brief Get the color space of the texture.
     *
     * @return the color space of the texture.
     */
    ColorSpace colorSpace() const { return mColorSpace; }

    /**
     * @brief Get the color space of the texture.
     *
     * @return the color space of the texture.
     */
    ColorSpace& colorSpace() { return mColorSpace; }


    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mPath);
        mImg.serialize(os);
        vcl::serialize(os, mColorSpace);
        vcl::serialize(os, mMinFilter, mMagFilter, mWrapU, mWrapV);

    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mPath);
        mImg.deserialize(is);
        vcl::deserialize(is, mColorSpace);
        vcl::deserialize(is, mMinFilter, mMagFilter, mWrapU, mWrapV);
    }
};

/* Concepts */

/**
 * @brief A concept representing a Texture.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Texture class.
 *
 * @tparam T: The type to be tested for conformity to the TextureConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept TextureConcept = std::derived_from<std::remove_cvref_t<T>, Texture>;

} // namespace vcl

#endif // VCL_SPACE_CORE_TEXTURE_H
