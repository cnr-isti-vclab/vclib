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

#include "abstract_grid.h"

#include <vclib/algorithms/distance/functions.h>

namespace vcl {

template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractGrid<GridType, ValueType, DerivedGrid>::cellEmpty(const KeyType& k) const
{
	auto p = static_cast<const DerivedGrid*>(this)->valuesInCell(k);
	return p.first == p.second;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
std::size_t AbstractGrid<GridType, ValueType, DerivedGrid>::countInCell(const KeyType& k) const
{
	auto p = static_cast<const DerivedGrid*>(this)->valuesInCell(k);
	return std::distance(p.first, p.second);
}

/**
 * @brief Inserts the given element in the AbstractGrid.
 *
 * If the ValueType is Puntual (a Point or a Vertex), the element will be inserted in just one
 * cell of the grid. If the element is a spatial object having a bounding box with min != max, the
 * element will be stored in all the cells where its bounding box lies.
 *
 * @param v
 * @return
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractGrid<GridType, ValueType, DerivedGrid>::insert(const ValueType& v)
{
	const VT* vv = getCleanValueTypePointer(v);

	if (vv) { // if vv is a valid pointer (ValueType, or ValueType* if ValueType is not a pointer)

		KeyType bmin, bmax; // first and last cell where insert (could be the same)

		// if ValueType is Point, Point*, Vertex, Vertex*
		if constexpr(PointConcept<VT> || VertexConcept<VT>) {
			typename GridType::PointType p;
			if constexpr(PointConcept<VT>)
				p = *vv;
			else
				p = vv->coord();
			bmin = bmax = GridType::cell(p);
		}
		else { // else, call the boundingBox function
			typename GridType::BBoxType bb = vcl::boundingBox(*vv); //bbox of value

			bmin = GridType::cell(bb.min()); // first cell where insert
			bmax = GridType::cell(bb.max()); // last cell where insert
		}

		bool ins = false;

		if (intersects) { // custom intersection function between cell and value
			for (const auto& cell : GridType::cells(bmin, bmax)) {
				if (intersects(GridType::cellBox(cell), v)) {
					ins |= static_cast<DerivedGrid*>(this)->insertInCell(cell, v);
				}
			}
		}
		else {
			for (const auto& cell : GridType::cells(bmin, bmax)) {
				ins |= static_cast<DerivedGrid*>(this)->insertInCell(cell, v);
			}
		}
		return ins;
	}
	return false;
}

/**
 * @brief Inserts all the elements from `begin` to `end`. The type referenced by the iterator must
 * be the ValueType of the AbstractGrid.
 * @param begin
 * @param end
 * @return The number of inserted elements.
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename ObjIterator>
uint AbstractGrid<GridType, ValueType, DerivedGrid>::insert(ObjIterator begin, ObjIterator end)
{
	uint cnt = 0;
	for (ObjIterator it = begin; it != end; ++it)
		if (insert(*it))
			cnt++;
	return cnt;
}

/**
 * @brief Inserts all the elements contained in the input range r, from `begin` to `end`. The type
 * referenced by the iterator must be the ValueType of the AbstractGrid.
 * @param r: a range that satisfies the concept std::ranges::range
 * @return The number of inserted elements.
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<vcl::Range Rng>
uint AbstractGrid<GridType, ValueType, DerivedGrid>::insert(Rng&& r)
{
	return insert(std::ranges::begin(r), std::ranges::end(r));
}

template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractGrid<GridType, ValueType, DerivedGrid>::erase(const ValueType& v)
{
	const VT* vv = getCleanValueTypePointer(v);

	if (vv) {
		KeyType bmin, bmax; // first and last cell where erase (could be the same)

		if constexpr (PointConcept<VT> || VertexConcept<VT>) {
			typename GridType::PointType p;
			if constexpr(PointConcept<VT>)
				p = *vv;
			else
				p = vv->coord();
			bmin = bmax = GridType::cell(p);
		}
		else {
			typename GridType::BBoxType bb = vcl::boundingBox(*vv);

			bmin = GridType::cell(bb.min);
			bmax = GridType::cell(bb.max);
		}

		bool found = false;
		for (const auto& cell : GridType::cells(bmin, bmax)) {
			found |= static_cast<DerivedGrid*>(this)->eraseInCell(cell, v);
		}
		return found;
	}
	return false;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
bool AbstractGrid<GridType, ValueType, DerivedGrid>::eraseAllInCell(const KeyType& k)
{
	bool res = false;
	auto& p = static_cast<DerivedGrid*>(this)->valuesInCell(k);
	for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
		res |= static_cast<DerivedGrid*>(this)->eraseInCell(k, it->second);
	}
	return res;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
uint AbstractGrid<GridType, ValueType, DerivedGrid>::countInSphere(
	const Sphere<typename GridType::ScalarType>& s) const
{
	return valuesInSphere(s).size();
}

template<typename GridType, typename ValueType, typename DerivedGrid>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::valuesInSphere(
	const Sphere<typename GridType::ScalarType>& s) const
{
	using Iter = typename DerivedGrid::ConstIterator;
	using IterSet = std::set<Iter, IterComparator<Iter>>;

	// will use this set only if the value type is not a point -- that is when the value can occupy
	// more than one single cell. We therefore push the results in a set in order to avoid
	// possible duplicates
	IterSet valuesSet;

	std::vector<Iter> resVec;

	// interval of cells containing the sphere
	KeyType first = GridType::cell(s.center() - s.radius());
	KeyType last = GridType::cell(s.center() + s.radius());

	for (const KeyType& c : GridType::cells(first, last)) { // for each cell in the interval
		// p is a pair of iterators
		const auto& p = static_cast<const DerivedGrid*>(this)->valuesInCell(c);
		for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
			if (valueIsInSpehere(it, s)) {
				if constexpr (!PointConcept<VT> && !VertexConcept<VT>) {
					valuesSet.insert(it);
				}
				else {
					resVec.push_back(it);
				}
			}
		}
	}

	// if the valuetype is a point (or vertex), we already pushed in resVec - faster than using set
	if constexpr (!PointConcept<VT> && !VertexConcept<VT>) {
		resVec = std::vector<Iter>(valuesSet.begin(), valuesSet.end());
	}
	return resVec;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
void AbstractGrid<GridType, ValueType, DerivedGrid>::eraseInSphere(
	const Sphere<typename GridType::ScalarType>& s)
{
	// interval of cells containing the sphere
	KeyType first = GridType::cell(s.center() - s.radius());
	KeyType last = GridType::cell(s.center() + s.radius());

	for (const KeyType& c : GridType::cells(first, last)) { // for each cell in the interval
		// p is a pair of iterators
		const auto& p = static_cast<const DerivedGrid*>(this)->valuesInCell(c);
		for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
			if (valueIsInSpehere(it, s)) {
				eraseInCell(it->first, it->second);
			}
		}
	}
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::closestValue(
	const QueryValueType& qv,
	QueryBoundedDistFunction<QueryValueType> distFunction,
	typename GridType::ScalarType& dist) const
{
	using ScalarType = typename GridType::ScalarType;
	using PointType = typename GridType::PointType;
	using ResType = typename DerivedGrid::ConstIterator;

	using QVT = RemoveRefAndPointer<QueryValueType>;
	const QVT* qvv = getCleanValueTypePointer(qv);
	ResType result = static_cast<const DerivedGrid*>(this)->end();

	if (qvv) {
		typename GridType::ScalarType maxDist = dist;

		const ScalarType cellDiag = GridType::cellDiagonal();

		ScalarType centerDist = cellDiag;
		PointType center = vcl::boundingBox(*qvv).center();

		// we first look just on the cells where the query value lies
		// here, we will store also the looking interval where we need to look
		Boxui currentIntervalBox;

		typename GridType::BBoxType bb = vcl::boundingBox(*qvv); //bbox of query value
		currentIntervalBox.add(GridType::cell(bb.min())); // first cell where look for closest
		currentIntervalBox.add(GridType::cell(bb.max())); // last cell where look for closest

		// looking just on cells where query lies
		ScalarType tmp = cellDiag;
		result = closestInCells(qv, tmp, currentIntervalBox, distFunction);

		// we have found (maybe) the closest value contained in the cell(s) where the query value
		// lies (if the cells were empty, we did not found nothing).
		// we now have to expand the search on adjacent cells starting from the center of the
		// query value and expanding until we find a value

		// now we start with the actual search, including all the cells in the actual interval

		if (result != static_cast<const DerivedGrid*>(this)->end()) {
			dist = tmp;
			centerDist = dist;
		}

		bool end = false;

		do {
			Boxui lastIntervalBox = currentIntervalBox;
			currentIntervalBox.add(GridType::cell(center - centerDist));
			currentIntervalBox.add(GridType::cell(center + centerDist));

			ScalarType tmp = centerDist;
			ResType winner =
				closestInCells(qv, tmp, currentIntervalBox, distFunction, lastIntervalBox);

			if (winner != static_cast<const DerivedGrid*>(this)->end()) {
				result = winner;
				dist = tmp;
			}

			centerDist += cellDiag;
			end = result != static_cast<const DerivedGrid*>(this)->end();
			end |= (centerDist > maxDist);
			end |= (center - centerDist < GridType::min() && center + centerDist > GridType::max());
		} while (!end);
	}

	return result;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::closestValue(
	const QueryValueType& qv,
	QueryDistFunction<QueryValueType> distFunction,
	typename GridType::ScalarType& dist) const
{
	QueryBoundedDistFunction<QueryValueType> boundDistFun =
		[&](const QueryValueType& q, const ValueType& v, typename GridType::ScalarType)
	{
		return distFunction(q, v);
	};

	dist = std::numeric_limits<typename GridType::ScalarType>::max();

	return closestValue(qv, boundDistFun, dist);
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::closestValue(
	const QueryValueType& qv,
	QueryDistFunction<QueryValueType> distFunction) const
{
	typename GridType::ScalarType dist = std::numeric_limits<typename GridType::ScalarType>::max();
	return closestValue(qv, distFunction, dist);
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::closestValue(
	const QueryValueType& qv,
	typename GridType::ScalarType& dist) const
{
	std::function f = boundedDistFunction<QueryValueType, ValueType, typename GridType::ScalarType>();
	return closestValue(qv, f, dist);
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::closestValue(const QueryValueType& qv) const
{
	std::function f = boundedDistFunction<QueryValueType, ValueType, typename GridType::ScalarType>();
	typename GridType::ScalarType dist = std::numeric_limits<typename GridType::ScalarType>::max();
	return closestValue(qv, f, dist);
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::kClosestValues(
	const QueryValueType&             qv,
	uint                              n,
	QueryDistFunction<QueryValueType> distFunction) const
{
	using ResType = std::vector<typename DerivedGrid::ConstIterator>;
	// KClosest Types
	using KClosestPairType =
		std::pair<typename GridType::ScalarType, typename DerivedGrid::ConstIterator>;
	using KClosestSet = std::set<KClosestPairType, DistIterPairComparator<KClosestPairType>>;

	Boxui ignore; // will contain the interval of cells already visited

	KClosestSet set = valuesInCellNeighborhood(qv, n, distFunction, ignore);

	auto it = set.size() >= n ? std::next(set.begin(), n) : set.end();
	// if we didn't found n values, it means that there aren't n values in the grid - nothing to do
	if (it != set.end()) {
		using QVT = RemoveRefAndPointer<QueryValueType>;
		const QVT* qvv = getCleanValueTypePointer(qv);

		typename GridType::BBoxType bb = vcl::boundingBox(*qvv); //bbox of query value
		// we need to be sure that there are no values that are closest w.r.t. the n-th that we
		// have already found by looking in the cell neighborhood
		// we extend the bb with the distance of the n-th closest found value
		bb.min() -= it->first;
		bb.max() += it->first;

		// and we look in all of these cells
		Boxui currentIntervalBox;
		currentIntervalBox.add(GridType::cell(bb.min())); // first cell where look for closest
		currentIntervalBox.add(GridType::cell(bb.max())); // last cell where look for closest
		for (const KeyType& c : GridType::cells(currentIntervalBox.min(), currentIntervalBox.max())) {
			if (!ignore.isInsideOpenBox(c)) {
				const auto& p = static_cast<const DerivedGrid*>(this)->valuesInCell(c);
				for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
					auto tmp = distFunction(qv, it->second);
					set.insert(std::make_pair(tmp, it));
				}
			}
		}
	}

	ResType vec;
	// if there are more than n values in the set, we will return n values, otherwise set.size()
	uint retNValues = std::min((uint)set.size(), n);
	vec.reserve(retNValues);
	it = set.begin();
	for (uint i = 0; i < retNValues; i++) {
		vec.push_back(it->second);
		it++;
	}
	return vec;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::kClosestValues(
	const QueryValueType& qv,
	uint                  n) const
{
	std::function f = distFunction<QueryValueType, ValueType>();
	return kClosestValues(qv, n, f);
}

/**
 * @brief Empty constructor, creates an usable AbstractGrid, since the Grid is not initialized.
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
AbstractGrid<GridType, ValueType, DerivedGrid>::AbstractGrid()
{
}

/**
 * @brief Creates a AbstractGrid that allows to store ValueType values on the given grid.
 * @param grid
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
AbstractGrid<GridType, ValueType, DerivedGrid>::AbstractGrid(
	const GridType&  grid,
	IsInCellFunction intersects) :
		GridType(grid), intersects(intersects)
{
}

/**
 * @brief Creates a AbstractGrid that allows to store ValueType values on a Grid having `min` as
 * minimum coordinte of the Grid, `max` as maximum coordinate of the grid, and the number of cells
 * per dimension given by `sizes`.
 * @param min
 * @param max
 * @param sizes
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<PointConcept PointType>
AbstractGrid<GridType, ValueType, DerivedGrid>::AbstractGrid(
	const PointType& min,
	const PointType& max,
	const KeyType&   sizes,
	IsInCellFunction intersects) :
		GridType(min, max, sizes), intersects(intersects)
{
}

/**
 * @brief Creates a AbstractGrid that allows to store ValueType values on a Grid bounded by
 * `bbox`, and having the number of cells per dimension given by `sizes`.
 * @param bbox
 * @param sizes
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename BoxType>
AbstractGrid<GridType, ValueType, DerivedGrid>::AbstractGrid(
	const BoxType& bbox,
	const KeyType& sizes,
	IsInCellFunction intersects) :
		GridType(bbox, sizes), intersects(intersects)
{
}

/**
 * @brief Creates an AbstractGrid having a proper Grid to store the elements.
 *
 * The bounding box and the sizes of the Grid are automatically computed.
 * Bounding box is computed starting from the bounding box of all the iterated elements, and then
 * inflated. The number of cells per dimension is computed using the `vcl::bestGridSize` function.
 *
 * @note This constructor **does not insert the elements** in the Grid Data Structure.
 * Pure virtual classes cannot call pure virtual member functions from constuctors. It is duty of
 * each derived class to insert the elements after calling this constructor.
 *
 * @param begin
 * @param end
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename ObjIterator>
AbstractGrid<GridType, ValueType, DerivedGrid>::AbstractGrid(
	ObjIterator      begin,
	ObjIterator      end,
	IsInCellFunction intersects) :
		intersects(intersects)
{
	using ScalarType = typename GridType::ScalarType;
	using BBoxType = typename GridType::BBoxType;
	using CellCoord = typename GridType::CellCoord;

	BBoxType bbox = boundingBox(begin, end);
	uint nElements = std::distance(begin, end);

	if (nElements > 0) {
		// inflated bb
		ScalarType infl = bbox.diagonal() / nElements;
		bbox.min() -= infl;
		bbox.max() += infl;

		CellCoord sizes = bestGridSize(bbox.size(), nElements);

		GridType::set(bbox, sizes);
	}
}

/**
 * @brief Creates an AbstractGrid having a proper Grid to store the elements.
 *
 * The bounding box and the sizes of the Grid are automatically computed.
 * Bounding box is computed starting from the bounding box of all the iterated elements, and then
 * inflated. The number of cells per dimension is computed using the `vcl::bestGridSize` function.
 *
 * @note This constructor **does not insert the elements** in the Grid Data Structure.
 * Pure virtual classes cannot call pure virtual member functions from constuctors. It is duty of
 * each derived class to insert the elements after calling this constructor.
 *
 * @param r: a range that satisfies the concept std::ranges::range
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<vcl::Range Rng>
AbstractGrid<GridType, ValueType, DerivedGrid>::AbstractGrid(
	Rng&& r,
	IsInCellFunction intersects) :
		AbstractGrid(std::ranges::begin(r), std::ranges::end(r), intersects)
{
}

/**
 * This function is meant to be called by another function of the AbstractGrid.
 * Check if the current iterated value is inside a sphere
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename Iterator>
bool AbstractGrid<GridType, ValueType, DerivedGrid>::valueIsInSpehere(
	const Iterator& it,
	const Sphere<typename GridType::ScalarType>& s) const
{
	const VT* vv = getCleanValueTypePointer(it->second);

	bool test = false;
	if constexpr (PointConcept<VT> || VertexConcept<VT>) {
		typename GridType::PointType p;
		if constexpr(PointConcept<VT>)
			p = *vv;
		else
			p = vv->coord();
		test = vv && s.isInside(p);
	}
	else { // check if the bbox of the value intersects the sphere
		test = vv && s.intersects(vcl::boundingBox(*vv));
	}
	return test;
}

/**
 * This function is meant to be called by another function of the AbstractGrid.
 * Returns the closest Value (if any) to the given query value contained in the given interval of
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::closestInCells(
	const QueryValueType&                    qv,
	typename GridType::ScalarType&           dist,
	const Boxui&                             interval,
	QueryBoundedDistFunction<QueryValueType> distFunction,
	const Boxui&                             ignore) const
{
	using ResType = typename DerivedGrid::ConstIterator;
	ResType res = static_cast<const DerivedGrid*>(this)->end();

	// for each cell in the interval
	for (const KeyType& c : GridType::cells(interval.min(), interval.max())) {
		if (!ignore.isInsideOpenBox(c)) {
			// p is a pair of iterators
			const auto& p = static_cast<const DerivedGrid*>(this)->valuesInCell(c);
			for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
				auto tmp = distFunction(qv, it->second, dist);
				if (tmp < dist) {
					dist = tmp;
					res = it;
				}
			}
		}
	}
	return res;
}

template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename QueryValueType>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::valuesInCellNeighborhood(
	const QueryValueType&             qv,
	uint                              n,
	QueryDistFunction<QueryValueType> distFunction,
	Boxui& ignore) const
{
	// types used for K closest neighbors queries
	using KClosestPairType =
		std::pair<typename GridType::ScalarType, typename DerivedGrid::ConstIterator>;
	using KClosestSet = std::set<KClosestPairType, DistIterPairComparator<KClosestPairType>>;
	KClosestSet res;

	using QVT = RemoveRefAndPointer<QueryValueType>;
	const QVT* qvv = getCleanValueTypePointer(qv);

	if (qvv) {
		Boxui currentIntervalBox;
		typename GridType::BBoxType bb = vcl::boundingBox(*qvv); //bbox of query value
		currentIntervalBox.add(GridType::cell(bb.min())); // first cell where look for closest
		currentIntervalBox.add(GridType::cell(bb.max())); // last cell where look for closest

		ignore.setNull();
		while (res.size() < n && /*res.size() < values.size() &&*/ currentIntervalBox != ignore) {
			// for each cell in the interval
			for (const KeyType& c : GridType::cells(currentIntervalBox.min(), currentIntervalBox.max())) {
				if (!ignore.isInsideOpenBox(c)) {
					const auto& p = static_cast<const DerivedGrid*>(this)->valuesInCell(c);
					for (auto it = p.first; it != p.second; ++it) { // for each value contained in the cell
						auto tmp = distFunction(qv, it->second);
						res.emplace(tmp, it);
					}
				}
			}
			ignore = currentIntervalBox;
			for (uint i = 0; i < currentIntervalBox.min().DIM; ++i) {
				if (currentIntervalBox.min()(i) != 0)
					currentIntervalBox.min()(i)--;
				if (currentIntervalBox.max()(i) != GridType::cellNumber(i))
					currentIntervalBox.max()(i)++;
			}
		}
	}

	return res;
}

/**
 * @brief This function returns a clean pointer (VT*) to the given valuetype.
 * It is just an utility function used to avoid code duplication. It allows equal treatment of
 * ValueType variable, whether that v is a pointer or not.
 *
 * Example: you have a ValueType v, and you want to compute its bounding box, but you don't know if
 * ValueType is a pointer or not.
 * Instead of doing:
 * if constexpr(std::is_pointer<ValueType>::value) {
 *     if (vv) vcl::boundingBox(*v);
 * }
 * else{
 *     vcl::boundingBox(v);
 * }
 *
 * you can do:
 * const VT* vv = getCleanValueTypePointer(v);
 * if (vv) vcl::boundingBox(*vv);
 *
 * @param v
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
template<typename T>
auto AbstractGrid<GridType, ValueType, DerivedGrid>::getCleanValueTypePointer(const T& v)
{
	using NT = RemoveRefAndPointer<T>;
	const NT* vv = nullptr; // vv is a pointer to T
	if constexpr(std::is_pointer<T>::value) {
		vv = v;
	}
	else {
		vv = &v;
	}

	return vv;
}

} // namespace vcl
