/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_INFO_H
#define VCL_MESH_COMPONENTS_OPTIONAL_INFO_H

#include <type_traits>

namespace vcl::mesh {

template<typename T>
class ElementContainer;

}

namespace vcl::internal {

template <typename T>
class ComponentsVector;

}

namespace vcl::comp {

template<typename T>
class OptionalInfo
{
public:
	OptionalInfo() {}

	// copy and move semantics: nothing will be done here
	// this instance will be updated only by the mesh, and optional data
	// can be accessed only when an element is part of a mesh
	OptionalInfo(const OptionalInfo<T>&) {}
	OptionalInfo(OptionalInfo<T>&&) {}
	OptionalInfo& operator=(const OptionalInfo<T>&) {return *this;}

protected:
	void setContainerPointer(vcl::mesh::ElementContainer<T>* cp) { contPtr = cp; }

	vcl::internal::ComponentsVector<T>& optCont()
	{
		assert(contPtr != nullptr);
		return contPtr->optionalVec;
	}

	const vcl::internal::ComponentsVector<T>& optCont() const
	{
		assert(contPtr != nullptr);
		return contPtr->optionalVec;
	}

	uint index(const T* e) const
	{
		assert(contPtr != nullptr);
		assert(!contPtr->vec.empty() && e >= contPtr->vec.data() && e <= &contPtr->vec.back());
		return e - contPtr->vec.data();
	}

	vcl::mesh::ElementContainer<T>* contPtr = nullptr;
};

/**
 * Detector to check if a class has (inherits) OptionalInfo
 */

template<typename T>
using hasOptionalInfoT = std::is_base_of<OptionalInfo<T>, T>;

template<typename T>
bool constexpr hasOptionalInfo()
{
	return hasOptionalInfoT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_OPTIONAL_INFO_H
