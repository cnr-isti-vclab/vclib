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

#ifndef VCL_MESH_COMPONENTS_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_BIT_FLAGS_H

#include <vclib/concepts/mesh/components/bit_flags.h>
#include <vclib/space/bit_set.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The BitFlags component class represents a collection of 8 bits that
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
 * - other: user bits that can have custom meanings to the user
 *
 * This class provides 4 user bits, that can be accessed using the member
 * function userBit(uint i) with position in the interval [0, 3].
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
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
class BitFlags :
		public Component<
			BitFlags<ElementType, OPT>,
			BIT_FLAGS,
			BitSet<char>,
			ElementType,
			OPT>
{
	using Base = Component<
		BitFlags<ElementType, OPT>,
		BIT_FLAGS,
		BitSet<char>,
		ElementType,
		OPT>;

	using FT = char; // FlagsType, the integral type used for the flags

	// indices of the bits
	enum {
		DELETED  = 0, // bit 0
		SELECTED = 1, // bit 1
		BORDER   = 2, // bit 2
		VISITED  = 3  // bit 3
	};

	static const uint FIRST_USER_BIT = 4;

public:
	/**
	 * @brief Static number of bits that can have custom meanings to the user
	 */
	static const uint USER_BITS_NUMBER = sizeof(FT) * 8 - FIRST_USER_BIT;

	/* Constructors */

	/**
	 * @brief Initializes the bits to `false`.
	 */
	BitFlags()
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
	 * @brief Returns whether the current Element is deleted or not.
	 * @return true if the Element is deleted, false otherwise.
	 */
	bool deleted() const { return flags()[DELETED]; }

	/**
	 * @brief Accesses the 'selected' bit of this Element, returning a reference
	 * to it.
	 * @return a reference to the 'selected' bit of this Element.
	 */
	BitProxy<FT> selected() { return flags()[SELECTED]; }

	/**
	 * @brief Returns whether the current Element is selected or not.
	 * @return true if the Element is selected, false otherwise.
	 */
	bool selected() const { return flags()[SELECTED]; }

	/**
	 * @brief Accesses the 'onBorder' bit of this Element, returning a reference
	 * to it.
	 * @return a reference to the 'onBorder' bit of this Element.
	 */
	BitProxy<FT> onBorder() { return flags()[BORDER]; }

	/**
	 * @brief Returns whether the current Element is on border or not.
	 * @return true if the Element is on border, false otherwise.
	 */
	bool onBorder() const { return flags()[BORDER]; }

	/**
	 * @brief Accesses the 'visited' bit of this Element, returning a reference
	 * to it.
	 * @return a reference to the 'visited' bit of this Element.
	 */
	BitProxy<FT> visited() { return flags()[VISITED]; }

	/**
	 * @brief Returns whether the current Element has been visited or not.
	 * @return true if the Element has been visited, false otherwise.
	 */
	bool visited() const { return flags()[VISITED]; }

	/**
	 * @brief Returns a reference to the value of the user bit of this Element
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
	 * @brief Returns the boolean value of the user bit of this Element given in
	 * input. The bit is checked to be less than the total number of assigned
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
	 * @brief Unsets all the flags of this Element and sets them to `false`,
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
		resetBitFlags();
		if (f & 0x0010)
			visited() = true;
		if (f & 0x0020)
			selected() = true;
		if (f & 0x0100)
			onBorder() = true;
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
			f &= 0x0010;
		if (selected())
			f &= 0x0020;
		if (onBorder())
			f &= 0x0100;
		return f;
	}

protected:
	BitProxy<FT> deletedBit() { return flags()[DELETED]; }

	// Component interface function
	template<typename Element>
	void importFrom(const Element& e)
	{
		if constexpr (HasBitFlags<Element>) {
			resetBitFlags();
			if constexpr (
				HasPolygonBitFlags<Element> || HasTriangleBitFlags<Element>)
			{
				deletedBit() = e.deleted();
				selected() = e.selected();
				visited() = e.visited();
				onBorder() = e.onBorder();
				const uint UM = std::min(USER_BITS_NUMBER, e.USER_BITS_NUMBER);
				for (uint i = 0; i < UM; ++i)
					userBit(i) = e.userBit(i);
			}
			else {
				flags() = e.flags();
			}

		}
	}

private:
	// members that allow to access the flags, trough data (horizontal) or
	// trough parent (vertical)
	BitSet<FT>& flags()
	{
		return Base::data();
	}

	BitSet<FT> flags() const
	{
		return Base::data();
	}
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BIT_FLAGS_H
