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

#ifndef VCL_MESH_COMPONENTS_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_WEDGE_COLORS_H

#include <vclib/concepts/mesh/components/wedge_colors.h>
#include <vclib/views/view.h>
#include <vclib/space/vector.h>

#include "bases/container_component.h"

namespace vcl::comp {

/**
 * @brief The WedgeColors class is a container of colors associated to the
 * wedges of a Face element.
 *
 * It is a static or dynamic size container of colors, depending on the value of
 * the template argument N (a negative value indicates a dynamic size).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component, altough it is usually used (and it
 * makes sense only) on the Face element.
 *
 * For example, if you have a Face Element `f` that has the WedgeColors
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * vcl::Color c = f.wedgeColor(0);
 * @endcode
 *
 * @note This component is *Tied To Vertex Number*: it means that the size of
 * the container, if dynamic, will change automatically along the Vertex Number
 * of the Component. For further details check the documentation of the @ref
 * ContainerComponent class.
 *
 * @tparam N: The size of the container, that will represent the number of
 * storable wedge colors. If N is negative, the container will be dynamic.
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
template<int N, typename ElementType = void, bool OPT = false>
class WedgeColors :
		public ContainerComponent<
			WEDGE_COLORS,
			vcl::Color,
			N,
			void,
			ElementType,
			OPT,
			true>
{
	using Base = ContainerComponent<
		WEDGE_COLORS,
		vcl::Color,
		N,
		void,
		ElementType,
		OPT,
		true>;

public:
	static const int WEDGE_COLOR_NUMBER = Base::SIZE;

	/* Iterator Types declaration */

	using WedgeColorsIterator      = typename Base::Iterator;
	using ConstWedgeColorsIterator = typename Base::ConstIterator;

	bool isAvailable() const;

	/* Member functions */

	vcl::Color&       wedgeColor(uint i);
	const vcl::Color& wedgeColor(uint i) const;

	vcl::Color&       wedgeColorMod(int i);
	const vcl::Color& wedgeColorMod(int i) const;

	void setWedgeColor(const vcl::Color& c, uint i);
	void setWedgeColors(const std::vector<vcl::Color>& list);

	/* Iterator Member functions */

	WedgeColorsIterator            wedgeColorBegin();
	WedgeColorsIterator            wedgeColorEnd();
	ConstWedgeColorsIterator       wedgeColorBegin() const;
	ConstWedgeColorsIterator       wedgeColorEnd() const;
	View<WedgeColorsIterator>      wedgeColors();
	View<ConstWedgeColorsIterator> wedgeColors() const;

	// dummy member to discriminate between WedgeColors and FaceHalfEdgePointers
	void __wedgeColors() const {}

protected:
	// Component interface function
	template <typename Element>
	void importFrom(const Element& e);

	// ContainerComponent interface functions
	void resize(uint n) requires (N < 0);
	void pushBack(const vcl::Color& c = vcl::Color()) requires (N < 0);
	void insert(uint i, const vcl::Color& c = vcl::Color()) requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);

private:
	template<typename Element>
	void importWedgeColorsFrom(const Element& e);

	Vector<vcl::Color, N>& colors();
	const Vector<vcl::Color, N>& colors() const;
};

/* Detector function to check if a class has WedgeColors available */

bool isWedgeColorsAvailableOn(const ElementConcept auto& element);

} // namespace vcl::comp

#include "wedge_colors.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_COLORS_H
