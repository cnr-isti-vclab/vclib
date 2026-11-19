// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H

#include "base/bit_flags_concepts.h"

#include <vclib/space/core.h>

#include <cassert>
#include <istream>
#include <ostream>

namespace vcl::comp {

/**
 * @brief Struct that holds the data for PolygonBitFlags: a BitSet<uint> for the
 * face flags and a Vector of BitSet<uchar> for the per-edge flags.
 *
 * @tparam N: The size of the container representing the number of storable bit
 * flags for each edge of the Polygon. If negative (default -1), the container
 * is dynamic and tied to the vertex count.
 */
template<int N = -1>
struct PolygonBitFlagsData
{
    BitSet<uint>                  flags;
    vcl::Vector<BitSet<uchar>, N> edgeFlags;

    PolygonBitFlagsData() = default;

    explicit PolygonBitFlagsData(std::size_t n) requires (N < 0)
            : edgeFlags(n)
    {
    }

    decltype(auto) operator[](this auto&& self, std::size_t bit)
    {
        return self.flags[bit];
    }

    bool operator==(const PolygonBitFlagsData&) const = default;

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, flags, edgeFlags);
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, flags, edgeFlags);
    }
};

/**
 * @brief The PolygonBitFlags class represents a collection of 32 bits plus 8
 * bits for each edge that will be part of a generic Polygonal Face of a Mesh.
 *
 * This is a specialization class of the BitFlags component, meaning that it can
 * be used in substitution to that component. The difference is that this class
 * is meant to be used on Polygonal Faces, and adds bits with particular
 * meanings for polygons.
 *
 * This class also provides 3 flags for faux edges. These flags are added just
 * to make portable all the algorithms that use faux flags also for
 * PolygonMeshes. However, these flags should be used only when the mesh is a
 * Triangle mesh, that is when each face has vertexCount() == 3.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Polygon has been deleted - read only
 * - 1: selected: if the current Polygon has been selected
 * - 2: visited: if the current Polygon has been visited (useful for some visit
 *               algorithms)
 * - from 3 to 5: edge faux: if the current Face has its i-th edge (i in [0, 2])
 *                marked as faux
 * - from 6 to 14: unused for compatibility with TriangleBitFlags
 * - from 15 to 31: user bits that can have custom meanings to the user
 *
 * This class provides 17 user bits, that can be accessed using the member
 * function userBit(uint i) with position in the interval [0, 16].
 *
 * Additionally, this class provides the following bits for each edge of the
 * Polygonal Face:
 * - 0: edge border: if the current Polygonal face has the i-th edge on border
 * - 1: edge selection: if the current Polygonal face has the i-th edge selected
 * - 2: edge visited: if the i-th edge of the current Polygonal face has been
 *                    visited
 * - from 3 to 7: user bits of the i-th edge that can have custom meanings to
 *                the user
 *
 * This class provides 5 edge user bits for each edge, that can be accessed
 * using the member function edgeUserBit(uint i, uint bit) with position in the
 * interval [0, 4].
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the PolygonBitFlags
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * f.edgeSelected(1) = true;
 * @endcode
 *
 * @note This component is *Tied To Vertex Count*: it means that the size of
 * the container, if dynamic, will change automatically along the Vertex Count
 * of the Element.
 *
 * @tparam N: The size of the container, that will represent the number of
 * storable bit flags for each edge of the Polygon. If negative (default -1),
 * the container is dynamic and tied to the vertex number.
 *
 * @ingroup components
 */
