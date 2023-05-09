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

#ifndef VCL_MESH_COMPONENTS_SCALAR_H
#define VCL_MESH_COMPONENTS_SCALAR_H

#include <vclib/concepts/mesh/components/scalar.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The Scalar class
 *
 * @ingroup components
 */
template<typename T, typename ElementType = void, bool optional = false>
class Scalar : public Component<T, ElementType, optional>
{
	using Base = Component<T, ElementType, optional>;
	using ThisType = Scalar<T, ElementType, optional>;

public:
	using ScalarComponent = ThisType; // expose the type to allow access to this component

	using ScalarType = T;

	bool isEnabled() const;
	bool isScalarEnabled() const;

	const ScalarType& scalar() const;
	ScalarType&       scalar();

protected:
	template<typename Element>
	void importFrom(const Element& e);
};

/* Detector function to check if a class has Scalar enabled */

template <typename T>
bool isScalarEnabledOn(const T& element);

template<typename ElementType = void, bool optional = false>
using Scalarf = Scalar<float, ElementType, optional>;

template<typename ElementType = void, bool optional = false>
using Scalard = Scalar<double, ElementType, optional>;

} // namespace vcl::comp

#include "scalar.cpp"

#endif // VCL_MESH_COMPONENTS_SCALAR_H
