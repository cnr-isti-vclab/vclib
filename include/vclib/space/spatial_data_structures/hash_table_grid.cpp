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

/**
 * @brief Empty constructor, creates an usable HashTableGrid, since the Grid on which construct
 * the has table is not initialized.
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid()
{
}

/**
 * @brief Creates a HashTableGrid that allows to store ValueType values on the given grid.
 * @param grid
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid(const GridType& grid) :
		GridType(grid)
{
}

/**
 * @brief Creates a HashTableGrid that allows to store ValueType values on a Grid having `min` as
 * minimum coordinte of the Grid, `max` as maximum coordinate of the grid, and the number of cells
 * per dimension given by `sizes`.
 * @param min
 * @param max
 * @param sizes
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
template<PointConcept PointType>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid(
	const PointType& min,
	const PointType& max,
	const KeyType&   sizes) :
		GridType(min, max, sizes)
{
}

/**
 * @brief Creates a HashTableGrid that allows to store ValueType values on a Grid bounded by `bbox`,
 * and having the number of cells per dimension given by `sizes`.
 * @param bbox
 * @param sizes
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
template<typename BoxType>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid(
	const BoxType& bbox,
	const KeyType& sizes) :
		GridType(bbox, sizes)
{
}

/**
 * @brief Creates an HashTableGrid that contains all the elements that can be iterated from `begin`
 * to `end`.
 *
 * The bounding box and the sizes of the Grid are automatically computed.
 * Bounding box is computed starting from the bounding box of all the iterated elements, and then
 * inflated. The number of cells per dimension is computed using the `vcl::bestGridSize` function.
 *
 * @param begin
 * @param end
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
template<typename ObjIterator>
HashTableGrid<GridType, ValueType, AllowDuplicates>::HashTableGrid(ObjIterator begin, ObjIterator end)
{
	using ScalarType = typename GridType::ScalarType;
	using BBoxType = typename GridType::BBoxType;
	using CellCoord = typename GridType::CellCoord;

	BBoxType bbox = boundingBox(begin, end);
	uint nElements = std::distance(begin, end);

	if (nElements > 0) {
		// inflated bb
		ScalarType infl = bbox.diagonal()/nElements;
		bbox.min -= infl;
		bbox.max += infl;

		CellCoord sizes = bestGridSize(bbox.size(), nElements);

		GridType::set(bbox, sizes);

		insert(begin, end);
	}
}

/**
 * @brief Returns true if the HashTableGrid is empty (no elements in it).
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::empty() const
{
	return map.empty();
}

/**
 * @brief Returns true if the given cell coordinate does not contain elements in it.
 * @param k
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::cellEmpty(const KeyType& k) const
{
	return map.find(k) == map.end();
}

/**
 * @brief Returns an std::set containing the cell coordinates of all the cells that contain at least
 * one element.
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
std::set<typename HashTableGrid<GridType, ValueType, AllowDuplicates>::KeyType>
HashTableGrid<GridType, ValueType, AllowDuplicates>::nonEmptyCells() const
{
	std::set<KeyType> keys;
	for (const auto& p : map)
		keys.insert(p.first);
	return keys;
}

/**
 * @brief Returns the number of elements contained in the given cell.
 * @param k
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
std::size_t HashTableGrid<GridType, ValueType, AllowDuplicates>::countInCell(const KeyType& k) const
{
	return map.count(k);
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
uint HashTableGrid<GridType, ValueType, AllowDuplicates>::countInSphere(
	const Sphere<typename GridType::ScalarType>& s) const
{
	return valuesInSphere(s).size();
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
std::pair<
	typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator,
	typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator>
HashTableGrid<GridType, ValueType, AllowDuplicates>::valuesInCell(const KeyType& k) const
{
	auto p = map.equal_range(k);
	return std::make_pair(ConstIterator(p.first), ConstIterator(p.second));
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
std::vector<typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator>
HashTableGrid<GridType, ValueType, AllowDuplicates>::valuesInSphere(
	const vcl::Sphere<typename GridType::ScalarType>& s) const
{
	// ValueType having removed the pointer, if present
	using VT = typename std::remove_pointer<ValueType>::type;

	std::vector<typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator> resVec;

	// interval of cells containing the sphere
	KeyType first = GridType::cell(s.center() - s.radius());
	KeyType last = GridType::cell(s.center() + s.radius());

	unMarkAll();

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
				if (!isMarked(it.mapIt)) {
					test = vv && s.intersects(vcl::boundingBox(*vv));
					mark(it.mapIt);
				}
			}

			if (test)
				resVec.push_back(it);
		}
	}
	return resVec;
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
void HashTableGrid<GridType, ValueType, AllowDuplicates>::clear()
{
	map.clear();
}

/**
 * @brief Inserts the given element in the HashTableGrid.
 *
 * If the ValueType is Puntual (a Point or a Vertex), the element will be inserted in just one
 * cell of the grid. If the element is a spatial object having a bounding box with min != max, the
 * element will be stored in all the cells where its bounding box lies.
 *
 * @param v
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::insert(const ValueType& v)
{
	// ValueType could be anything. We need to understand first if it is a pointer or not,
	// and then insert the value only if it is not nullptr

	// VT is:
	// - ValueType if ValueType is not a pointer
	// - The Type pointed by ValueType, if ValueType is a pointer
	using TMPVT = typename std::remove_pointer<ValueType>::type;
	using VT = typename std::remove_reference<TMPVT>::type;
	const VT* vv = nullptr; // vv is a pointer to VT
	if constexpr(std::is_pointer<ValueType>::value) {
		vv = v;
	}
	else {
		vv = &v;
	}

	if (vv) { // if vv is a valid pointer (ValueType, or ValueType* if ValueType is not a pointer)
		if constexpr (PointConcept<VT>) { // if the ValueType was a Point (or Point*)
			typename GridType::CellCoord cell = GridType::cell(*vv);
			return insert(cell, v);
		}
		else if constexpr (VertexConcept<VT>) { // if the ValueType was a Vertex (or Vertex*)
			typename GridType::CellCoord cell = GridType::cell(vv->coord());
			return insert(cell, v);
		}
		else { // else, call the boundingBox function
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			typename GridType::CellCoord bmin = GridType::cell(bb.min);
			typename GridType::CellCoord bmax = GridType::cell(bb.max);

			bool ins = false;
			for (const auto& cell : GridType::cells(bmin, bmax)) {
				ins |= insert(cell, v);
			}
			return ins;
		}
	}
	return false;
}

/**
 * @brief Inserts all the elements from `begin` to `end`. The type referenced by the iterator must
 * be the ValueType of the HashTableGrid.
 * @param begin
 * @param end
 * @return
 */
