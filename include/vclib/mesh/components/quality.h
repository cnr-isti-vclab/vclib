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
 * @brief The Quality class
 *
 * @ingroup components
 */
template<typename T, typename ElementType = void, bool OPT = false>
class Quality : public Component<T, ElementType, OPT>
{
	using Base = Component<T, ElementType, OPT>;
	using ThisType = Quality<T, ElementType, OPT>;

public:
	using QualityComponent = ThisType; // expose the type to allow access to this component

	using QualityType = T;

	bool isEnabled() const;
	bool isQualityEnabled() const;

	const QualityType& quality() const;
	QualityType&       quality();

protected:
	// PointersComponent interface functions
	template<typename Element>
	void importFrom(const Element& e);
};

/* Detector function to check if a class has Quality enabled */

template <typename T>
bool isQualityEnabledOn(const T& element);

template<typename ElementType = void, bool OPT = false>
using Qualityf = Quality<float, ElementType, OPT>;

template<typename ElementType = void, bool OPT = false>
using Qualityd = Quality<double, ElementType, OPT>;

} // namespace vcl::comp

#include "quality.cpp"

#endif // VCL_MESH_COMPONENTS_QUALITY_H
