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

#include "wedge_tex_coords.h"

#include <vclib/concepts/mesh/components/tex_coord.h>

namespace vcl::comp {

/**
 * @private
 * @brief Returns `true` if the component is available, `false` otherwise.
 *
 * This member function can return `false` only if the component is optional,
 * and it is not enabled.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is available, `false` otherwise.
 */
template<typename Scalar, int N, typename El, bool O>
bool WedgeTexCoords<Scalar, N, El, O>::isAvailable() const
{
	return Base::isAvailable(this);
}

/**
 * @brief Returns a reference to the i-th wedge texcoord of the element.
 *
 * You can use this function to set the i-th texcoord of the element:
 *
 * @code{.cpp}
 * f.wedgeTexCoord(0) = {0.5, 0.5};
 * @endcode
 *
 * @param[in] i: the index of the wedge texcoord to return. The value must be
 * between 0 and the number of vertices of the element.
 * @return A reference to the i-th wedge texcoord of the element.
 */
template<typename Scalar, int N, typename El, bool O>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoord(uint i)
{
	return texCoords().at(i);
}

/**
 * @brief Returns a const reference to the i-th wedge texcoord of the element.
 *
 * @param[in] i: the index of the wedge texcoord to return. The value must be
 * between 0 and the number of vertices of the element.
 * @return A const reference to the i-th wedge texcoord of the element.
 */
template<typename Scalar, int N, typename El, bool O>
const vcl::TexCoord<Scalar>&
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoord(uint i) const
{
	return texCoords().at(i);
}

/**
 * @brief Returns a reference to the i-th wedge texcoord of the element but
 * using as index the module between i and the number of vertices of the
 * element. You can use this function if you need to get the "next wedge
 * texcoord after position k", without check if it is less than the number of
 * vertices. Works also for negative numbers:
 *
 * @code{.cpp}
 * f.wedgeTexCoordMod(-1) = {0.1, 0.2}; // the wedge texcoord in position
 *                                      // vertexNumber() - 1
 * @endcode
 *
 * @param[in] i: the position of the required wedge texcoord in the container,
 * w.r.t. the position 0; value is modularized on vertexNumber().
 * @return A reference to the required wedge texcoord of the element.
 */
template<typename Scalar, int N, typename El, bool O>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordMod(int i)
{
	return texCoords().atMod(i);
}

/**
 * @brief Same of wedgeTexCoordMod(int) but returns a const reference.
 * @param[in] i: the position of the required wedge texcoord in the container,
 * w.r.t. the position 0; value is modularized on vertexNumber().
 * @return A const reference to the required wedge texcoord of the element.
 */
template<typename Scalar, int N, typename El, bool O>
const vcl::TexCoord<Scalar>&
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordMod(int i) const
{
	return texCoords().atMod(i);
}

/**
 * @brief Sets the i-th wedge texcoord of the element.
 * @param[in] t: the new wedge texcoord.
 * @param[in] i: the position in the container on which set the wedge texcoord;
 * the value must be between 0 and the number of vertices of the element.
 */
template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::setWedgeTexCoord(
	const vcl::TexCoord<Scalar>& t,
	uint                         i)
{
	texCoords().set(t, i);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::setWedgeTexCoords(
	const std::vector<vcl::TexCoord<Scalar>>& list)
{
	texCoords().set(list);
}

/**
 * @brief Returns a reference to the texture index used to identify the texture
 * on which the wedge texture coordinates are mapped.
 *
 * @return A reference to the texture index.
 */
template<typename Scalar, int N, typename El, bool O>
short& WedgeTexCoords<Scalar, N, El, O>::textureIndex()
{
	return Base::additionalData();
}

/**
 * @brief Returns the texture index used to identify the texture on which the
 * wedge texture coordinates are mapped.
 *
 * @return The texture index.
 */
template<typename Scalar, int N, typename El, bool O>
short WedgeTexCoords<Scalar, N, El, O>::textureIndex() const
{
	return Base::additionalData();
}

/**
 * @brief Returns an iterator to the first wedge texcoord in the container of
 * this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Scalar, int N, typename El, bool O>
typename WedgeTexCoords<Scalar, N, El, O>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordBegin()
{
	return texCoords().begin();
}

/**
 * @brief Returns an iterator to the end of the container of this component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Scalar, int N, typename El, bool O>
typename WedgeTexCoords<Scalar, N, El, O>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordEnd()
{
	return texCoords().end();
}

/**
 * @brief Returns a const iterator to the first wedge texcoord in the container
 * of this component.
 *
 * @return a const iterator pointing to the begin of this container.
 */
template<typename Scalar, int N, typename El, bool O>
typename WedgeTexCoords<Scalar, N, El, O>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordBegin() const
{
	return texCoords().begin();
}

/**
 * @brief Returns a const iterator to the end of the container of this
 * component.
 *
 * @return a const iterator pointing to the end of this container.
 */
template<typename Scalar, int N, typename El, bool O>
typename WedgeTexCoords<Scalar, N, El, O>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordEnd() const
{
	return texCoords().end();
}

/**
 * @brief Returns a lightweight view object that stores the begin and end
 * iterators of the container of wedge texcoords of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (auto& tc : el.wedgeTexCoords()) {
 *     // Do something with tc
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over wedge texcoords.
 */
template<typename Scalar, int N, typename El, bool O>
auto WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoords()
{
	return View(wedgeTexCoordBegin(), wedgeTexCoordEnd());
}

/**
 * @brief Returns a lightweight const view object that stores the begin and end
 * iterators of the container of wedge texcoords of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (const auto& tc : el.wedgeTexCoords()) {
 *     // Do something read-only with tc
 * }
 * @endcode
 *
 * @return a lightweight const view object that can be used in range-based for
 * loops to iterate over wedge texcoords.
 */
template<typename Scalar, int N, typename El, bool O>
auto WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoords() const
{
	return View(wedgeTexCoordBegin(), wedgeTexCoordEnd());
}

template<typename Scalar, int N, typename El, bool O>
template<typename Element>
void WedgeTexCoords<Scalar, N, El, O>::importFrom(const Element& e)
{
	if constexpr (HasWedgeTexCoords<Element>) {
		if (isWedgeTexCoordsAvailableOn(e)) {
			if constexpr(N > 0) {
				// same static size
				if constexpr (N == Element::WEDGE_TEX_COORD_NUMBER) {
					importWedgeTexCoordsFrom(e);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::WEDGE_TEX_COORD_NUMBER < 0){
					if (e.vertexNumber() == N){
						importWedgeTexCoordsFrom(e);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic
					// size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first,
				// then import
				resize(e.vertexNumber());
				importWedgeTexCoordsFrom(e);
			}
		}
	}
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::resize(uint n) requires (N < 0)
{
	texCoords().resize(n);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::pushBack(const vcl::TexCoord<Scalar>& t)
	requires (N < 0)
{
	texCoords().pushBack(t);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::insert(
	uint                         i,
	const vcl::TexCoord<Scalar>& t) requires (N < 0)
{
	texCoords().insert(i, t);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::erase(uint i) requires (N < 0)
{
	texCoords().erase(i);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::clear() requires (N < 0)
{
	texCoords().clear();
}

template<typename Scalar, int N, typename El, bool O>
template<typename Element>
void WedgeTexCoords<Scalar, N, El, O>::importWedgeTexCoordsFrom(
	const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeTexCoord(i) = e.wedgeTexCoord(i).template cast<Scalar>();
	}
	textureIndex() = e.textureIndex();
}

template<typename Scalar, int N, typename El, bool O>
Vector<vcl::TexCoord<Scalar>, N>& WedgeTexCoords<Scalar, N, El, O>::texCoords()
{
	return Base::container();
}

template<typename Scalar, int N, typename El, bool O>
const Vector<vcl::TexCoord<Scalar>, N>&
WedgeTexCoords<Scalar, N, El, O>::texCoords() const
{
	return Base::container();
}

/**
 * @brief Checks if the given Element has WedgeTexCoords available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that satisfies
 * the ElementConcept.
 * @return `true` if the element has WedgeTexCoords available, `false`
 * otherwise.
 */
bool isWedgeTexCoordsAvailableOn(const ElementConcept auto& element)
{
	return isComponentAvailableOn<WEDGE_TEX_COORDS>(element);
}

} // namespace vcl::comp
