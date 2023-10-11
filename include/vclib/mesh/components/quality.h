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

#ifndef VCL_MESH_COMPONENTS_QUALITY_H
#define VCL_MESH_COMPONENTS_QUALITY_H

#include <vclib/concepts/mesh/components/quality.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The Quality class represents a component that stores the quality of a
 * mesh element, that is represented by a scalar.
 *
 * This scalar is actually used for a lot of different purposes, depending on
 * the applications.
 *
 * For example, if you have a Vertex Element `v` with the Quality component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto q = v.quality();
 * @endcode
 *
 * @tparam Scalar: The scalar type of the Quality value.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Scalar, typename ElementType = void, bool OPT = false>
class Quality :
		public Component<
			Quality<Scalar, ElementType, OPT>,
			QUALITY,
			Scalar,
			ElementType,
			OPT>
{
	using Base = Component<
		Quality<Scalar, ElementType, OPT>,
		QUALITY,
		Scalar,
		ElementType,
		OPT>;

public:
	/**
	 * @brief Exposes the scalar used as Quality type.
	 */
	using QualityType = Scalar;

	/* Constructors */

	/**
	 * @brief Initilizes the Quality value to 0.
	 */
	Quality() = default;

	/* Member functions */

	/**
	 * @brief Returns a const reference of the quality of the element.
	 * @return a const reference of the quality of the element.
	 */
	const QualityType& quality() const { return Base::data(); }

	/**
	 * @brief Returns a reference of the quality of the element.
	 * @return a reference of the quality of the element.
	 */
	QualityType& quality() { return Base::data(); }

protected:
	// Component interface functions
	template<typename Element>
	void importFrom(const Element& e)
	{
		if constexpr (HasQuality<Element>) {
			if (isQualityAvailableOn(e)){
				quality() = e.quality();
			}
		}
	}
};

/* Detector function to check if a class has Quality available */

/**
 * @brief Checks if the given Element has Quality component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element has Quality component available,
 * `false` otherwise.
 */
bool isQualityAvailableOn(const ElementOrMeshConcept auto& element)
{
	return isComponentAvailableOn<QUALITY>(element);
}

/* Specialization Aliases */

/**
 * The Qualityf class is an alias of the Quality component that uses float as
 * scalar type.
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
using Qualityf = Quality<float, ElementType, OPT>;

/**
 * The Qualityd class is an alias of the Quality component that uses double as
 * scalar type.
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
using Qualityd = Quality<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_QUALITY_H
