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

#ifndef TEXTURE_DESCRIPTOR_H
#define TEXTURE_DESCRIPTOR_H

#include <vclib/space.h>

void textureDescriptorStaticAsserts()
{
    using namespace vcl;

    // texture descriptor
    static_assert(
        TextureDescriptorConcept<TextureDescriptor>,
        "TextureDescriptor does not satisfy the TextureDescriptorConcept");
    static_assert(
        TextureDescriptorConcept<const TextureDescriptor>,
        "const TextureDescriptor does not satisfy the "
        "TextureDescriptorConcept");
    static_assert(
        TextureDescriptorConcept<TextureDescriptor&>,
        "TextureDescriptor& does not satisfy the TextureDescriptorConcept");
    static_assert(
        TextureDescriptorConcept<const TextureDescriptor&>,
        "const TextureDescriptor& does not satisfy the "
        "TextureDescriptorConcept");
    static_assert(
        TextureDescriptorConcept<TextureDescriptor&&>,
        "TextureDescriptor&& does not satisfy the TextureDescriptorConcept");

    static_assert(
        Serializable<TextureDescriptor>,
        "TextureDescriptor is not serializable");
}

#endif // TEXTURE_DESCRIPTOR_H
