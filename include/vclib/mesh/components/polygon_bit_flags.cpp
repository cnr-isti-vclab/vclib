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

#include "polygon_bit_flags.h"

namespace vcl::comp {

/**
 * @private
 * @brief Constructor that initializes the bits to false.
 */
template<int N, typename El, bool O>
PolygonBitFlags<N, El, O>::PolygonBitFlags()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

/**
 * @private
 * @brief Initializes the bits to false.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<int N, typename El, bool O>
void PolygonBitFlags<N, El, O>::init()
{
	flags().reset();
}

/**
 * @private
 * @brief Returns `true` if the component is enabled, `false` otherwise.
 * This member function can return `false` only if the component is optional.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is enabled, `false` otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns whether the current Polygon is deleted or not.
 * @return true if the Polygon is deleted, false otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::deleted() const
{
	return flags()[DELETED];
}

/**
 * @brief Accesses the 'selected' bit of this Polygon, returning a reference to
 * it.
 * @return a reference to the 'selected' bit of this Polygon.
 */
template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::selected()
{
	return flags()[SELECTED];
}

/**
 * @brief Returns whether the current Polygon is selected or not.
 * @return true if the Polygon is selected, false otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::selected() const
{
	return flags()[SELECTED];
}

/**
 * @brief Accesses the 'visited' bit of this Polygon, returning a reference to
 * it.
 * @return a reference to the 'visited' bit of this Polygon.
 */
template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::visited()
{
	return flags()[VISITED];
}

/**
 * @brief Returns whether the current Polygon has been visited or not.
 * @return true if the Polygon has been visited, false otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::visited() const
{
	return flags()[VISITED];
}

/**
 * @brief Returns whether the current Polygon is on border or not, by checking
 * whether at least one of its edges are on border or not.
 * @return true if at least one edge of the Polygon is on border, false
 * otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::onBorder() const
{
	for (uint i = 0; i < 12; ++i)
		if (edgeOnBorder(i)) return true;
	return false;
}

/**
 * @brief Accesses the 'onBorder' bit of the i-th edge of the polygon, returning
 * a reference to it.
 * @param[in] i: the index of the edge.
 * @return a reference to the 'onBorder' bit of the i-th edge of the polygon.
 */
template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::edgeOnBorder(uint i)
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGEBORD];
}

/**
 * @brief Returns whether the i-th edge of the current Polygon is on border or
 * not.
 * @param[in] i: the index of the edge.
 * @return true if the i-th edge of the Polygon is on border, false otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::edgeOnBorder(uint i) const
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGEBORD];
}

/**
 * @brief Accesses the 'selected' bit of the i-th edge of the polygon, returning
 * a reference to it.
 * @param[in] i: the index of the edge.
 * @return a reference to the 'selected' bit of the i-th edge of the polygon.
 */
template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::edgeSelected(uint i)
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGESEL];
}

/**
 * @brief Returns whether the i-th edge of the current Polygon is selected or
 * not.
 * @param[in] i: the index of the edge.
 * @return true if the i-th edge of the Polygon is selected, false otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::edgeSelected(uint i) const
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGESEL];
}

/**
 * @brief Accesses the 'visited' bit of the i-th edge of the polygon, returning
 * a reference to it.
 * @param[in] i: the index of the edge.
 * @return a reference to the 'visited' bit of the i-th edge of the polygon.
 */
template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::edgeVisited(uint i)
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGEVIS];
}

/**
 * @brief Returns whether the i-th edge of the current Polygon has been visited
 * or not.
 * @param[in] i: the index of the edge.
 * @return true if the i-th edge of the Polygon has been visited, false
 * otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::edgeVisited(uint i) const
{
	assert(i < edgeFlags().size());
	return edgeFlags()[i][EDGEVIS];
}

/**
 * @brief Accesses the 'faux' bit of the i-th edge of the polygon, returning
 * a reference to it.
 *
 * @note The 'faux' bit is used to just for portability with triangle meshes
 * that are stored in polygonal meshes. However, these flags should be used only
 * when the mesh is a Triangle mesh, that is when each face has
 * vertexNumber()== 3.
 *
 * @param[in] i: the index of the edge, it must be less than 3.
 * @return a reference to the 'faux' bit of the i-th edge of the polygon.
 */
template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::edgeFaux(uint i)
{
	assert(i < 3);
	return flags()[FAUX0 + i];
}

/**
 * @brief Returns whether the i-th edge of the current Polygon is faux or
 * not.
 *
 * @note The 'faux' bit is used to just for portability with triangle meshes
 * that are stored in polygonal meshes. However, these flags should be used only
 * when the mesh is a Triangle mesh, that is when each face has
 * vertexNumber()== 3.
 *
 * @param[in] i: the index of the edge, it must be less than 3.
 * @return true if the i-th edge of the Polygon is faux, false otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::edgeFaux(uint i) const
{
	assert(i < 3);
	return flags()[FAUX0 + i];
}

/**
 * @brief Returns the boolean value of the user bit of this Polygon given in
 * input. The bit is checked to be less than the total number of assigned user
 * bits, which in this class is 2.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 1], that will
 * be returned by reference.
 * @return `true` if the required bit is enabled, `false` otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::userBit(uint bit) const
{
	assert(bit < USER_BITS_NUMBER);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Returns a reference to the value of the user bit of this Polygon given
 * in input. The bit is checked to be less than the total number of assigned
 * user bits, which in this class is 2.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 1].
 * @return a reference to the desired user bit.
 */
