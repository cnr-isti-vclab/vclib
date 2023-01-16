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
	const KeyType&   sizes) :
		GridType(min, max, sizes)
{
}

template<typename GridType, typename ValueType>
template<typename BoxType>
HashTableGrid<GridType, ValueType>::HashTableGrid(const BoxType& bbox, const KeyType& sizes) :
		GridType(bbox, sizes)
{
}

template<typename GridType, typename ValueType>
bool HashTableGrid<GridType, ValueType>::empty() const
{
	return values.empty();
}

template<typename GridType, typename ValueType>
std::size_t HashTableGrid<GridType, ValueType>::size() const
{
	return values.size();
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
	typename HashTableGrid<GridType, ValueType>::Iterator,
	typename HashTableGrid<GridType, ValueType>::Iterator>
HashTableGrid<GridType, ValueType>::valuesInCell(const KeyType& k) const
{
	auto p = map.equal_range(k);
	return std::make_pair(Iterator(p.first, values), Iterator(p.second, values));
}

template<typename GridType, typename ValueType>
std::vector<typename HashTableGrid<GridType, ValueType>::Iterator>
HashTableGrid<GridType, ValueType>::valuesInSphere(
	const vcl::Sphere<typename GridType::ScalarType>& s) const
{
	// ValueType having removed the pointer, if present
	using VT = typename std::remove_pointer<ValueType>::type;

	std::vector<typename HashTableGrid<GridType, ValueType>::Iterator> resVec;

	// interval of cells containing the sphere
	KeyType first = GridType::cell(s.center() - s.radius());
	KeyType last = GridType::cell(s.center() + s.radius());

	values.unMarkAll();

	for (const KeyType& c : GridType::cells(first, last)) { // for each cell in the intervall
		const auto& p = valuesInCell(c);
		for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
			const VT* vv = nullptr; // vv will point to the current value
			if constexpr(std::is_pointer<ValueType>::value) {
				vv = it->value;
			}
			else {
				vv = &(it->value);
			}

			bool test = false;
			if constexpr(PointConcept<VT>) { // check if the point value is inside the sphere
				test = vv && s.isInside(*vv);
			}
			else if constexpr(VertexConcept<VT>) { // check if the vertex coord is inside the sphere
				test = vv && s.isInside(vv->coord());
			}
			else { // check if the bbox of the value intersects the sphere
				if (!values.isMarked(it.mapIt->second)) {
					test = vv && s.intersects(vcl::boundingBox(*vv));
					values.mark(it.mapIt->second);
				}
			}

			if (test)
				resVec.push_back(it);
		}
	}
	return resVec;
}

template<typename GridType, typename ValueType>
void HashTableGrid<GridType, ValueType>::clear()
{
	map.clear();
}

template<typename GridType, typename ValueType>
bool HashTableGrid<GridType, ValueType>::insert(const ValueType& v)
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

	if (vv) { // vv is a valid pointer (ValueType, or ValueType* if ValueType is not a pointer)
		uint vid = values.size();

		if constexpr (PointConcept<VT>) { // if the ValueType was a Point (or Point*)
			typename GridType::CellCoord cell = GridType::cell(*vv);
			if (insert(cell, v, vid)) {
				values.pushBack(v);
				valuesNumber++;
				return true;
			}
		}
		else if constexpr (VertexConcept<VT>) { // if the ValueType was a Vertex (or Vertex*)
			typename GridType::CellCoord cell = GridType::cell(vv->coord());
			if (insert(cell, v, vid)) {
				values.pushBack(v);
				valuesNumber++;
				return true;
			}
		}
		else { // else, call the boundingBox function
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			typename GridType::CellCoord bmin = GridType::cell(bb.min);
			typename GridType::CellCoord bmax = GridType::cell(bb.max);

			bool ins = false;
			for (const auto& cell : GridType::cells(bmin, bmax)) {
				ins |= insert(cell, v, vid);
			}
			if (ins) {
				values.pushBack(v);
				valuesNumber++;
				return true;
			}
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
			auto p = erase(cell, v);
			if (p.first)
				valuesNumber--;
			return p.first;
		}
		else if constexpr (VertexConcept<VT>) {
			typename GridType::CellCoord cell = GridType::cell(vv->coord());
			auto p = erase(cell, v);
			if (p.first)
				valuesNumber--;
			return p.first;
		}
		else {
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			typename GridType::CellCoord bmin = GridType::cell(bb.min);
			typename GridType::CellCoord bmax = GridType::cell(bb.max);

			auto found = std::make_pair(false, 0);

			for (const auto& cell : GridType::cells(bmin, bmax)) {
				auto p = erase(cell, v);
				if (p.first) {
					assert(!found.first || found.second == p.second);
					found = p;
				}
			}
			if (found.first)
				valuesNumber--;
			return found.first;
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
	std::vector<Iterator> toDel = valuesInSphere(s);
	for (auto& it : toDel)
		map.erase(it.mapIt);
}

template<typename GridType, typename ValueType>
typename HashTableGrid<GridType, ValueType>::Iterator
HashTableGrid<GridType, ValueType>::begin() const
{
	return Iterator(map.begin(), values);
}

template<typename GridType, typename ValueType>
typename HashTableGrid<GridType, ValueType>::Iterator
HashTableGrid<GridType, ValueType>::end() const
{
	return Iterator(map.end(), values);
}

template<typename GridType, typename ValueType>
bool HashTableGrid<GridType, ValueType>::insert(const KeyType &k, const ValueType& v, uint vid)
{
	auto range = map.equal_range(k);
	bool found = false;
	for(iterator ci = range.first; ci != range.second && !found; ++ci) {
		if (values[ci->second] == v) {
			found = true;
		}
	}
	if (!found)
		map.insert(MapValueType(k, vid));
	return !found;
}

template<typename GridType, typename ValueType>
std::pair<bool, uint> HashTableGrid<GridType, ValueType>::erase(
	const KeyType& k,
	const ValueType& v)
{
	std::pair<iterator, iterator> range = map.equal_range(k);
	for(iterator ci = range.first; ci != range.second; ++ci) {
		if (values[ci->second] == v) {
			map.erase(ci);
			return std::make_pair(true, ci->second);
		}
	}
	return std::make_pair(false, 0);
}

} // namespace vcl
