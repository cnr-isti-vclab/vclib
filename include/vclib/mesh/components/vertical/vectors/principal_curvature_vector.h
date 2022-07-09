/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_VECTOR_H
#define VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_VECTOR_H

#include "../../concepts/principal_curvature.h"

#include "generic_component_vector.h"

namespace vcl::internal {

template<typename>
class PrincipalCurvatureVector
{
public:
	void clear() {}
	void reserve(uint) {}
	void resize(uint) {}
	void compact(const std::vector<int>&) {}
	bool isPrincipalCurvatureEnabled() const { return false; };
};

template<comp::HasOptionalPrincipalCurvature T>
class PrincipalCurvatureVector<T> :
		private GenericComponentVector<typename T::PrincipalCurvatureType>
{
	using PrincipalCurvatureType = typename T::PrincipalCurvatureType;
	using Base                   = GenericComponentVector<PrincipalCurvatureType>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;

	bool isPrincipalCurvatureEnabled() const { return Base::isEnabled(); };
	void enablePrincipalCurvature(uint size) { Base::enable(size); }
	void disablePrincipalCurvature() { Base::disable(); }

	PrincipalCurvatureType&       principalCurvature(uint i) { return Base::at(i); }
	const PrincipalCurvatureType& principalCurvature(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_VECTOR_H
