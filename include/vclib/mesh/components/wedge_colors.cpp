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

#include "wedge_colors.h"

#include <vclib/concepts/mesh/components/color.h>

namespace vcl::comp {

/**
 * @brief Returns a reference to the i-th wedge color of the element.
 *
 * You can use this function to set the i-th color of the element:
 *
 * @code{.cpp}
 * f.wedgeColor(0) = vcl::Color::Red;
 * @endcode
 *
 * @param[in] i: the index of the wedge color to return. The value must be
 * between 0 and the number of vertices of the element.
 * @return A reference to the i-th wedge color of the element.
 */
template<int N, typename El, bool O>
vcl::Color& WedgeColors<N, El, O>::wedgeColor(uint i)
{
	return colors().at(i);
}

/**
 * @brief Returns a const reference to the i-th wedge color of the element.
 *
 * @param[in] i: the index of the wedge color to return. The value must be
 * between 0 and the number of vertices of the element.
 * @return A const reference to the i-th wedge color of the element.
 */
template<int N, typename El, bool O>
const vcl::Color& WedgeColors<N, El, O>::wedgeColor(uint i) const
{
	return colors().at(i);
}

/**
 * @brief Returns a reference to the i-th wedge color of the element but using
 * as index the module between i and the number of vertices of the element. You
 * can use this function if you need to get the "next wedge color after
 * position k", without check if it is less than the number of vertices. Works
 * also for negative numbers:
 *
 * @code{.cpp}
 * f.wedgeColorMod(-1) = vcl::Color::Red; // the wedge color in position
 *                                        // vertexNumber() - 1
 * @endcode
 *
 * @param[in] i: the position of the required wedge color in the container,
 * w.r.t. the position 0; value is modularized on vertexNumber().
 * @return A reference to the required wedge color of the element.
 */
template<int N, typename El, bool O>
vcl::Color& WedgeColors<N, El, O>::wedgeColorMod(int i)
{
	return colors().atMod(i);
}

/**
 * @brief Same of wedgeColorMod(int) but returns a const reference.
 * @param[in] i: the position of the required wedge color in the container,
 * w.r.t. the position 0; value is modularized on vertexNumber().
 * @return A const reference to the required wedge color of the element.
 */
template<int N, typename El, bool O>
const vcl::Color& WedgeColors<N, El, O>::wedgeColorMod(int i) const
{
	return colors().atMod(i);
}

/**
 * @brief Sets the i-th wedge color of the element.
 * @param[in] c: the new wedge color.
 * @param[in] i: the position in the container on which set the wedge color; the
 * value must be between 0 and the number of vertices of the element.
 */
template<int N, typename El, bool O>
void WedgeColors<N, El, O>::setWedgeColor(const vcl::Color& c, uint i)
{
	colors().set(c, i);
}

/**
 * @brief Sets all the wedge colors of the element.
 *
 * If the size of the container is static, the size of the input range must be
 * the same one of the container.
 *
 * @tparam Rng: The type of the range of wedge colors to set. The value type
 * of the range must be convertible to a vcl::Color.
 *
 * @param[in] r: range of colors to set.
 */
template<int N, typename El, bool O>
template<Range Rng>
void WedgeColors<N, El, O>::setWedgeColors(Rng&& r)
	requires RangeOfConvertibleTo<Rng, vcl::Color>
{
	colors().set(r);
}

/**
 * @brief Returns an iterator to the first wedge color in the container of
 * this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<int N, typename El, bool O>
auto WedgeColors<N, El, O>::wedgeColorBegin() -> WedgeColorsIterator
{
	return colors().begin();
}

/**
 * @brief Returns an iterator to the end of the container of this component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<int N, typename El, bool O>
auto WedgeColors<N, El, O>::wedgeColorEnd() -> WedgeColorsIterator
{
	return colors().end();
}

/**
 * @brief Returns a const iterator to the first wedge color in the container of
 * this component.
 *
 * @return a const iterator pointing to the begin of this container.
 */
template<int N, typename El, bool O>
auto WedgeColors<N, El, O>::wedgeColorBegin() const -> ConstWedgeColorsIterator
{
	return colors().begin();
}

/**
 * @brief Returns a const iterator to the end of the container of this
 * component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<int N, typename El, bool O>
auto WedgeColors<N, El, O>::wedgeColorEnd() const -> ConstWedgeColorsIterator
{
	return colors().end();
}

/**
 * @brief Returns a lightweight view object that stores the begin and end
 * iterators of the container of wedge colors of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (auto& wc : el.wedgeColors()) {
 *     // Do something with wc
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over wedge colors.
 */
template<int N, typename El, bool O>
auto WedgeColors<N, El, O>::wedgeColors() -> View<WedgeColorsIterator>
{
	return View(wedgeColorBegin(), wedgeColorEnd());
}

/**
 * @brief Returns a lightweight const view object that stores the begin and end
 * iterators of the container of wedge colors of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (const auto& wc : el.wedgeColors()) {
 *     // Do something read-only with wc
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over wedge colors.
 */
template<int N, typename El, bool O>
auto WedgeColors<N, El, O>::wedgeColors() const
	-> View<ConstWedgeColorsIterator>
{
	return View(wedgeColorBegin(), wedgeColorEnd());
}

template<int N, typename El, bool O>
template<typename Element>
void WedgeColors<N, El, O>::importFrom(const Element& e)
{
	if constexpr (HasWedgeColors<Element>) {
		if (isWedgeColorsAvailableOn(e)) {
			if constexpr(N > 0) {
				// same static size
				if constexpr (N == Element::WEDGE_COLOR_NUMBER) {
					importWedgeColorsFrom(e);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::WEDGE_COLOR_NUMBER < 0){
					if (e.vertexNumber() == N){
						importWedgeColorsFrom(e);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resize(e.vertexNumber());
				importWedgeColorsFrom(e);
			}
		}
	}
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::resize(uint n) requires (N < 0)
{
	colors().resize(n);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::pushBack(const vcl::Color& c) requires (N < 0)
{
	colors().pushBack(c);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::insert(uint i, const vcl::Color& c) requires (N < 0)
{
	colors().insert(i, c);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::erase(uint i) requires (N < 0)
{
	colors().erase(i);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::clear() requires (N < 0)
{
	colors().clear();
}

template<int N, typename El, bool O>
template<typename Element>
void WedgeColors<N, El, O>::importWedgeColorsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeColor(i) = e.wedgeColor(i);
	}
}

template<int N, typename El, bool O>
Vector<vcl::Color, N>& WedgeColors<N, El, O>::colors()
{
	return Base::container();
}

template<int N, typename El, bool O>
const Vector<vcl::Color, N>& WedgeColors<N, El, O>::colors() const
{
	return Base::container();
}

/**
 * @brief Checks if the given Element has WedgeColors available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that satisfies
 * the ElementConcept.
 * @return `true` if the element has WedgeColors available, `false` otherwise.
 */
bool isWedgeColorsAvailableOn(const ElementConcept auto& element)
{
	return isComponentAvailableOn<WEDGE_COLORS>(element);
}

} // namespace vcl::comp
