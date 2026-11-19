// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H

#include "base/bit_flags_concepts.h"

#include <vclib/space/core.h>

#include <cassert>

namespace vcl::comp {

/**
 * @brief The TriangleBitFlags class represents a collection of 32 bits that
 * will be part of a Triangle of a Mesh.
 *
 * This is a specialization class of the BitFlags component, meaning that it can
 * be used in substitution to that component. The difference is that this class
 * is meant to be used on Triangle Faces, and adds bits with particular meanings
 * for triangles.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Triangle has been deleted - read only
 * - 1: selected: if the current Triangle has been selected
 * - 2: visited: if the current Triangle has been visited (useful for some visit
 *               algorithms)
 * - from 3 to 5: edge faux: if the current Triangle has the i-th edge (i in
 *                            [0, 2]) marked as faux
 * - from 6 to 8: edge border: if the current Triangle has the i-th edge (i in
 *                             [0, 2]) on border
 * - from 9 to 11: edge selection: if the current Triangle has the i-th edge (i
 *                                in [0, 2]) selected
 * - from 12 to 14: edge visited: if the current Triangle has the i-th edge (i
 *                               in [0, 2]) visited
 * - from 15 to 31: user bits that can have custom meanings to the user
 *
 * This class provides 17 user bits, that can be accessed using the member
 * function userBit(uint i) with position in the interval [0, 16].
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the TriangleBitFlags
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * f.edgeSelected(1) = true;
 * @endcode
 *
 * @ingroup components
 */
class TriangleBitFlags : public Component<CompId::BIT_FLAGS, BitSet<uint>>
{
    using Base = Component<CompId::BIT_FLAGS, BitSet<uint>>;

    // indices of the bits
    enum {
        DELETED  = 0, // bit 0
        SELECTED = 1, // bit 1
        VISITED  = 2, // bit 2
        // Faux edges: when representing polygonal meshes on triangle meshes,
        // some triangle edges can be marked as "faux", meaning that they are
        // internal on the polygon
        FAUX0    = 3, // bits [3, 5]
        // Edge border
        BORDER0  = 6, // bits [6, 8]
        // Edge selection
        EDGESEL0 = 9,  // bits [9, 11]
        EDGEVIS0 = 12, // bits [12, 14]
    };

    static constexpr uint FIRST_USER_BIT = 15;

public:
    /**
     * @brief Expose the underlying type of the BitFlags.
     */
    using FlagsType = uint;

    /**
     * @brief Static number of bits that can have custom meanings to the user
     */
    static constexpr uint USER_BIT_COUNT =
        sizeof(FlagsType) * 8 - FIRST_USER_BIT;

    /* Member functions */

    /**
     * @brief Returns whether the current Triangle is deleted or not.
     * @return true if the Triangle is deleted, false otherwise.
     */
    bool deleted(this auto&& self)
    {
        if (!self.isValid())
            return false;
        return self.flags()[DELETED];
    }

    /**
     * @brief Returns whether the current Triangle is selected or not.
     * @return true if the Triangle is selected, false otherwise.
     */
    decltype(auto) selected(this auto&& self) { return self.flags()[SELECTED]; }

    /**
     * @brief Returns whether the current Triangle has been visited or not.
     * @return true if the Triangle has been visited, false otherwise.
     */
    decltype(auto) visited(this auto&& self) { return self.flags()[VISITED]; }

    /**
     * @brief Returns whether the current Triangle is on border or not, by
     * checking whether at least one of its three edges are on border or not.
     * @return true if at least one edge of the Triangle is on border, false
     * otherwise.
     */
    bool onBorder(this auto&& self)
    {
        return self.edgeOnBorder(0) || self.edgeOnBorder(1) ||
               self.edgeOnBorder(2);
    }

    /**
     * @brief Returns whether the i-th edge of the current triangle is on border
     * or not.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the triangle is on border, false
     * otherwise.
     */
    decltype(auto) edgeOnBorder(this auto&& self, uint i)
    {
        assert(i < 3);
        return self.flags()[BORDER0 + i];
    }

