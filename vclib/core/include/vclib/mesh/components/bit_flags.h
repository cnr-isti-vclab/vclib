// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_BIT_FLAGS_H

#include "base/bit_flags_concepts.h"

#include <vclib/space/core.h>

#include <cassert>

namespace vcl::comp {

/**
 * @brief The BitFlags component class represents a collection of 32 bits that
 * will be part of an Element (e.g. Vertex, Face, ...).
 *
 * This Component (or a specialization, that is any class that satisfies the
 * HasBitFlags concept) is mandatory into every Element of the mesh.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Element has been deleted - read only
 * - 1: selected: if the current Element has been selected
 * - 2: border: if the current Element is on border
 * - 3: visited: if the current Element has been visited (useful for some visit
 *               algorithms)
 * - from 4 to 31: user bits that can have custom meanings to the user
 *
 * This class provides 28 user bits, that can be accessed using the member
 * function userBit(uint i) with position in the interval [0, 27].
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Vertex Element `v` with the BitFlags component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * bool isD = v.deleted();
 * @endcode
 *
 * @ingroup components
 */
class BitFlags : public Component<CompId::BIT_FLAGS, BitSet<uint>>
{
    using Base = Component<CompId::BIT_FLAGS, BitSet<uint>>;

    // indices of the bits
    enum {
        DELETED  = 0, // bit 0
        SELECTED = 1, // bit 1
        BORDER   = 2, // bit 2
        VISITED  = 3  // bit 3
    };

    static constexpr uint FIRST_USER_BIT = 4;

public:
    /**
     * @brief Expose the underlying type of the BitFlags.
     */
    using FlagsType = uint;

    /**
     * @brief Static number of bits that can have custom meanings to the user
     */
    static constexpr uint USER_BIT_COUNT = sizeof(uint) * 8 - FIRST_USER_BIT;

    /* Member functions */

    /**
     * @brief Returns whether the current Element is deleted or not.
     * @return true if the Element is deleted, false otherwise.
     */
    bool deleted(this auto&& self)
    {
        if (!self.isValid())
            return false;
        return self.flags()[DELETED];
    }

    /**
     * @brief Returns a reference of the bit that tells whether the current
     * Element is selected or not.
     * @return true if the Element is selected, false otherwise.
     */
    decltype(auto) selected(this auto&& self) { return self.flags()[SELECTED]; }

    /**
     * @brief Returns a reference of the bit that tells whether the current
     * Element is on border or not.
     * @return true if the Element is on border, false otherwise.
     */
    decltype(auto) onBorder(this auto&& self) { return self.flags()[BORDER]; }

    /**
     * @brief Returns a reference of the bit that tells whether the current
     * Element has been visited or not.
     * @return true if the Element has been visited, false otherwise.
     */
    decltype(auto) visited(this auto&& self) { return self.flags()[VISITED]; }

    /**
     * @brief Returns a reference to the user bit of this Element given in
     * input. The bit is checked to be less than the total number of assigned
     * user bits, which in this class is 28.
     *
     * @param[in] bit: the position of the bit, in the interval [0 - 27].
     * @return true if the required bit is enabled, `false` otherwise.
     */
    decltype(auto) userBit(this auto&& self, uint bit)
    {
        assert(bit < USER_BIT_COUNT);
        return self.flags()[bit + FIRST_USER_BIT];
    }

    /**
     * @brief Unsets all the flags of this Element and sets them to `false`,
     * **except the deleted flag**, which needs to be manually reset.
     */
    void resetBitFlags(this auto&& self)
    {
        bool isD = self.deleted();
        self.flags().reset();
        self.flags()[DELETED] = isD;
    }

    /**
     * @brief Returns the underlying integral value of the BitFlags.
     * @return the underlying integral value of the BitFlags.
     */
    FlagsType underlyingBitFlags(this auto&& self)
    {
        return self.flags().underlying();
    }

    /**
     * @brief Sets the underlying integral value of the BitFlags.
     *
     * @note The deleted flag will be preserved and won't be overridden by the
     * input integral value.
     *
     * @param[in] bits: the integral value to set as the underlying value of the
     * BitFlags
     */
    void setUnderlyingBitFlags(this auto&& self, FlagsType bits)
    {
        bool isD = self.deleted();
        self.flags().setUnderlying(bits);
        self.flags()[DELETED] = isD;
    }

    /**
     * @brief Sets all the flags of this element to the values contained in the
     * integer input parameter, that represents the bit flags of the VCG
     * library.
     * @param[in] f: bit flags in the format of the VCG library.
     */
    void importFlagsFromVCGFormat(this auto&& self, int f)
    {
        self.resetBitFlags();
        if (f & 0x0010)
            self.visited() = true;
        if (f & 0x0020)
            self.selected() = true;
        if (f & 0x0100)
            self.onBorder() = true;
    }

    /**
     * @brief Returns the bit flags of this element in the format of the VCG
     * library.
     *
     * @return an integer representing the bit flags of this element in the
     * format of the VCG library.
     */
    int exportFlagsToVCGFormat(this auto&& self)
    {
        int f = 0;
        if (self.visited())
            f |= 0x0010;
        if (self.selected())
            f |= 0x0020;
        if (self.onBorder())
            f |= 0x0100;
        return f;
    }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);

    void setDeleted(this auto&& self, bool b = true)
    {
        self.flags()[DELETED] = b;
    }

private:
    decltype(auto) flags(this auto&& self) { return Base::compData(self); }
};

/* Implementation of member functions */

template<typename OtherElement>
void BitFlags::importFrom(this auto&& self, const OtherElement& other, bool)
{
    if constexpr (HasBitFlags<OtherElement>) {
        const bool wasDeleted = self.deleted();
        const bool isDeleted  = other.deleted();
        self.resetBitFlags();
        if constexpr (HasFaceBitFlags<OtherElement>) {
            self.setDeleted(isDeleted);
            self.selected() = other.selected();
            self.visited()  = other.visited();
            self.onBorder() = other.onBorder();

            const uint um =
                std::min(USER_BIT_COUNT, (uint) OtherElement::USER_BIT_COUNT);
            for (uint i = 0; i < um; ++i) {
                self.userBit(i) = other.userBit(i);
            }
        }
        else {
            self.setUnderlyingBitFlags(other.underlyingBitFlags());
            self.setDeleted(isDeleted);
        }
        if constexpr (CanAdjustContainerElemCount<decltype(self)>) {
            if (self.isValid()) {
                self.parentContainer().adjustElemCountOnDeletedChange(
                    wasDeleted, isDeleted);
            }
        }
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BIT_FLAGS_H