template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::userBit(uint bit)
{
	assert(bit < USER_BITS_NUMBER);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Returns the boolean value of the user bit of the i-th edge of this
 * Polygon given in input. The bit is checked to be less than the total number
 * of assigned edge user bits, which in this class is 5.
 *
 * @param[in] i: the index of the edge.
 * @param[in] bit: the position of the bit, in the interval [0 - 4].
 * @return `true` if the required bit is enabled, `false` otherwise.
 */
template<int N, typename El, bool O>
bool PolygonBitFlags<N, El, O>::edgeUserBit(uint i, uint bit) const
{
	assert(bit < EDGE_USER_BITS_NUMBER);
	return edgeFlags()[i][bit + FIRST_EDGE_USER_BIT];
}

/**
 * @brief Returns a reference to the value of the user bit of the i-th edge of
 * this Polygon given in input. The bit is checked to be less than the total
 * number of assigned edge user bits, which in this class is 5.
 *
 * @param[in] i: the index of the edge.
 * @param[in] bit: the position of the bit, in the interval [0 - 4].
 * @return a reference to the desired user bit.
 */
template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::edgeUserBit(uint i, uint bit)
{
	assert(bit < EDGE_USER_BITS_NUMBER);
	return edgeFlags()[i][bit + FIRST_EDGE_USER_BIT];
}

/**
 * @brief Unsets all the flags of this Polygon and sets them to `false`,
 * **except the deleted flag**, which needs to be manually reset.
 */
template<int N, typename El, bool O>
void PolygonBitFlags<N, El, O>::resetBitFlags()
{
	bool isD = deleted();
	flags().reset();
	for (uint i = 0; i < edgeFlags().size(); ++i)
		edgeFlags()[i].reset();
	deleted() = isD;
}

/**
 * @brief Sets all the flags of this element to the values contained in the
 * integer input parameter, that represents the bit flags of the VCG library.
 * @param[in] f: bit flags in the format of the VCG library.
 */
template<int N, typename El, bool O>
void PolygonBitFlags<N, El, O>::importFromVCGFlags(int f)
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
 * @return an integer representing the bit flags of this element in the format
 * of the VCG library.
 */
template<int N, typename El, bool O>
int PolygonBitFlags<N, El, O>::exportToVCGFlags() const
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

template<int N, typename El, bool O>
BitProxy<typename PolygonBitFlags<N, El, O>::FT>
PolygonBitFlags<N, El, O>::deleted()
{
	return flags()[DELETED];
}

template<int N, typename El, bool O>
template<typename Element>
void PolygonBitFlags<N, El, O>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		resetBitFlags();
		if constexpr (HasPolygonBitFlags<Element>) {
			flags() = e.flags();
			edgeFlags() = e.edgeFlags();
		}
		else {
			// BitFlags
			deleted() = e.deleted();
			selected() = e.selected();
			visited() = e.visited();
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

template<int N, typename El, bool O>
void PolygonBitFlags<N, El, O>::resize(uint n) requires (N < 0)
{
	edgeFlags().resize(n);
}

template<int N, typename El, bool O>
void PolygonBitFlags<N, El, O>::pushBack(BitSet<FT> f) requires (N < 0)
{
	edgeFlags().pushBack(f);
}

template<int N, typename El, bool O>
void PolygonBitFlags<N, El, O>::insert(uint i, BitSet<FT> f) requires (N < 0)
{
	edgeFlags().insert(edgeFlags().begin() + i, f);
}

template<int N, typename El, bool O>
void PolygonBitFlags<N, El, O>::erase(uint i) requires (N < 0)
{
	edgeFlags().erase(edgeFlags().begin() + i);
}

template<int N, typename El, bool O>
void PolygonBitFlags<N, El, O>::clear() requires (N < 0)
{
	edgeFlags().clear();
}

template<int N, typename El, bool O>
BitSet<typename PolygonBitFlags<N, El, O>::FT>&
PolygonBitFlags<N, El, O>::flags()
{
	return Base::additionalData(this);
}

template<int N, typename El, bool O>
const BitSet<typename PolygonBitFlags<N, El, O>::FT>&
PolygonBitFlags<N, El, O>::flags() const
{
	return Base::additionalData(this);
}

template<int N, typename El, bool O>
Vector<BitSet<typename PolygonBitFlags<N, El, O>::FT>, -1>&
PolygonBitFlags<N, El, O>::edgeFlags()
{
	return Base::container(this);
}

template<int N, typename El, bool O>
const Vector<BitSet<typename PolygonBitFlags<N, El, O>::FT>, -1>&
PolygonBitFlags<N, El, O>::edgeFlags() const
{
	return Base::container(this);
}

} // namespace vcl::comp
