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

#include "hash_table_grid.h"

#include <vclib/algorithms/stat/bounding_box.h>

namespace vcl {

template<typename GridType, typename ValueType>
HashTableGrid<GridType, ValueType>::HashTableGrid()
{
}

template<typename GridType, typename ValueType>
HashTableGrid<GridType, ValueType>::HashTableGrid(const GridType& grid) :
		GridType(grid)
{
}

template<typename GridType, typename ValueType>
template<PointConcept PointType>
HashTableGrid<GridType, ValueType>::HashTableGrid(
	const PointType& min,
	const PointType& max,
	const KeyType&   size) :
		GridType(min, max, size)
{
}

template<typename GridType, typename ValueType>
template<typename BoxType>
HashTableGrid<GridType, ValueType>::HashTableGrid(const BoxType& bbox, const KeyType& size) :
		GridType(bbox, size)
{
}

template<typename GridType, typename ValueType>
bool HashTableGrid<GridType, ValueType>::empty() const
{
	return map.empty();
}

template<typename GridType, typename ValueType>
std::size_t HashTableGrid<GridType, ValueType>::size() const
{
	return map.size();
}

template<typename GridType, typename ValueType>
bool HashTableGrid<GridType, ValueType>::cellEmpty(const KeyType& k) const
{
	return map.find(k) == map.end();
}

template<typename GridType, typename ValueType>
std::set<typename HashTableGrid<GridType, ValueType>::KeyType>
HashTableGrid<GridType, ValueType>::nonEmptyCells() const
{
	std::set<KeyType> keys;
	for (const auto& p : map)
		keys.insert(p.first);
	return keys;
}

template<typename GridType, typename ValueType>
std::size_t HashTableGrid<GridType, ValueType>::countInCell(const KeyType& k) const
{
	return map.count(k);
}

template<typename GridType, typename ValueType>
uint HashTableGrid<GridType, ValueType>::countInSphere(
	const Sphere<typename GridType::ScalarType>& s) const
{
	return valuesInSphere(s).size();
}

template<typename GridType, typename ValueType>
std::pair<
	typename HashTableGrid<GridType, ValueType>::const_iterator,
	typename HashTableGrid<GridType, ValueType>::const_iterator>
HashTableGrid<GridType, ValueType>::valuesInCell(const KeyType& k) const
{
	return map.equal_range(k);
}

template<typename GridType, typename ValueType>
std::vector<typename HashTableGrid<GridType, ValueType>::const_iterator>
HashTableGrid<GridType, ValueType>::valuesInSphere(
	const vcl::Sphere<typename GridType::ScalarType>& s) const
{
	// ValueType having removed the pointer, if present
	using VT = typename std::remove_pointer<ValueType>::type;

	std::vector<typename HashTableGrid<GridType, ValueType>::const_iterator> set;

	// interval of cells containing the sphere
	KeyType first = GridType::cell(s.center() - s.radius());
	KeyType last = GridType::cell(s.center() + s.radius());

	for (const KeyType& c : GridType::cells(first, last)) { // for each cell in the intervall
		const auto& p = valuesInCell(c);
		for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
			const VT* vv = nullptr; // vv will point to the current value
			if constexpr(std::is_pointer<ValueType>::value) {
				vv = it->second;
			}
			else {
				vv = &(it->second);
			}

			bool test = false;
			if constexpr(PointConcept<VT>) { // check if the point value is inside the sphere
				test = vv && s.isInside(*vv);
			}
			else if constexpr(VertexConcept<VT>) { // check if the vertex coord is inside the sphere
				test = vv && s.isInside(vv->coord());
			}
			else { // check if the bbox of the value intersects the sphere
				test = vv && s.intersects(vcl::boundingBox(*vv));
			}

			if (test)
				set.push_back(it);
		}
	}
	return set;
}

template<typename GridType, typename ValueType>
void HashTableGrid<GridType, ValueType>::clear()
{
	map.clear();
}

