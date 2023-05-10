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
 * @brief The WedgeTexCoords class
 *
 * @note This component is *Tied To Vertex Number*: it means that the size of the container,
 * if dynamic, will change automatically along the Vertex Number of the Component.
 * For further details check the documentation of the @ref ContainerComponent class.
 *
 * @ingroup components
 */
template<typename Scalar, int N, typename ElementType = void, bool optional = false>
class WedgeTexCoords :
		public ContainerComponent<vcl::TexCoord<Scalar>, N, short, ElementType, optional, true>
{
	using Base = ContainerComponent<vcl::TexCoord<Scalar>, N, short, ElementType, optional, true>;
	using ThisType = WedgeTexCoords<Scalar, N, ElementType, optional>;

public:
	using WedgeTexCoordsComponent = ThisType; // expose the type to allow access to this component

	static const int WEDGE_TEX_COORD_NUMBER = N;

	using WedgeTexCoordType = vcl::TexCoord<Scalar>;

	/* Iterator Types declaration */

	using WedgeTexCoordsIterator      = typename Vector<vcl::TexCoord<Scalar>, N>::Iterator;
	using ConstWedgeTexCoordsIterator = typename Vector<vcl::TexCoord<Scalar>, N>::ConstIterator;

	/* Member functions */

	bool isEnabled() const;
	bool isWedgeTexCoordsEnabled() const;

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

protected:
	using WedgeTexCoordScalarType = Scalar;

	// Component interface function
	template <typename Element>
	void importFrom(const Element& e);

	// ContainerComponent interface functions
	void resize(uint n) requires (N < 0);
	void pushBack(const vcl::TexCoord<Scalar>& t = vcl::TexCoord<Scalar>()) requires (N < 0);
	void insert(uint i, const vcl::TexCoord<Scalar>& t = vcl::TexCoord<Scalar>()) requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);

private:
	template<typename Element>
	void importWedgeTexCoordsFrom(const Element& e);

	short& texIndex();
	short texIndex() const;
	Vector<vcl::TexCoord<Scalar>, N>& texCoords();
	const Vector<vcl::TexCoord<Scalar>, N>& texCoords() const;
};

/* Detector function to check if a class has WedgeTexCoords enabled */

template <typename T>
bool isWedgeTexCoordsEnabledOn(const T& element);

} // namespace vcl::comp

#include "wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
