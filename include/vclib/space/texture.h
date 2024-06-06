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

#ifndef VCL_SPACE_TEXTURE_H
#define VCL_SPACE_TEXTURE_H

#include "image.h"

#include <vclib/io/file_info.h>

namespace vcl {

class Texture
{
    Image mImg;
    std::string mName;

public:
    Texture() {}

    /**
     * @brief Load a texture from a file.
     *
     * @param[in] filename: the name of the file.
     */
    Texture(const std::string& filename)
    {
        mName = FileInfo::fileNameWithExtension(filename);
        mImg.load(filename);
    }

    /**
     * @brief Get the name of the texture.
     *
     * @return the name of the texture.
     */
    const std::string& name() const { return mName; }

    /**
     * @brief Get the name of the texture.
     *
     * @return the name of the texture.
     */
    std::string& name() { return mName; }

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
};

} // namespace vcl

#endif // VCL_SPACE_TEXTURE_H