template<typename GridType, typename ValueType>
void HashTableGrid<GridType, ValueType>::insert(const KeyType &k, const ValueType &v)
{
	map.insert(MapValueType(k, v));
}

template<typename GridType, typename ValueType>
void HashTableGrid<GridType, ValueType>::insert(const ValueType& v)
{
	// ValueType could be anything. We need to understand first if it is a pointer or not,
	// and then insert the value only if it is not nullptr

	// VT is:
	// - ValueType if ValueType is not a pointer
	// - The Type pointed by ValueType, if ValueType is a pointer
	using VT = typename std::remove_pointer<ValueType>::type;
	const VT* vv = nullptr; // vv is a pointer to VT
	if constexpr(std::is_pointer<ValueType>::value) {
		vv = v;
	}
	else {
		vv = &v;
	}

	if (vv) { // vv is a pointer (ValueType or ValueType* if ValueType is not a pointer)
		if constexpr (PointConcept<VT>) { // if the ValueType was a Point (or Point*)
			typename GridType::CellCoord cell = GridType::cell(*vv);
			insert(cell, v);
		}
		else if constexpr (VertexConcept<VT>) { // if the ValueType was a Vertex (or Vertex*)
			typename GridType::CellCoord cell = GridType::cell(vv->coord());
			insert(cell, v);
		}
		else { // else, call the boundingBox function
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			typename GridType::CellCoord bmin = GridType::cell(bb.min);
			typename GridType::CellCoord bmax = GridType::cell(bb.max);

			for (const auto& cell : GridType::cells(bmin, bmax)) {
				insert(cell, v);
			}
		}
	}
}

template<typename GridType, typename ValueType>
bool HashTableGrid<GridType, ValueType>::erase(const KeyType& k, const ValueType& v)
{
	std::pair<iterator, iterator> range = map.equal_range(k);
	for(iterator ci = range.first; ci != range.second; ++ci) {
		if (ci->second == v) {
			map.erase(ci);
			return true;
		}
	}
	return false;
}

template<typename GridType, typename ValueType>
bool HashTableGrid<GridType, ValueType>::erase(const ValueType& v)
{
	using VT = typename std::remove_pointer<ValueType>::type;
	const VT* vv = nullptr;
	if constexpr(std::is_pointer<ValueType>::value) {
		vv = v;
	}
	else {
		vv = &v;
	}

	if (vv) {
		if constexpr (PointConcept<VT>) {
			typename GridType::CellCoord cell = GridType::cell(*vv);
			return erase(cell, v);
		}
		else if constexpr (VertexConcept<VT>) {
			typename GridType::CellCoord cell = GridType::cell(vv->coord());
			return erase(cell, v);
		}
		else {
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			typename GridType::CellCoord bmin = GridType::cell(bb.min);
			typename GridType::CellCoord bmax = GridType::cell(bb.max);

			bool found = false;

			for (const auto& cell : GridType::cells(bmin, bmax)) {
				found |= erase(cell, v);
			}
			return found;
		}
	}
	return false;
}

template<typename GridType, typename ValueType>
bool HashTableGrid<GridType, ValueType>::eraseCell(const KeyType& k)
{
	std::pair<iterator,iterator> range = map.equal_range(k);
	if (range != map.end()) {
		map.erase(range.first, range.second);
		return true;
	}
	return false;
}

template<typename GridType, typename ValueType>
void HashTableGrid<GridType, ValueType>::eraseInSphere(
	const Sphere<typename GridType::ScalarType>& s)
{
	std::vector<iterator> toDel = valuesInSphere(s);
	for (auto& it : toDel)
		map.erase(it);
}

template<typename GridType, typename ValueType>
typename HashTableGrid<GridType, ValueType>::const_iterator
HashTableGrid<GridType, ValueType>::begin() const
{
	return map.begin();
}

template<typename GridType, typename ValueType>
typename HashTableGrid<GridType, ValueType>::const_iterator
HashTableGrid<GridType, ValueType>::end() const
{
	return map.end();
}

} // namespace vcl
