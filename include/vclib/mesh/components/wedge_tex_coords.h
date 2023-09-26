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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include <vclib/concepts/mesh/components/tex_coord.h>
#include <vclib/concepts/mesh/components/wedge_tex_coords.h>
#include <vclib/space/tex_coord.h>
#include <vclib/space/vector.h>
#include <vclib/views/view.h>

#include "bases/container_component.h"

namespace vcl::comp {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief The WedgeTexCoords class is a container of texture coordinates
 * associated to the wedges of a Face element.
 *
 * The component is composed of a static or dynamic size container, depending on
 * the value of the template argument N (a negative value indicates a dynamic
 * size), plus a texture index that represents the index of the texture used
 * by all the texture coordinates stored in the container.
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component, altough it is usually used (and it
 * makes sense only) on the Face element.
 *
 * For example, if you have a Face Element `f` that has the WedgeTexCoords
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * auto t = f.wedgeTexCoord(0);
 * short tid = f.textureIndex();
 * @endcode
 *
 * @note This component is *Tied To Vertex Number*: it means that the size of
 * the container, if dynamic, will change automatically along the Vertex Number
 * of the Component. For further details check the documentation of the @ref
 * ContainerComponent class.
 *
 * @tparam Scalar: The Scalar type used for the texture coordinates.
 * @tparam N: The size of the container, that will represent the number of
 * storable wedge texcoords. If N is negative, the container will be dynamic.
 * In any case, N must be the same of the Vertex Number of the Element that
 * will contain this component.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Scalar, int N, typename ElementType = void, bool OPT = false>
class WedgeTexCoords :
		public ContainerComponent<
			WedgeTexCoords<Scalar, N, ElementType, OPT>,
			WEDGE_TEX_COORDS,
			vcl::TexCoord<Scalar>,
			N,
			short,
			ElementType,
			OPT,
			true>
{
	using Base = ContainerComponent<
		WedgeTexCoords<Scalar, N, ElementType, OPT>,
		WEDGE_TEX_COORDS,
		vcl::TexCoord<Scalar>,
		N,
		short,
		ElementType,
		OPT,
		true>;

public:
	static const int WEDGE_TEX_COORD_NUMBER = N;

	/**
	 * @brief Expose the type of the Texture Coordinate.
	 */
	using WedgeTexCoordType = vcl::TexCoord<Scalar>;

	/* Iterator Types declaration */

	using WedgeTexCoordsIterator = Vector<vcl::TexCoord<Scalar>, N>::Iterator;
	using ConstWedgeTexCoordsIterator =
		Vector<vcl::TexCoord<Scalar>, N>::ConstIterator;

	/* Member functions */

	vcl::TexCoord<Scalar>&       wedgeTexCoord(uint i);
	const vcl::TexCoord<Scalar>& wedgeTexCoord(uint i) const;

	vcl::TexCoord<Scalar>&       wedgeTexCoordMod(int i);
	const vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i) const;

	void setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i);

	template<Range Rng>
	void setWedgeTexCoords(Rng&& r)
		requires RangeOfConvertibleTo<Rng, vcl::TexCoord<Scalar>>;

	short& textureIndex();
	short textureIndex() const;

	/* Iterator Member functions */

	WedgeTexCoordsIterator            wedgeTexCoordBegin();
	WedgeTexCoordsIterator            wedgeTexCoordEnd();
	ConstWedgeTexCoordsIterator       wedgeTexCoordBegin() const;
	ConstWedgeTexCoordsIterator       wedgeTexCoordEnd() const;
	View<WedgeTexCoordsIterator>      wedgeTexCoords();
	View<ConstWedgeTexCoordsIterator> wedgeTexCoords() const;

	// dummy member to discriminate between WedgeTexCoords and
	// FaceHalfEdgePointers
	void __wedgeTexCoords() const {}

protected:
	using WedgeTexCoordScalarType = Scalar;

	// Component interface function
	template <typename Element>
	void importFrom(const Element& e);

	// ContainerComponent interface functions
	void resize(uint n) requires (N < 0);
	void pushBack(const vcl::TexCoord<Scalar>& t = vcl::TexCoord<Scalar>())
		requires (N < 0);
	void
	insert(uint i, const vcl::TexCoord<Scalar>& t = vcl::TexCoord<Scalar>())
		requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);

private:
	template<typename Element>
	void importWedgeTexCoordsFrom(const Element& e);

	Vector<vcl::TexCoord<Scalar>, N>& texCoords();
	const Vector<vcl::TexCoord<Scalar>, N>& texCoords() const;
};

/* Detector function to check if a class has WedgeTexCoords available */

bool isWedgeTexCoordsAvailableOn(const ElementConcept auto& element);

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

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

/**
 * @brief Sets all the wedge texcoords of the element.
 *
 * If the size of the container is static, the size of the input range must be
 * the same one of the container.
 *
 * @tparam Rng: The type of the range of wedge texcoords to set. The value type
 * of the range must be convertible to a vcl::TexCoord<Scalar>.
 *
 * @param[in] r: range of texcoords to set.
 */
template<typename Scalar, int N, typename El, bool O>
template<Range Rng>
void WedgeTexCoords<Scalar, N, El, O>::setWedgeTexCoords(Rng&& r)
	requires RangeOfConvertibleTo<Rng, vcl::TexCoord<Scalar>>
{
	texCoords().set(r);
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
auto WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordBegin()
	-> WedgeTexCoordsIterator
{
	return texCoords().begin();
}

/**
 * @brief Returns an iterator to the end of the container of this component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Scalar, int N, typename El, bool O>
auto WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordEnd()
	-> WedgeTexCoordsIterator
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
auto WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordBegin() const
	-> ConstWedgeTexCoordsIterator
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
auto WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordEnd() const
	-> ConstWedgeTexCoordsIterator
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
	-> View<WedgeTexCoordsIterator>
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
	-> View<ConstWedgeTexCoordsIterator>
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

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
