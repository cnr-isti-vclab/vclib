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

#include <vclib/concepts/mesh/components/wedge_tex_coords.h>
#include <vclib/views/view.h>
#include <vclib/space/tex_coord.h>
#include <vclib/space/vector.h>

#include "bases/container_component.h"

namespace vcl::comp {

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
			WEDGE_TEX_COORDS,
			vcl::TexCoord<Scalar>,
			N,
			short,
			ElementType,
			OPT,
			true>
{
	using Base = ContainerComponent<
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

	using WedgeTexCoordsIterator =
		typename Vector<vcl::TexCoord<Scalar>, N>::Iterator;
	using ConstWedgeTexCoordsIterator =
		typename Vector<vcl::TexCoord<Scalar>, N>::ConstIterator;

	/* Member functions */

	bool isEnabled() const;

	vcl::TexCoord<Scalar>&       wedgeTexCoord(uint i);
	const vcl::TexCoord<Scalar>& wedgeTexCoord(uint i) const;

	vcl::TexCoord<Scalar>&       wedgeTexCoordMod(int i);
	const vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i) const;

	void setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i);
	void setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar>>& list);

	short& textureIndex();
	short textureIndex() const;

	/* Iterator Member functions */

	WedgeTexCoordsIterator      wedgeTexCoordBegin();
	WedgeTexCoordsIterator      wedgeTexCoordEnd();
	ConstWedgeTexCoordsIterator wedgeTexCoordBegin() const;
	ConstWedgeTexCoordsIterator wedgeTexCoordEnd() const;
	auto                        wedgeTexCoords();
	auto                        wedgeTexCoords() const;

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

/* Detector function to check if a class has WedgeTexCoords enabled */

bool isWedgeTexCoordsEnabledOn(const ElementConcept auto& element);

} // namespace vcl::comp

#include "wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