template<typename GridType, typename ValueType, bool AllowDuplicates>
template<typename ObjIterator>
uint HashTableGrid<GridType, ValueType, AllowDuplicates>::insert(ObjIterator begin, ObjIterator end)
{
	uint cnt = 0;
	for (ObjIterator it = begin; it != end; ++it)
		if (insert(*it))
			cnt++;
	return cnt;
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::erase(const ValueType& v)
{
	using TMPVT = typename std::remove_pointer<ValueType>::type;
	using VT = typename std::remove_reference<TMPVT>::type;
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

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::eraseCell(const KeyType& k)
{
	std::pair<MapIterator,MapIterator> range = map.equal_range(k);
	if (range != map.end()) {
		map.erase(range.first, range.second);
		return true;
	}
	return false;
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
void HashTableGrid<GridType, ValueType, AllowDuplicates>::eraseInSphere(
	const Sphere<typename GridType::ScalarType>& s)
{
	std::vector<ConstIterator> toDel = valuesInSphere(s);
	for (auto& it : toDel)
		map.erase(it.mapIt);
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::Iterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::begin()
{
	return Iterator(map.begin());
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::begin() const
{
	return ConstIterator(map.begin());
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::Iterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::end()
{
	return Iterator(map.end());
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
typename HashTableGrid<GridType, ValueType, AllowDuplicates>::ConstIterator
HashTableGrid<GridType, ValueType, AllowDuplicates>::end() const
{
	return ConstIterator(map.end());
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::insert(
	const KeyType&   k,
	const ValueType& v)
{
	if constexpr (AllowDuplicates) {
		map.insert(MapValueType(k, Markable<ValueType>(v)));
		return true;
	}
	else {
		auto range = map.equal_range(k);
		bool found = false;
		for(MapIterator ci = range.first; ci != range.second && !found; ++ci) {
			if (ci->second.get() == v) {
				found = true;
			}
		}
		if (!found)
			map.insert(MapValueType(k, Markable<ValueType>(v)));
		return !found;
	}
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::erase(
	const KeyType& k,
	const ValueType& v)
{
	bool found = false;

	std::pair<MapIterator, MapIterator> range = map.equal_range(k);
	for(MapIterator ci = range.first; ci != range.second; ++ci) {
		if (ci->second.get() == v) {
			found = true;
			map.erase(ci);
			if constexpr (!AllowDuplicates) {
				return true;
			}
		}
	}
	return found;
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
bool HashTableGrid<GridType, ValueType, AllowDuplicates>::isMarked(MapIterator it) const
{
	return it->second.mark() == m;
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
void HashTableGrid<GridType, ValueType, AllowDuplicates>::mark(MapIterator it) const
{
	it->second.mark() = m;
}

template<typename GridType, typename ValueType, bool AllowDuplicates>
void HashTableGrid<GridType, ValueType, AllowDuplicates>::unMarkAll() const
{
	m++;
}

} // namespace vcl