template<int N = -1>
class PolygonBitFlags :
        public Component<CompId::BIT_FLAGS, PolygonBitFlagsData<N>>
{
    using Base = Component<CompId::BIT_FLAGS, PolygonBitFlagsData<N>>;

    static constexpr uint FIRST_USER_BIT      = 15;
    static constexpr uint FIRST_EDGE_USER_BIT = 3;

    // indices of the bits
    enum {
        DELETED  = 0, // bit 0
        SELECTED = 1, // bit 1
        VISITED  = 2, // bit 2
        // Faux edges, for portability with TriangleBits
        FAUX0 = 3, // bits [3, 5]
    };

    // indices of the bits used for each edge of the polygon
    enum { EDGEBORD = 0, EDGESEL = 1, EDGEVIS = 2 };

public:
    /**
     * @brief Expose the underlying type of the BitFlags.
     */
    using FlagsType = uint;

    /**
     * @brief Expose the underlying type of the Edge BitFlags.
     */
    using EdgeFlagsType = uchar;

    /**
     * @brief Static number of bits that can have custom meanings to the user
     */
    static constexpr uint USER_BIT_COUNT =
        sizeof(FlagsType) * 8 - FIRST_USER_BIT;

    /**
     * @brief Static number of bits for each edge that can have custom meanings
     * to the user
     */
    static constexpr uint EDGE_USER_BIT_COUNT =
        sizeof(EdgeFlagsType) * 8 - FIRST_EDGE_USER_BIT;

    /**
     * @brief Compile-time edge count (-1 if dynamic).
     */
    static constexpr int EDGE_COUNT = N;

    /**
     * @brief Tells if this component stores a container having its size tied to
     * the number of the vertices of the Element.
     */
    static constexpr bool TIED_TO_VERTEX_COUNT = (N < 0);

    /* Member functions */

    /**
     * @brief Returns whether the current Polygon is deleted or not.
     * @return true if the Polygon is deleted, false otherwise.
     */
    bool deleted(this auto&& self)
    {
        if (!self.isValid())
            return false;
        return self.flags()[DELETED];
    }

    /**
     * @brief Returns whether the current Polygon is selected or not.
     * @return a reference (or value for const) to the 'selected' bit of this
     * Polygon.
     */
    decltype(auto) selected(this auto&& self) { return self.flags()[SELECTED]; }

    /**
     * @brief Returns whether the current Polygon has been visited or not.
     * @return a reference (or value for const) to the 'visited' bit of this
     * Polygon.
     */
    decltype(auto) visited(this auto&& self) { return self.flags()[VISITED]; }

    /**
     * @brief Returns whether the current Polygon is on border or not, by
     * checking whether at least one of its edges are on border or not.
     * @return true if at least one edge of the Polygon is on border, false
     * otherwise.
     */
    bool onBorder(this auto&& self)
    {
        const uint n = self.edgeFlags().size();
        for (uint i = 0; i < n; ++i) {
            if (self.edgeOnBorder(i))
                return true;
        }
        return false;
    }

    /**
     * @brief Returns whether the i-th edge of the current Polygon is on border
     * or not.
     * @param[in] i: the index of the edge.
     * @return a reference (or value for const) to the 'onBorder' bit of the
     * i-th edge of the polygon.
     */
    decltype(auto) edgeOnBorder(this auto&& self, uint i)
    {
        assert(i < self.edgeFlags().size());
        return self.edgeFlags()[i][EDGEBORD];
    }

    /**
     * @brief Returns whether the i-th edge of the current Polygon is selected
     * or not.
     * @param[in] i: the index of the edge.
     * @return a reference (or value for const) to the 'selected' bit of the
     * i-th edge of the polygon.
     */
    decltype(auto) edgeSelected(this auto&& self, uint i)
    {
        assert(i < self.edgeFlags().size());
        return self.edgeFlags()[i][EDGESEL];
    }

    /**
     * @brief Returns whether the i-th edge of the current Polygon has been
     * visited or not.
     * @param[in] i: the index of the edge.
     * @return a reference (or value for const) to the 'visited' bit of the
     * i-th edge of the polygon.
     */
    decltype(auto) edgeVisited(this auto&& self, uint i)
    {
        assert(i < self.edgeFlags().size());
        return self.edgeFlags()[i][EDGEVIS];
    }

    /**
     * @brief Returns whether the i-th edge of the current Polygon is faux or
     * not.
     *
     * @note The 'faux' bit is used just for portability with triangle meshes
     * that are stored in polygonal meshes. However, these flags should be used
     * only when the mesh is a Triangle mesh, that is when each face has
     * vertexCount() == 3.
     *
     * @param[in] i: the index of the edge, it must be less than 3.
     * @return a reference (or value for const) to the 'faux' bit of the i-th
     * edge of the polygon.
     */
    decltype(auto) edgeFaux(this auto&& self, uint i)
    {
        assert(i < 3);
        return self.flags()[FAUX0 + i];
    }

    /**
     * @brief Returns a reference to the value of the user bit of this Polygon
     * given in input. The bit is checked to be less than the total number of
     * assigned user bits, which in this class is 17.
     *
     * @param[in] bit: the position of the bit, in the interval [0 - 16].
     * @return a reference (or value for const) to the desired user bit.
     */
    decltype(auto) userBit(this auto&& self, uint bit)
    {
        assert(bit < USER_BIT_COUNT);
        return self.flags()[bit + FIRST_USER_BIT];
    }

    /**
     * @brief Returns a reference to the value of the user bit of the i-th edge
     * of this Polygon given in input. The bit is checked to be less than the
     * total number of assigned edge user bits, which in this class is 5.
     *
     * @param[in] i: the index of the edge.
     * @param[in] bit: the position of the bit, in the interval [0 - 4].
     * @return a reference (or value for const) to the desired edge user bit.
     */
    decltype(auto) edgeUserBit(this auto&& self, uint i, uint bit)
    {
        assert(i < self.edgeFlags().size());
        assert(bit < EDGE_USER_BIT_COUNT);
        return self.edgeFlags()[i][bit + FIRST_EDGE_USER_BIT];
    }

    /**
     * @brief Unsets all the flags of this Polygon and sets them to `false`,
     * **except the deleted flag**, which needs to be manually reset.
     */
    void resetBitFlags(this auto&& self)
    {
        bool isD = self.deleted();
        self.flags().reset();
        const uint n = self.edgeFlags().size();
        for (uint i = 0; i < n; ++i) {
            self.edgeFlags()[i].reset();
        }
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
     * @brief Returns the underlying integral value of the Edge BitFlags of the
     * i-th edge.
     * @param[in] i: the index of the edge.
     * @return the underlying integral value of the Edge BitFlags of the i-th
     * edge.
     */
    EdgeFlagsType underlyingEdgeBitFlags(this auto&& self, uint i)
    {
        assert(i < self.edgeFlags().size());
        return self.edgeFlags()[i].underlying();
    }

    /**
     * @brief Sets the underlying integral value of the Edge BitFlags of the
     * i-th edge.
     *
     * @param[in] i: the index of the edge.
     * @param[in] bits: the integral value to set as the underlying value of the
     * Edge BitFlags of the i-th edge.
     */
    void setUnderlyingEdgeBitFlags(this auto&& self, uint i, EdgeFlagsType bits)
    {
        assert(i < self.edgeFlags().size());
        self.edgeFlags()[i].setUnderlying(bits);
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
        if (self.edgeFlags().size() > 0 && (f & 0x00000040))
            self.edgeOnBorder(0) = true;
        if (self.edgeFlags().size() > 1 && (f & 0x00000080))
            self.edgeOnBorder(1) = true;
        if (self.edgeFlags().size() > 2 && (f & 0x00000100))
            self.edgeOnBorder(2) = true;
        if (self.edgeFlags().size() > 0 && (f & 0x00008000))
            self.edgeSelected(0) = true;
        if (self.edgeFlags().size() > 1 && (f & 0x00010000))
            self.edgeSelected(1) = true;
        if (self.edgeFlags().size() > 2 && (f & 0x00020000))
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
        if (self.edgeFlags().size() > 0 && self.edgeOnBorder(0))
            f |= 0x00000040;
        if (self.edgeFlags().size() > 1 && self.edgeOnBorder(1))
            f |= 0x00000080;
        if (self.edgeFlags().size() > 2 && self.edgeOnBorder(2))
            f |= 0x00000100;
        if (self.edgeFlags().size() > 0 && self.edgeSelected(0))
            f |= 0x00008000;
        if (self.edgeFlags().size() > 1 && self.edgeSelected(1))
            f |= 0x00010000;
        if (self.edgeFlags().size() > 2 && self.edgeSelected(2))
            f |= 0x00020000;
        if (self.edgeFaux(0))
            f |= 0x00040000;
        if (self.edgeFaux(1))
            f |= 0x00080000;
        if (self.edgeFaux(2))
            f |= 0x00100000;
        return f;
    }

    /**
     * @brief Returns the number of edges of the Polygon.
     * @return the number of edges of the Polygon.
     */
    uint edgeCount(this auto&& self)
    {
        return self.edgeFlags().size();
    }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);

    void setDeleted(this auto&& self, bool b = true)
    {
        self.flags()[DELETED] = b;
    }

    decltype(auto) deletedBit(this auto&& self)
    {
        return self.flags()[DELETED];
    }

    // ContainerComponent / TTVC interface functions
    void resize(this auto&& self, uint n) requires (N < 0)
    {
        self.edgeFlags().resize(n);
    }

    void pushBack(
        this auto&&           self,
        BitSet<EdgeFlagsType> f = BitSet<EdgeFlagsType>()) requires (N < 0)
    {
        self.edgeFlags().pushBack(f);
    }

    void insert(
        this auto&&           self,
        uint                  pos,
        BitSet<EdgeFlagsType> f = BitSet<EdgeFlagsType>()) requires (N < 0)
    {
        self.edgeFlags().insert(pos, f);
    }

    void erase(this auto&& self, uint pos) requires (N < 0)
    {
        self.edgeFlags().erase(pos);
    }

    void clear(this auto&& self) requires (N < 0) { self.edgeFlags().clear(); }

private:
    template<int OtherN>
    friend class PolygonBitFlags;

    decltype(auto) data(this auto&& self) { return Base::compData(self); }

    decltype(auto) flags(this auto&& self) { return (self.data().flags); }

    decltype(auto) edgeFlags(this auto&& self)
    {
        return (self.data().edgeFlags);
    }
};

/* Implementation of member functions */

template<int N>
template<typename OtherElement>
void PolygonBitFlags<N>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool)
{
    if constexpr (HasBitFlags<OtherElement>) {
        const bool wasDeleted = self.deleted();
        const bool isDeleted  = other.deleted();
        self.resetBitFlags();
        if constexpr (HasPolygonBitFlags<OtherElement>) {
            self.setUnderlyingBitFlags(other.underlyingBitFlags());
            self.setDeleted(isDeleted);
            const uint n = std::min(
                (uint) self.edgeCount(),
                (uint) other.edgeCount());
            for (uint i = 0; i < n; ++i) {
                self.setUnderlyingEdgeBitFlags(i, other.underlyingEdgeBitFlags(i));
            }
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
            if constexpr (HasTriangleBitFlags<OtherElement>) {
                const uint numEdges =
                    std::min((uint) self.edgeCount(), 3u);
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

#endif // VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
