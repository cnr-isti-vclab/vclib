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

#ifndef VCL_MESH_COMPONENTS_MARK_H
#define VCL_MESH_COMPONENTS_MARK_H

#include <vclib/concepts/mesh/components/mark.h>

#include "bases/component.h"

namespace vcl::comp {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief The Mark class is an utility class useful to un-mark components in
 * constant time.
 *
 * Its implementation is just an integer that can be incremented and
 * decremented.
 *
 * Assuming that two Elements (or a Mesh and an Element) have the Mark
 * component: you can consider one of the elements "marked" if it has the same
 * mark value of the other element/mesh.
 *
 * Example: suppose that you have a Mesh with Vertex Elements, and both Mesh and
 * Vertices have the Mark component. In initialization, all the elements are
 * considered marked, because the elements have the same mark value of the Mesh.
 * To un-mark all the vertices of the mesh:
 *
 * @code{.cpp}
 * m.incrementMark();
 * @endcode
 *
 * Now all the vertices (and all the other elements) are un-marked because they
 * have a different mark value w.r.t. the one of the mesh.
 *
 * Then, if you want to mark the vertices having index 3 and 5:
 *
 * @code{.cpp}
 * m.vertex(3).incrementMark();
 * m.vertex(5).incrementMark();
 * @endcode
 *
 * And to check if vertices 4 and 5 are marked:
 *
 * @code{.cpp}
 * m.hasSameMark(m.vertex(4)); // or: m.vertex(4).hasSameMark(m)
 * m.hasSameMark(m.vertex(5)); // or: m.vertex(5).hasSameMark(m)
 * @endcode
 *
 * @note This component can be both used for Elements and Meshes.
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
class Mark :
		public Component<Mark<ElementType, OPT>, MARK, int, ElementType, OPT>
{
	using Base = Component<Mark<ElementType, OPT>, MARK, int, ElementType, OPT>;

public:
	/* Constructor and isAvailable */

	Mark();

	void init();

	/* Member functions */

	int  mark() const;
	void resetMark();

	template<typename E>
	bool hasSameMark(const E& e) const;

	void incrementMark();
	void decrementMark();

protected:
	// Component interface function
	template<typename Element>
	void importFrom(const Element& e);

private:
	int& mark();
};

/* Detector function to check if a class has Mark available */

bool isMarkAvailableOn(const ElementOrMeshConcept auto& element);

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @private
 * @brief Constructor that initializes the mark to 0.
 */
template<typename El, bool O>
Mark<El, O>::Mark()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

/**
 * @private
 * @brief Initializes the mark to 0.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename El, bool O>
void Mark<El, O>::init()
{
	mark() = 0;
}

/**
 * @brief Returns the value of the mark.
 * @return the value of the mark.
 */
template<typename El, bool O>
int Mark<El, O>::mark() const
{
	return Base::data();
}

/**
 * @brief Resets the mark to 0.
 */
template<typename El, bool O>
void Mark<El, O>::resetMark()
{
	mark() = 0;
}

/**
 * @brief Checks if the current element/mesh has the same mark of the given
 * input element/mesh `e`.
 *
 * @tparam E: the type of the input element/mesh.
 * @param e: the input element/mesh.
 */
template<typename El, bool O>
template<typename E>
bool Mark<El, O>::hasSameMark(const E& e) const
{
	if constexpr (std::is_pointer<E>::value) {
		return e->mark() == mark();
	}
	else {
		return e.mark() == mark();
	}
}

/**
 * @brief Increments the mark of the current element/mesh by 1.
 */
template<typename El, bool O>
void Mark<El, O>::incrementMark()
{
	mark()++;
}

/**
 * @brief Decrements the mark of the current element/mesh by 1.
 */
template<typename El, bool O>
void Mark<El, O>::decrementMark()
{
	mark()--;
}

template<typename El, bool O>
template<typename Element>
void Mark<El, O>::importFrom(const Element& e)
{
	if constexpr (HasMark<Element>) {
		if (isMarkAvailableOn(e)) {
			mark() = e.mark();
		}
	}
}

template<typename El, bool O>
int& Mark<El, O>::mark()
{
	return Base::data();
}

/**
 * @brief Checks if the given Element/Mesh has the Mark component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element/mesh to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element/mesh has the Mark component available, `false`
 * otherwise.
 */
bool isMarkAvailableOn(const ElementOrMeshConcept auto& element)
{
	return isComponentAvailableOn<MARK>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_MARK_H
