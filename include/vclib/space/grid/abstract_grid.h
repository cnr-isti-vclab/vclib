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

#ifndef VCL_SPACE_GRID_ABSTRACT_GRID_H
#define VCL_SPACE_GRID_ABSTRACT_GRID_H

#include <deque>
#include <set>

#include <vclib/algorithms/bounding_box.h>
#include <vclib/concepts/range.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/comparators.h>
#include <vclib/space/sphere.h>

namespace vcl {

// Developer Documentation
// A class that derives from an AbstractGrid must store, in some way, an association between a
// GridCell coordinate (a Point of unsigned integers) and elements of type ValueType. Each cell may
// contain more than one element value, and each element value may be stored in more than one cell
// if it intersects with more than one cell.
//
// This class is made using CRTP static polymorphism
// (https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern).
// The last template parameter, is the Derived class that derives from this grid.
// This allows to avoid calling virtual member functions, which would be called very often and may
// cause a significant overhead during queries.
//
// What you need to do is to declared your Grid Data Structure as follows:
//
// template<typename GridType, typename ValueType>
// class MyDSGrid :
//     public AbstractGrid<GridType, ValueType, MyDSGrid<GridType, ValueType>>
// { ... };
//
// Make sure that your class:
// - declares the AbstractGrid class as friend (if one of the following member functions is
//   protected/private);
// - implements iterators Iterator and ConstIterator that iterate over pairs of
//   <KeyType, ValueType> (see below for more details)
// - implements the following member functions:
//   - std::pair<Iterator, Iterator> valuesInCell(const KeyType) -> returns a pair of begin-end
//     iterators that will allow to cycle over the elements contained in a cell.
//     this function must be overloaded both in const and non-const versions, returning proper
//     const or non-const iterators.
//   - bool insertInCell(const KeyType&, const ValueType&) -> inserts the element in a
//     Grid Cell. The returned type tells if the element has been inserted (in some implementations,
//     insertion could not happen, e.g. when duplicates are not allowed).
//   - bool eraseInCell(const KeyType&, const ValueType&) -> erases all the element equal to the
//     given element from the given Grid Cell.
//     the returned type tells if the element has been erased from the given cell.
//   - Iterator end() -> classic member function that returns the end iterator
//
// Iterators of your class must:
// - Iterate over pairs of <KeyType, ValueType&>:
//   - operator*() returns a pair having first member that is the key, and second member that is a
//     reference to the value
//   - operator->() return a pointer to the same pair
// - operator+() and operator+(int), that moves the iterator in the same cell if there are more
//   values in it, or moves in the next cell-element
//
// You can reimplement in your derived class all the member functions implemented in this class that
// may take advantage of a speedup depending on the internal storage.
// You can also hide all the member functions implemented in this class that do not apply to your
// derived class.

/**
 * @brief The AbstractGrid class describes a generic Spatial Data Structure organized on a regular
 * grid, that allows to store elements (ValueType) in a particular way that will be managed by its
 * derived classes (e.g. StaticGrid, HashTableGrid).
 *
 * This class cannot be instantiated. You can only instantiate a derived class.
 *
 * This class provides only constructor and member functions that are common with all the derived
 * classes.
 * Note that derived class can hide some of the member exposed in this class (e.g. they could
 * disallow the possibility to remove elements from a grid).
 *
 * @ingroup space
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
class AbstractGrid : public GridType
{
private:
	// ValueType could be anything. We need to understand if it is a pointer, a reference or not, in
	// order to make proper optimized operations. Therefore, we declare VT, that is used internally
	// in this class. VT is ValueType without pointers or references:
	using VT = RemoveCVRefAndPointer<ValueType>;

public:
	using KeyType = GridType::CellCoord;
	using IsInCellFunction =
		std::function<bool(const typename GridType::BBoxType&, const RemoveCVRefAndPointer<ValueType>&)>;

	template<typename QueryValueType>
	using QueryDistFunction =
		std::function<typename GridType::ScalarType(const QueryValueType&, const RemoveCVRefAndPointer<ValueType>&)>;

	template<typename QueryValueType>
	using QueryBoundedDistFunction = std::function<typename GridType::ScalarType(
		const QueryValueType&,
		const RemoveCVRefAndPointer<ValueType>&,
		typename GridType::ScalarType)>;

	bool cellEmpty(const KeyType& k) const;

	std::size_t countInCell(const KeyType& k) const;

	// insert
	bool insert(const ValueType& v);

	template<typename ObjIterator>
	uint insert(ObjIterator begin, ObjIterator end);

	template<vcl::Range Rng>
	uint insert(Rng&& r);

	// erase
	bool erase(const ValueType& v);
	bool eraseAllInCell(const KeyType& k);

	// sphere operations
	uint countInSphere(const Sphere<typename GridType::ScalarType>& s) const;

	// vector of iterators - return type must be auto here (we still don't know the iterator type)
	auto valuesInSphere(const Sphere<typename GridType::ScalarType>& s) const;

	void eraseInSphere(const Sphere<typename GridType::ScalarType>& s);

	// closest queries
	template<typename QueryValueType>
	auto closestValue(
		const QueryValueType& qv,
		QueryBoundedDistFunction<QueryValueType> distFunction,
		typename GridType::ScalarType& dist) const;

	template<typename QueryValueType>
	auto closestValue(
		const QueryValueType& qv,
		QueryDistFunction<QueryValueType> distFunction,
		typename GridType::ScalarType& dist) const;

	template<typename QueryValueType>
	auto closestValue(
		const QueryValueType& qv,
		QueryDistFunction<QueryValueType> distFunction) const;

	template<typename QueryValueType>
	auto closestValue(
		const QueryValueType& qv,
		typename GridType::ScalarType& dist) const;

	template<typename QueryValueType>
	auto closestValue(const QueryValueType& qv) const;

	template<typename QueryValueType>
	auto kClosestValues(
		const QueryValueType& qv,
		uint n,
		QueryDistFunction<QueryValueType> distFunction) const;

	template<typename QueryValueType>
	auto kClosestValues(const QueryValueType& qv, uint n) const;

protected:
	AbstractGrid();

	AbstractGrid(const GridType& grid, IsInCellFunction intersects = nullptr);

	template<PointConcept PointType>
	AbstractGrid(
		const PointType& min,
		const PointType& max,
		const KeyType&   sizes,
		IsInCellFunction intersects = nullptr);

	template<typename BoxType>
	AbstractGrid(
		const BoxType&   bbox,
		const KeyType&   sizes,
		IsInCellFunction intersects = nullptr);

	// this constructor **does not insert elements**.
	template<typename ObjIterator>
	AbstractGrid(ObjIterator begin, ObjIterator end, IsInCellFunction intersects = nullptr);

	template<vcl::Range Rng>
	AbstractGrid(Rng&& r, IsInCellFunction intersects = nullptr);

	// custom function that checks if a value intersects with a cell (a box)
	// if not initialized, bounding box of value will be used to check if it is in cell
	IsInCellFunction intersects;

private:
	using Boxui = vcl::Box<Point<uint, GridType::DIM>>;

	template<typename Iter>
	struct IterComparator
	{
		bool operator()(const Iter& i1, const Iter& i2) const {
			return i1->second < i2->second;
		}
	};

	template<typename Pair>
	struct DistIterPairComparator
	{
		bool operator()(const Pair& p1, const Pair& p2) const
		{
			if (p1.first == p2.first) {
				return p1.second->second < p2.second->second;
			}
			return p1.first < p2.first;
		}
	};

	//std::deque<ValueType> values;

	template<typename Iterator>
	bool valueIsInSpehere(const Iterator& it, const Sphere<typename GridType::ScalarType>& s) const;

	template<typename QueryValueType>
	auto closestInCells(
		const QueryValueType&             qv,
		typename GridType::ScalarType&    dist,
		const Boxui&                      interval,
		QueryBoundedDistFunction<QueryValueType> distFunction,
		const Boxui&                      ignore = Boxui()) const;

	template<typename QueryValueType>
	auto valuesInCellNeighborhood(
		const QueryValueType&             qv,
		uint                              n,
		QueryDistFunction<QueryValueType> distFunction,
		Boxui& ignore) const;
};

} // namespace vcl

#include "abstract_grid.cpp"

#endif // VCL_SPACE_GRID_ABSTRACT_GRID_H