    /**
     * @brief Returns whether the i-th edge of the current triangle is selected
     * or not.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the triangle is selected, false
     * otherwise.
     */
    decltype(auto) edgeSelected(this auto&& self, uint i)
    {
        assert(i < 3);
        return self.flags()[EDGESEL0 + i];
    }

    /**
     * @brief Returns whether the i-th edge of the current triangle is visited
     * or not.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the triangle is visited, false
     * otherwise.
     */
    decltype(auto) edgeVisited(this auto&& self, uint i)
    {
        assert(i < 3);
        return self.flags()[EDGEVIS0 + i];
    }

    /**
     * @brief Returns whether the i-th edge of the current triangle is faux or
     * not.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the triangle is faux, false otherwise.
     */
    decltype(auto) edgeFaux(this auto&& self, uint i)
    {
        assert(i < 3);
        return self.flags()[FAUX0 + i];
    }

    /**
     * @brief Returns a reference to the value of the user bit of this Triangle
     * given in input. The bit is checked to be less than the total number of
     * assigned user bits, which in this class is 17.
     *
     * @param[in] bit: the position of the bit, in the interval [0 - 16].
     * @return a reference to the desired user bit.
     */
    decltype(auto) userBit(this auto&& self, uint bit)
    {
        assert(bit < USER_BIT_COUNT);
        return self.flags()[bit + FIRST_USER_BIT];
    }

    /**
     * @brief Unsets all the flags of this Triangle and sets them to `false`,
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
        if (f & 0x00000010)
            self.visited() = true;
        if (f & 0x00000020)
            self.selected() = true;
        if (f & 0x00000040)
            self.edgeOnBorder(0) = true;
        if (f & 0x00000080)
            self.edgeOnBorder(1) = true;
        if (f & 0x00000100)
            self.edgeOnBorder(2) = true;
        if (f & 0x00008000)
            self.edgeSelected(0) = true;
        if (f & 0x00010000)
            self.edgeSelected(1) = true;
        if (f & 0x00020000)
            self.edgeSelected(2) = true;
        if (f & 0x00040000)
            self.edgeFaux(0) = true;
        if (f & 0x00080000)
            self.edgeFaux(1) = true;
        if (f & 0x00100000)
            self.edgeFaux(2) = true;
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
            f |= 0x00000010;
        if (self.selected())
            f |= 0x00000020;
        if (self.edgeOnBorder(0))
            f |= 0x00000040;
        if (self.edgeOnBorder(1))
            f |= 0x00000080;
        if (self.edgeOnBorder(2))
            f |= 0x00000100;
        if (self.edgeSelected(0))
            f |= 0x00008000;
        if (self.edgeSelected(1))
            f |= 0x00010000;
        if (self.edgeSelected(2))
            f |= 0x00020000;
        if (self.edgeFaux(0))
            f |= 0x00040000;
        if (self.edgeFaux(1))
            f |= 0x00080000;
        if (self.edgeFaux(2))
            f |= 0x00100000;
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
void TriangleBitFlags::importFrom(this auto&& self, const OtherElement& other, bool)
{
    if constexpr (HasBitFlags<OtherElement>) {
        const bool wasDeleted = self.deleted();
        const bool isDeleted  = other.deleted();
        self.resetBitFlags();
        if constexpr (HasTriangleBitFlags<OtherElement>) {
            self.setUnderlyingBitFlags(other.underlyingBitFlags());
            self.setDeleted(isDeleted);
        }
        else {
            self.setDeleted(isDeleted);
            self.selected() = other.selected();
            self.visited()  = other.visited();

            const uint um =
                std::min(USER_BIT_COUNT, (uint) OtherElement::USER_BIT_COUNT);
            for (uint i = 0; i < um; ++i) {
                self.userBit(i) = other.userBit(i);
            }
            if constexpr (HasPolygonBitFlags<OtherElement>) {
                const uint numEdges = std::min(3u, (uint) other.edgeCount());
                for (uint i = 0; i < numEdges; ++i) {
                    self.edgeOnBorder(i) = other.edgeOnBorder(i);
                    self.edgeSelected(i) = other.edgeSelected(i);
                    self.edgeVisited(i)  = other.edgeVisited(i);
                    self.edgeFaux(i)     = other.edgeFaux(i);
                }
            }
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

#endif // VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
