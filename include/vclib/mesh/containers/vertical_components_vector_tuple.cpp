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

#include "vertical_components_vector_tuple.h"

namespace vcl::mesh {

template<typename ...Comp>
VerticalComponentsVectorTuple<Comp...>::VerticalComponentsVectorTuple()
{
	// by default, all the components are enabled
	vecEnabled.fill(true);
}

template<typename ...Comp>
constexpr uint VerticalComponentsVectorTuple<Comp...>::componentsNumber()
{
	return std::tuple_size_v<std::tuple<std::vector<Comp>...>>;
}

template<typename ...Comp>
template<typename C>
constexpr uint VerticalComponentsVectorTuple<Comp...>::indexOfType()
{
	return vcl::indexInTypePack<C, Comp...>();
}

template<typename ...Comp>
template<typename C>
constexpr std::vector<typename C::ValueType>& VerticalComponentsVectorTuple<Comp...>::vector()
{
	constexpr uint ind = indexOfType<C>();
	return std::get<ind>(tuple);
}

template<typename ...Comp>
template<typename C>
constexpr const std::vector<typename C::ValueType>& VerticalComponentsVectorTuple<Comp...>::vector() const
{
	constexpr uint ind = indexOfType<C>();
	return std::get<ind>(tuple);
}

template<typename ...Comp>
std::size_t VerticalComponentsVectorTuple<Comp...>::size() const
{
	return siz;
}

template<typename ...Comp>
void VerticalComponentsVectorTuple<Comp...>::resize(std::size_t size)
{
	vectorResize<componentsNumber()-1>(size);
	siz = size;
}

template<typename ...Comp>
void VerticalComponentsVectorTuple<Comp...>::clear()
{
	auto function =
		[](auto&... args)
	{
		((args.clear()), ...);
	};

	std::apply(function, tuple);
	siz = 0;
}

template<typename ...Comp>
template<typename C>
bool VerticalComponentsVectorTuple<Comp...>::isComponentEnabled() const
{
	constexpr uint ind = indexOfType<C>();
	return vecEnabled[ind];
}

template<typename ...Comp>
template<typename C>
void VerticalComponentsVectorTuple<Comp...>::enableComponent()
{
	constexpr uint ind = indexOfType<C>();
	vecEnabled[ind] = true;
	vector<C>().resize(siz);
}

template<typename ...Comp>
template<typename C>
void VerticalComponentsVectorTuple<Comp...>::disableComponent()
{
	constexpr uint ind = indexOfType<C>();
	vecEnabled[ind] = false;
	vector<C>().clear();
}

template<typename ...Comp>
template<std::size_t N>
void VerticalComponentsVectorTuple<Comp...>::vectorResize(std::size_t size)
{
	if (vecEnabled[N]) {
		std::get<N>(tuple).resize(size);
	}
	if constexpr (N != 0)
		vectorResize<N-1>(size);
}

} // namespace vcl::mesh
