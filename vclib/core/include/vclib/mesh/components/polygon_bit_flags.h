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

#ifndef VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H

#include "bases/container_component.h"

#include <vclib/concepts.h>
#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The PolygonBitFlags class represents a collection of 8 bits plus 8
 * bits for each edge that will be part of a generic Polygonal Face of a Mesh.
 *
 * This is a specialization class of the BitFlags component, meaning that it can
 * be used in sostitution to that component. The difference is that this class
 * is meant to be used on Polygonal Faces, and adds bits with particular
 * meanings for polygons.
 *
 * This class also provides 3 flags for faux edges. These flags are added just
 * to make portable all the algorithms that use faux flags also for
 * PolygonMeshes. However, these flags should be used only when the mesh is a
 * Triangle mesh, that is when each face has vertexNumber() == 3.
 *
 * The bits have the follwing meaning:
 * - 0: deleted: if the current Polygon has been deleted - read only
 * - 1: selected: if the current Polygon has been selected
 * - 2: visited: if the current Polygon has been visited (useful for some visit
 *               algorithms)
 * - from 3 to 5: edge faux: if the current Face has is i-th edge (i in [0, 2])
 *                marked as faux
 * - from 6 to 7: user bits that can have custom meanings to the user
 *
 * This class provides 2 user bits, that can be accessed using the member
 * function userBit(uint i) with position in the interval [0, 1].
 *
 * Additionally, this class provides the following bits for each edge of the
 * Polygonal Face:
 * - 0: edge border: if the current Polygonal face has the i-th edge on border
 * - 1: edge selection: if  the current Polygonal face has the i-th edge
 *                      selected
 * - 2: edge visited: if the i-th edge of the current Polygonal face has been
 *                    visited
 * - from 3 to 7: user bits of the i-th edge that can have custo meanings to the
 *                user
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the PolygonBitFlags
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * v.isAnyEdgeOnBorder();
 * @endcode
 *
 * @note This component is *Tied To Vertex Number*: it means that the size of
 * the container, if dynamic, will change automatically along the Vertex Number
 * of the Component. For further details check the documentation of the @ref
 * ContainerComponent class.
 *
 * @tparam N: The size of the container, that will represent the number of
 * storable bit flags for each edge of the Polygon. If negative, the container
 * is dynamic and tied to the vertex number.
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<int N, typename ParentElemType = void, bool OPT = false>
class PolygonBitFlags :
        public ContainerComponent<
            PolygonBitFlags<N, ParentElemType, OPT>,
            CompId::BIT_FLAGS,
            BitSet<char>,
            N,
            BitSet<char>,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT,
            true>
{
    using FT = char; // FlagsType, the integral type used for the flags

    using Base = ContainerComponent<
        PolygonBitFlags<N, ParentElemType, OPT>,
        CompId::BIT_FLAGS,
        BitSet<FT>,
        N,
        BitSet<FT>,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT,
        true>;

    static const uint FIRST_USER_BIT      = 6;
    static const uint FIRST_EDGE_USER_BIT = 3;

    // indices of the bits
    enum {
        DELETED  = 0, // bit 0
        SELECTED = 1, // bit 1
        VISITED  = 2, // bit 2
        // Faux edges, for portability with TriangleBits
        FAUX0 = 3 // bits [3, 5]
    };

    // indices of the bits used for each edge of the polygon
    enum { EDGEBORD = 0, EDGESEL = 1, EDGEVIS = 2 };

public:
    /**
     * @brief Static number of bits that can have custom meanings to the user
     */
    inline static const uint USER_BITS_NUMBER = sizeof(FT) * 8 - FIRST_USER_BIT;

    /**
     * @brief Static number of bits for each edge that can have custom meanings
     * to the user
     */
    static const uint EDGE_USER_BITS_NUMBER =
        sizeof(FT) * 8 - FIRST_EDGE_USER_BIT;

    /* Constructors */

    /**
     * @brief Initializes all the bits to `false`.
     *
     * If the Edge flags container size is static, initializes all the
     * Edge flags to `false`, otherwise the container will be empty.
     */
    PolygonBitFlags()
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
     * @brief Returns whether the current Polygon is deleted or not.
     * @return true if the Polygon is deleted, false otherwise.
     */
    bool deleted() const { return flags()[DELETED]; }

    /**
     * @brief Accesses the 'selected' bit of this Polygon, returning a reference
     * to it.
     * @return a reference to the 'selected' bit of this Polygon.
     */
    BitProxy<FT> selected() { return flags()[SELECTED]; }

    /**
     * @brief Returns whether the current Polygon is selected or not.
     * @return true if the Polygon is selected, false otherwise.
     */
    bool selected() const { return flags()[SELECTED]; }

    /**
     * @brief Accesses the 'visited' bit of this Polygon, returning a reference
     * to it.
     * @return a reference to the 'visited' bit of this Polygon.
     */
    BitProxy<FT> visited() { return flags()[VISITED]; }

    /**
     * @brief Returns whether the current Polygon has been visited or not.
     * @return true if the Polygon has been visited, false otherwise.
     */
    bool visited() const { return flags()[VISITED]; }

    /**
     * @brief Returns whether the current Polygon is on border or not, by
     * checking whether at least one of its edges are on border or not.
     * @return true if at least one edge of the Polygon is on border, false
     * otherwise.
     */
    bool onBorder() const
    {
        for (uint i = 0; i < 12; ++i)
            if (edgeOnBorder(i))
                return true;
        return false;
    }

    /**
     * @brief Accesses the 'onBorder' bit of the i-th edge of the polygon,
     * returning a reference to it.
     * @param[in] i: the index of the edge.
     * @return a reference to the 'onBorder' bit of the i-th edge of the
     * polygon.
     */
    BitProxy<FT> edgeOnBorder(uint i)
    {
        assert(i < edgeFlags().size());
        return edgeFlags()[i][EDGEBORD];
    }

    /**
     * @brief Returns whether the i-th edge of the current Polygon is on border
     * or not.
     * @param[in] i: the index of the edge.
     * @return true if the i-th edge of the Polygon is on border, false
     * otherwise.
     */
    bool edgeOnBorder(uint i) const
    {
        assert(i < edgeFlags().size());
        return edgeFlags()[i][EDGEBORD];
    }

    /**
     * @brief Accesses the 'selected' bit of the i-th edge of the polygon,
     * returning a reference to it.
     * @param[in] i: the index of the edge.
     * @return a reference to the 'selected' bit of the i-th edge of the
     * polygon.
     */
    BitProxy<FT> edgeSelected(uint i)
    {
        assert(i < edgeFlags().size());
        return edgeFlags()[i][EDGESEL];
    }

    /**
     * @brief Returns whether the i-th edge of the current Polygon is selected
     * or not.
     * @param[in] i: the index of the edge.
     * @return true if the i-th edge of the Polygon is selected, false
     * otherwise.
     */
    bool edgeSelected(uint i) const
    {
        assert(i < edgeFlags().size());
        return edgeFlags()[i][EDGESEL];
    }

    /**
     * @brief Accesses the 'visited' bit of the i-th edge of the polygon,
     * returning a reference to it.
     * @param[in] i: the index of the edge.
     * @return a reference to the 'visited' bit of the i-th edge of the polygon.
     */
    BitProxy<FT> edgeVisited(uint i)
    {
        assert(i < edgeFlags().size());
        return edgeFlags()[i][EDGEVIS];
    }

    /**
     * @brief Returns whether the i-th edge of the current Polygon has been
     * visited or not.
     * @param[in] i: the index of the edge.
     * @return true if the i-th edge of the Polygon has been visited, false
     * otherwise.
     */
    bool edgeVisited(uint i) const
    {
        assert(i < edgeFlags().size());
        return edgeFlags()[i][EDGEVIS];
    }

    /**
     * @brief Accesses the 'faux' bit of the i-th edge of the polygon, returning
     * a reference to it.
     *
     * @note The 'faux' bit is used to just for portability with triangle meshes
     * that are stored in polygonal meshes. However, these flags should be used
     * only when the mesh is a Triangle mesh, that is when each face has
     * vertexNumber()== 3.
     *
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return a reference to the 'faux' bit of the i-th edge of the polygon.
     */
    BitProxy<FT> edgeFaux(uint i)
    {
        assert(i < 3);
        return flags()[FAUX0 + i];
    }

    /**
     * @brief Returns whether the i-th edge of the current Polygon is faux or
     * not.
     *
     * @note The 'faux' bit is used to just for portability with triangle meshes
     * that are stored in polygonal meshes. However, these flags should be used
     * only when the mesh is a Triangle mesh, that is when each face has
     * vertexNumber()== 3.
     *
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return true if the i-th edge of the Polygon is faux, false otherwise.
     */
    bool edgeFaux(uint i) const
    {
        assert(i < 3);
        return flags()[FAUX0 + i];
    }

    /**
     * @brief Returns the boolean value of the user bit of this Polygon given in
     * input. The bit is checked to be less than the total number of assigned
     * user bits, which in this class is 2.
     *
     * @param[in] bit: the position of the bit, in the interval [0 - 1], that
     * will be returned by reference.
     * @return `true` if the required bit is enabled, `false` otherwise.
     */
    bool userBit(uint bit) const
    {
        assert(bit < USER_BITS_NUMBER);
        return flags()[bit + FIRST_USER_BIT];
    }

    /**
     * @brief Returns a reference to the value of the user bit of this Polygon
     * given in input. The bit is checked to be less than the total number of
     * assigned user bits, which in this class is 2.
     *
     * @param[in] bit: the position of the bit, in the interval [0 - 1].
     * @return a reference to the desired user bit.
     */
    BitProxy<FT> userBit(uint bit)
    {
        assert(bit < USER_BITS_NUMBER);
        return flags()[bit + FIRST_USER_BIT];
    }

    /**
     * @brief Returns the boolean value of the user bit of the i-th edge of this
     * Polygon given in input. The bit is checked to be less than the total
     * number of assigned edge user bits, which in this class is 5.
     *
     * @param[in] i: the index of the edge.
     * @param[in] bit: the position of the bit, in the interval [0 - 4].
     * @return `true` if the required bit is enabled, `false` otherwise.
     */
    bool edgeUserBit(uint i, uint bit) const
    {
        assert(bit < EDGE_USER_BITS_NUMBER);
        return edgeFlags()[i][bit + FIRST_EDGE_USER_BIT];
    }

    /**
     * @brief Returns a reference to the value of the user bit of the i-th edge
     * of this Polygon given in input. The bit is checked to be less than the
     * total number of assigned edge user bits, which in this class is 5.
     *
     * @param[in] i: the index of the edge.
     * @param[in] bit: the position of the bit, in the interval [0 - 4].
     * @return a reference to the desired user bit.
     */
    BitProxy<FT> edgeUserBit(uint i, uint bit)
    {
        assert(bit < EDGE_USER_BITS_NUMBER);
        return edgeFlags()[i][bit + FIRST_EDGE_USER_BIT];
    }

    /**
     * @brief Unsets all the flags of this Polygon and sets them to `false`,
     * **except the deleted flag**, which needs to be manually reset.
     */
    void resetBitFlags()
    {
        bool isD = deleted();
        flags().reset();
        for (uint i = 0; i < edgeFlags().size(); ++i)
            edgeFlags()[i].reset();
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
    void __polygonBitFlags() const {}

protected:
    BitProxy<FT> deletedBit() { return flags()[DELETED]; }

    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        if constexpr (HasBitFlags<Element>) {
            resetBitFlags();
            if constexpr (HasPolygonBitFlags<Element>) {
                flags()     = e.flags();
                edgeFlags() = e.edgeFlags();
            }
            else {
                // BitFlags
                deletedBit()  = e.deleted();
                selected()    = e.selected();
                visited()     = e.visited();
                const uint UM = std::min(USER_BITS_NUMBER, e.USER_BITS_NUMBER);
                for (uint i = 0; i < UM; ++i)
                    userBit(i) = e.userBit(i);
                if constexpr (HasTriangleBitFlags<Element>) {
                    // TriangleBitFlags
                    for (uint i = 0; i < 3; ++i) {
                        edgeOnBorder(i) = e.edgeOnBorder(i);
                        edgeSelected(i) = e.edgeSelected(i);
                        edgeVisited(i)  = e.edgeVisited(i);
                        edgeFaux(i)     = e.edgeFaux(i);
                    }
                }
            }
        }
    }

    void serialize(std::ostream& os) const
    {
        flags().serialize(os);
        edgeFlags().serialize(os);
    }

    void deserialize(std::istream& is)
    {
        flags().deserialize(is);
        edgeFlags().deserialize(is);
    }

    // ContainerComponent interface functions
    void resize(uint n) requires (N < 0) { edgeFlags().resize(n); }

    void pushBack(BitSet<FT> f = BitSet<FT>()) requires (N < 0)
    {
        edgeFlags().pushBack(f);
    }

    void insert(uint i, BitSet<FT> f = BitSet<FT>()) requires (N < 0)
    {
        edgeFlags().insert(i, f);
    }

    void erase(uint i) requires (N < 0) { edgeFlags().erase(i); }

    void clear() requires (N < 0) { edgeFlags().clear(); }

private:
    // members that allow to access the flags, trough data (horizontal) or
    // trough parent (vertical)

    BitSet<FT>& flags() { return Base::additionalData(); }

    const BitSet<FT>& flags() const { return Base::additionalData(); }

    Vector<BitSet<FT>, -1>& edgeFlags() { return Base::container(); }

    const Vector<BitSet<FT>, -1>& edgeFlags() const
    {
        return Base::container();
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
