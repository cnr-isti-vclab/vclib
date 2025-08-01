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
    Image       mImg;
    std::string mPath;

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

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mPath);
        mImg.serialize(os);
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mPath);
        mImg.deserialize(is);
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
