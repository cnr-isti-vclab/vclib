/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H

#include <vclib/concepts/mesh/components/bit_flags.h>
#include <vclib/space/bit_set.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The TriangleBitFlags class represents a collection of 16 bits that
 * will be part of a Triangle of a Mesh.
 *
 * This is a specialization class of the BitFlags component, meaning that it can
 * be used in sostitution to that component. The difference is that this class
 * is meant to be used on Triangle Faces, and adds bits with particular meanings
 * for triangles.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Triangle has been deleted - read only
 * - 1: selected: if the current Triangle has been selected
 * - 2: visited: if the current Triangle has been visited (useful for some visit
 *               algorithms)
 * - from 3 to 5: edge border: if the current Triangle has the i-th edge (i in
 *                             [0, 2]) on border
 * - from 6 to 8: edge selection: if the current Triangle has the i-th edge (i
 *                                in [0, 2]) selected
 * - from 9 to 11: edge visited: if the current Triangle has the i-th edge (i
 *                               in [0, 2]) visited
 * - from 12 to 14: edge faux: if the current Triangle has the i-th edge (i in
 *                            [0, 2]) marked as faux
 * - 15: user bit that can have custom meanings to the user
 *
 * This class provides 1 user bit, that can be accessed using the member
 * function userBit(uint i) with position 0.
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the TriangleBitFlags
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * v.edgeSelected(1) = true;
 * @endcode
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
class TriangleBitFlags :
        public Component<
            TriangleBitFlags<ElementType, OPT>,
            BIT_FLAGS,
            BitSet<short>,
            ElementType,
            OPT>
{
    using Base = Component<
        TriangleBitFlags<ElementType, OPT>,
        BIT_FLAGS,
        BitSet<short>,
        ElementType,
        OPT>;

    using FT = short; // FlagsType, the integral type used for the flags

    static const uint FIRST_USER_BIT = 15;

    // indices of the bits
    enum {
        DELETED  = 0, // bit 0
        SELECTED = 1, // bit 1
        VISITED  = 2, // bit 2
        // Edge border
        BORDER0 = 3, // bits [3, 5]
        // Edge selection
        EDGESEL0 = 6, // bits [6, 8]
        EDGEVIS0 = 9, // bits [9, 11]
        // Faux edges: when representing polygonal meshes on triangle meshes,
        // some triangle edges can be marked as "faux", meaning that they are
        // internal on the polygon
        FAUX0 = 12 // bits [12, 14]
    };

public:
    /**
     * @brief Static number of bits that can have custom meanings to the user
     */
    inline static const uint USER_BITS_NUMBER = sizeof(FT) * 8 - FIRST_USER_BIT;

    /* Constructors */

    /**
     * @brief Initializes the bits to `false`.
     */
    TriangleBitFlags()
    {
        if constexpr (!Base::IS_VERTICAL) {
            init();
        }
    }

    /**
     * @private
     * @brief Initializes the bits to false.
     *
     * It is made in the init function since the component could be not
     * available during construction (e.g. if the component is optional and not
     * enabled).
     *
     * This member function is hidden by the element that inherits this class.
     */
    void init() { flags().reset(); }

    /* Member functions */

    /**
     * @brief Returns whether the current Triangle is deleted or not.
     * @return true if the Triangle is deleted, false otherwise.
     */
    bool deleted() const { return flags()[DELETED]; }

    /**
     * @brief Accesses the 'selected' bit of this Triangle, returning a
     * reference to it.
     * @return a reference to the 'selected' bit of this Triangle.
     */
    BitProxy<FT> selected() { return flags()[SELECTED]; }

    /**
     * @brief Returns whether the current Triangle is selected or not.
     * @return true if the Triangle is selected, false otherwise.
     */
    bool selected() const { return flags()[SELECTED]; }

    /**
     * @brief Accesses the 'visited' bit of this Triangle, returning a reference
     * to it.
     * @return a reference to the 'visited' bit of this Triangle.
     */
    BitProxy<FT> visited() { return flags()[VISITED]; }

    /**
     * @brief Returns whether the current Triangle has been visited or not.
     * @return true if the Triangle has been visited, false otherwise.
     */
    bool visited() const { return flags()[VISITED]; }

    /**
     * @brief Returns whether the current Triangle is on border or not, by
     * checking whether at least one of its three edges are on border or not.
     * @return true if at least one edge of the Triangle is on border, false
     * otherwise.
     */
    bool onBorder() const
    {
        return edgeOnBorder(0) || edgeOnBorder(1) || edgeOnBorder(2);
    }

    /**
     * @brief Accesses the 'onBorder' bit of the i-th edge of the triangle,
     * returning a reference to it.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return a reference to the 'onBorder' bit of the i-th edge of the
     * triangle.
     */
    BitProxy<FT> edgeOnBorder(uint i)
    {
        assert(i < 3);
        return flags()[BORDER0 + i];
    }

    /**
     * @brief Returns whether the i-th edge of the current triangle is on border
     * or not.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the triangle is on border, false
     * otherwise.
     */
    bool edgeOnBorder(uint i) const
    {
        assert(i < 3);
        return flags()[BORDER0 + i];
    }

    /**
     * @brief Accesses the 'selected' bit of the i-th edge of the triangle,
     * returning a reference to it.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return a reference to the 'selected' bit of the i-th edge of the
     * triangle.
     */
    BitProxy<FT> edgeSelected(uint i)
    {
        assert(i < 3);
        return flags()[EDGESEL0 + i];
    }

    /**
     * @brief Returns whether the i-th edge of the current triangle is selected
     * or not.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the triangle is selected, false
     * otherwise.
     */
    bool edgeSelected(uint i) const
    {
        assert(i < 3);
        return flags()[EDGESEL0 + i];
    }

    /**
     * @brief Accesses the 'visited' bit of the i-th edge of the triangle,
     * returning a reference to it.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return a reference to the 'visited' bit of the i-th edge of the
     * triangle.
     */
    BitProxy<FT> edgeVisited(uint i)
    {
        assert(i < 3);
        return flags()[EDGEVIS0 + i];
    }

    /**
     * @brief Returns whether the i-th edge of the current triangle is visited
     * or not.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the triangle is visited, false
     * otherwise.
     */
    bool edgeVisited(uint i) const
    {
        assert(i < 3);
        return flags()[EDGEVIS0 + i];
    }

    /**
     * @brief Accesses the 'faux' bit of the i-th edge of the triangle,
     * returning a reference to it.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return a reference to the 'faux' bit of the i-th edge of the triangle.
     */
    BitProxy<FT> edgeFaux(uint i)
    {
        assert(i < 3);
        return flags()[FAUX0 + i];
    }

    /**
     * @brief Returns whether the i-th edge of the current triangle is faux or
     * not.
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the triangle is faux, false otherwise.
     */
    bool edgeFaux(uint i) const
    {
        assert(i < 3);
        return flags()[FAUX0 + i];
    }

    /**
     * @brief Returns a reference to the value of the user bit of this Triangle
     * given in input. The bit is checked to be less than the total number of
     * assigned user bits, which in this class is 4.
     *
     * @param[in] bit: the position of the bit, in the interval [0 - 3].
     * @return a reference to the desired user bit.
     */
    bool userBit(uint bit) const
    {
        assert(bit < USER_BITS_NUMBER);
        return flags()[bit + FIRST_USER_BIT];
    }

    /**
     * @brief Returns the boolean value of the user bit of this Triangle given
     * in input. The bit is checked to be less than the total number of assigned
     * user bits, which in this class is 4.
     *
     * @param[in] bit: the position of the bit, in the interval [0 - 3], that
     * will be returned by reference.
     * @return `true` if the required bit is enabled, `false` otherwise.
     */
    BitProxy<FT> userBit(uint bit)
    {
        assert(bit < USER_BITS_NUMBER);
        return flags()[bit + FIRST_USER_BIT];
    }

    /**
     * @brief Unsets all the flags of this Triangle and sets them to `false`,
     * **except the deleted flag**, which needs to be manually reset.
     */
    void resetBitFlags()
    {
        bool isD = deleted();
        flags().reset();
        deletedBit() = isD;
    }

    /**
     * @brief Sets all the flags of this element to the values contained in the
     * integer input parameter, that represents the bit flags of the VCG
     * library.
     * @param[in] f: bit flags in the format of the VCG library.
     */
    void importFlagsFromVCGFormat(int f)
    {
        if (f & 0x00000010)
            visited() = true;
        if (f & 0x00000020)
            selected() = true;
        if (f & 0x00000040)
            edgeOnBorder(0) = true;
        if (f & 0x00000080)
            edgeOnBorder(1) = true;
        if (f & 0x00000100)
            edgeOnBorder(2) = true;
        if (f & 0x00008000)
            edgeSelected(0) = true;
        if (f & 0x00010000)
            edgeSelected(1) = true;
        if (f & 0x00020000)
            edgeSelected(2) = true;
        if (f & 0x00040000)
            edgeFaux(0) = true;
        if (f & 0x00080000)
            edgeFaux(1) = true;
        if (f & 0x00100000)
            edgeFaux(2) = true;
    }

    /**
     * @brief Returns the bit flags of this element in the format of the VCG
     * library.
     *
     * @return an integer representing the bit flags of this element in the
     * format of the VCG library.
     */
    int exportFlagsToVCGFormat() const
    {
        int f = 0;
        if (visited())
            f &= 0x00000010;
        if (selected())
            f &= 0x00000020;
        if (edgeOnBorder(0))
            f &= 0x00000040;
        if (edgeOnBorder(1))
            f &= 0x00000080;
        if (edgeOnBorder(2))
            f &= 0x00000100;
        if (edgeSelected(0))
            f &= 0x00008000;
        if (edgeSelected(1))
            f &= 0x00010000;
        if (edgeSelected(2))
            f &= 0x00020000;
        if (edgeFaux(0))
            f &= 0x00040000;
        if (edgeFaux(1))
            f &= 0x00080000;
        if (edgeFaux(2))
            f &= 0x00100000;
        return f;
    }

    // dummy member to discriminate between triangle and non-triangle bit flags
    void __triangleBitFlags() const {}

protected:
    BitProxy<FT> deletedBit() { return flags()[DELETED]; }

    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e)
    {
        if constexpr (HasBitFlags<Element>) {
            resetBitFlags();
            if constexpr (HasTriangleBitFlags<Element>) {
                flags() = e.flags();
            }
            else {
                // BitFlags
                deletedBit()  = e.deleted();
                selected()    = e.selected();
                visited()     = e.visited();
                const uint UM = std::min(USER_BITS_NUMBER, e.USER_BITS_NUMBER);
                for (uint i = 0; i < UM; ++i)
                    userBit(i) = e.userBit(i);
                if constexpr (HasPolygonBitFlags<Element>) {
                    // PolygonBitFlags
                    for (uint i = 0; i < 3; ++i) {
                        edgeOnBorder(i) = e.edgeOnBorder(i);
                        edgeSelected(i) = e.edgeSelected(i);
                        edgeFaux(i)     = e.edgeFaux(i);
                    }
                }
            }
        }
    }

private:
    // members that allow to access the flags, trough data (horizontal) or
    // trough parent (vertical)
    BitSet<FT>& flags() { return Base::data(); }

    BitSet<FT> flags() const { return Base::data(); }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
