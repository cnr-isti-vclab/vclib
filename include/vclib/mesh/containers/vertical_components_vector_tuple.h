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

#ifndef VCL_MESH_CONTAINER_VERTICAL_COMPONENTS_VECTOR_TUPLE_H
#define VCL_MESH_CONTAINER_VERTICAL_COMPONENTS_VECTOR_TUPLE_H

#include <array>
#include <vector>
#include <tuple>

#include <vclib/types.h>
#include <vclib/misc/tuple.h>

namespace vcl::mesh {

template<typename ...Comp>
class VerticalComponentsVectorTuple
{
	using ComponentTypes = std::tuple<Comp...>;
public:
	VerticalComponentsVectorTuple();

	static constexpr uint componentsNumber();

	template<typename C>
	static constexpr uint indexOfType();

	template<typename C>
	constexpr std::vector<typename C::DataValueType>& vector();

	template<typename C>
	constexpr const std::vector<typename C::DataValueType>& vector() const;

	std::size_t size() const;

	void resize(std::size_t size);
	void reserve(std::size_t size);

	void compact(const std::vector<int>& newIndices);

	void clear();

	template<typename C>
	bool isComponentEnabled() const;

	template<typename C>
	void enableComponent();

	template<typename C>
	void disableComponent();

private:
	std::tuple<std::vector<typename Comp::DataValueType>...> tuple;

	std::array<bool, componentsNumber()> vecEnabled;
	std::size_t siz = 0;

	template<std::size_t N>
	void vectorResize(std::size_t size);

	template<std::size_t N>
	void vectorReserve(std::size_t size);

	template<std::size_t N>
	void vectorCompact(const std::vector<int>& newIndices);

	template<typename C, bool E>
	void setComponentEnabled();
};

/** Crucial specialization - allows to catch components that are passed with a TypeWrapper **/
template<typename ...Comp>
class VerticalComponentsVectorTuple<TypeWrapper<Comp...>> :
		public VerticalComponentsVectorTuple<Comp...>
{
};

} // namespace vcl::mesh

#include "vertical_components_vector_tuple.cpp"

#endif // VCL_MESH_CONTAINER_VERTICAL_COMPONENTS_VECTOR_TUPLE_H
