// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_SERIALIZATION_ENDIAN_H
#define VCL_BASE_SERIALIZATION_ENDIAN_H

#include <cstdio>

namespace vcl::detail {

// https://stackoverflow.com/a/38141476/5851101
template<typename T>
T swapEndian(T u)
{
    union
    {
        T             u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (std::size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

} // namespace vcl::detail

#endif // VCL_BASE_SERIALIZATION_ENDIAN_H
