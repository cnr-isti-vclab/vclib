// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
