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

#ifndef VCL_SPACE_CORE_BIT_SET_BIT_PROXY_H
#define VCL_SPACE_CORE_BIT_SET_BIT_PROXY_H

#include <vclib/types.h>

#include <concepts>
#include <functional>

namespace vcl {

// See: https://stackoverflow.com/a/10145050/5851101

/**
 * @brief The BitProxy class allows to access to a bool reference from a bit
 * saved in a mask, and then allow assignment.
 *
 * @ingroup space_core
 */
template<std::integral T>
class BitProxy
{
    std::reference_wrapper<T> mMask;
    const uint                mIndex;

public:
    /**
     * @brief The type of the underlying integral value used to store the bits.
     */
    using UnderlyingType = T;

    /**
     * @brief Constructs the BitProxy with the given mask and index.
     * @param[in] mask: the mask containing the bit to access.
     * @param[in] index: the index of the bit to access.
     */
    BitProxy(T& mask, uint index) : mMask(mask), mIndex(index) {}

    operator bool() const { return mMask.get() & (1 << mIndex); }

    void operator=(bool bit)
    {
        mMask.get() = (mMask.get() & ~(1 << mIndex)) | (bit << mIndex);
    }

    BitProxy& operator|=(bool bit)
    {
        mMask.get() |= (bit << mIndex);
        return *this;
    }

    BitProxy& operator&=(bool bit)
    {
        mMask.get() &= ~(bit << mIndex);
        return *this;
    }

    BitProxy& operator/=(bool bit)
    {
        mMask.get() ^= (bit << mIndex);
        return *this;
    }
};

/* Concepts */

/**
 * @brief A concept representing a BitProxy.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a BitProxy class having any integral type as a template parameter.
 *
 * @tparam T: The type to be tested for conformity to the BitProxyConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept BitProxyConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    BitProxy<typename RemoveRef<T>::UnderlyingType>>;

} // namespace vcl

#endif // VCL_SPACE_CORE_BIT_SET_BIT_PROXY_H
