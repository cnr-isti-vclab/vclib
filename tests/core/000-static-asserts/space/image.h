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

#ifndef IMAGE_H
#define IMAGE_H

#include <vclib/space.h>

void imageStaticAsserts()
{
    using namespace vcl;

    // image
    static_assert(
        ImageConcept<Image>, "Image does not satisfy the ImageConcept");
    static_assert(
        ImageConcept<Image&>, "Image& does not satisfy the ImageConcept");
    static_assert(
        ImageConcept<const Image>,
        "const Image does not satisfy the ImageConcept");
    static_assert(
        ImageConcept<const Image&>,
        "const Image& does not satisfy the ImageConcept");
    static_assert(
        ImageConcept<Image&&>, "Image&& does not satisfy the ImageConcept");

    static_assert(Serializable<Image>, "Image is not serializable");
}

#endif // IMAGE_H
