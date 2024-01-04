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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCLIB_SPACE_BIT_SET_BIT_PROXY_H
#define VCLIB_SPACE_BIT_SET_BIT_PROXY_H

#include <concepts>
#include <functional>

#include <vclib/types.h>

namespace vcl {

// See: https://stackoverflow.com/a/10145050/5851101

/**
 * @brief The BitProxy class allows to access to a bool reference from a bit
 * saved in a mask, and then allow assignment.
 *
 * @ingroup space
 */
template<std::integral T>
class BitProxy
{
    std::reference_wrapper<T> mask;
    const uint                index;

public:
    /**
     * @brief Constructs the BitProxy with the given mask and index.
     * @param[in] mask: the mask containing the bit to access.
     * @param[in] index: the index of the bit to access.
     */
    BitProxy(T& mask, uint index) : mask(mask), index(index) {}

    operator bool() const { return mask.get() & (1 << index); }

    void operator=(bool bit)
    {
        mask.get() = (mask.get() & ~(bit << index)) | (bit << index);
    }

    BitProxy& operator|=(bool bit)
    {
        mask.get() |= (bit << index);
        return *this;
    }

    BitProxy& operator&=(bool bit)
    {
        mask.get() &= ~(bit << index);
        return *this;
    }

    BitProxy& operator/=(bool bit)
    {
        mask.get() ^= (bit << index);
        return *this;
    }
};

} // namespace vcl

#endif // VCLIB_SPACE_BIT_SET_BIT_PROXY_H
