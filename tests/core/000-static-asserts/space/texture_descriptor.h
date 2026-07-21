// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
