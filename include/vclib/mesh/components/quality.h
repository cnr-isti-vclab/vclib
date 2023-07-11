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
class Quality : public Component<QUALITY, Scalar, ElementType, OPT>
{
	using Base = Component<QUALITY, Scalar, ElementType, OPT>;

public:
	using QualityType = Scalar;

	bool isEnabled() const;

	const QualityType& quality() const;
	QualityType&       quality();

protected:
	// Component interface functions
	template<typename Element>
	void importFrom(const Element& e);
};

/* Detector function to check if a class has Quality enabled */

bool isQualityEnabledOn(const ElementOrMeshConcept auto& element);

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

#include "quality.cpp"

#endif // VCL_MESH_COMPONENTS_QUALITY_H
