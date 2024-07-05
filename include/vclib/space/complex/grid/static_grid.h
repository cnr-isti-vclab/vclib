/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SPACE_COMPLEX_GRID_STATIC_GRID_H
#define VCL_SPACE_COMPLEX_GRID_STATIC_GRID_H

#include <set>
#include <vector>

#include <vclib/concepts/ranges/mesh/vertex_range.h>

#include "abstract_grid.h"
#include "iterators/static_grid_iterator.h"
#include "regular_grid.h"

namespace vcl {

template<typename GridType, typename ValueType>
class StaticGrid :
        public AbstractGrid<
            GridType,
            ValueType,
            StaticGrid<GridType, ValueType>>
{
    using AbsGrid =
        AbstractGrid<GridType, ValueType, StaticGrid<GridType, ValueType>>;

    using PairType       = std::pair<uint, ValueType>;
    using PairComparator = FirstElementPairComparator<PairType>;

    friend AbsGrid;

    PairComparator mComparator = PairComparator();

    // each value is stored as a pair: [cell index of the grid - value]
    // when the grid is built, this vector is sorted by the cell indices
    std::vector<PairType> mValues;

    // for each cell of the grid, we store the index (in the values vector ) of
    // the first ValueType object contained in the cell, or values.size() if the
    // cell is empty
    std::vector<uint> mGrid;

public:
    using KeyType                = AbsGrid::KeyType;
    using IntersectsCellFunction = AbsGrid::IntersectsCellFunction;

    using Iterator      = StaticGridIterator<KeyType, ValueType, GridType>;
    using ConstIterator = ConstStaticGridIterator<KeyType, ValueType, GridType>;

    StaticGrid() {}

    StaticGrid(const GridType& g) : AbsGrid(g) {}

    template<typename ObjIterator>
    StaticGrid(
        ObjIterator                   begin,
        ObjIterator                   end,
        const IntersectsCellFunction& intersects = nullptr) :
            AbsGrid(begin, end, intersects)
    {
        AbsGrid::insert(begin, end);
        build();
    }

    template<vcl::Range Rng>
    StaticGrid(Rng&& r, const IntersectsCellFunction& intersects = nullptr) :
            StaticGrid(std::ranges::begin(r), std::ranges::end(r), intersects)
    {
    }

    void build()
    {
        uint totCellNumber = 1;
        for (uint i = 0; i < GridType::DIM; ++i) {
            totCellNumber *= GridType::cellNumber(i);
        }

        mGrid.resize(totCellNumber);

        std::sort(mValues.begin(), mValues.end(), mComparator);

        // values[vi].first points to the next non empty cell in the grid
        uint vi = 0;

        // for each cell
        for (uint ci = 0; ci < mGrid.size(); ++ci) {
            // values[vi] is in the cell ci
            if (vi < mValues.size() && mValues[vi].first == ci)
                mGrid[ci] = vi;
            else { // there are no values in this grid cell
                mGrid[ci] = mValues.size(); // set the sentinel value
            }

            // move vi to the next non-empty cell
            // skip all the values that are in the current cell ci
            // won't increment vi if the values[vi].first is not equal to ci
            while (vi < mValues.size() && mValues[vi].first == ci) {
                vi++;
            }
        }
    }

    bool empty() const { return mValues.empty(); }

    bool cellEmpty(const KeyType& k) const
    {
        uint ind = GridType::indexOfCell(k);
        return mGrid[ind] == mValues.size();
    }

    std::set<KeyType> nonEmptyCells() const
    {
        std::set<KeyType> keys;
        uint              actualInd = mValues.size();
        for (uint i = 0; i < mValues.size(); ++i) {
            if (mValues[i].first != actualInd) {
                actualInd = mValues[i].first;
                keys.insert(GridType::cellOfIndex(actualInd));
            }
        }
        return keys;
    }

    std::size_t countInCell(const KeyType& k) const
    {
        uint ind = GridType::indexOfCell(k);
        uint i   = mGrid[ind];
        uint cnt = 0;

        while (i < mValues.size() && mValues[i].first == ind) {
            i++;
            cnt++;
        }
        return cnt;
    }

    std::pair<Iterator, Iterator> valuesInCell(const KeyType& k)
    {
        uint ind = GridType::indexOfCell(k);
        uint i   = mGrid[ind];
        if (i < mValues.size()) {
            std::pair<Iterator, Iterator> p;
            p.first = Iterator(mValues.begin() + i, (const GridType&) *this);
            while (i < mValues.size() && mValues[i].first == ind) {
                i++;
            }
            auto it  = i < mValues.size() ? mValues.begin() + i : mValues.end();
            p.second = Iterator(it, (const GridType&) *this);
            return p;
        }
        else {
            return std::make_pair(end(), end());
        }
    }

    std::pair<ConstIterator, ConstIterator> valuesInCell(const KeyType& k) const
    {
        uint ind = GridType::indexOfCell(k);
        uint i   = mGrid[ind];
        if (i < mValues.size()) {
            std::pair<ConstIterator, ConstIterator> p;
            p.first =
                ConstIterator(mValues.begin() + i, (const GridType&) *this);
            while (i < mValues.size() && mValues[i].first == ind) {
                i++;
            }
            auto it  = i < mValues.size() ? mValues.begin() + i : mValues.end();
            p.second = ConstIterator(it, (const GridType&) *this);
            return p;
        }
        else {
            return std::make_pair(end(), end());
        }
    }

    Iterator begin()
    {
        return Iterator(mValues.begin(), (const GridType&) *this);
    }

    ConstIterator begin() const
    {
        return ConstIterator(mValues.begin(), (const GridType&) *this);
    }

    Iterator end() { return Iterator(mValues.end(), (const GridType&) *this); }

    ConstIterator end() const
    {
        return ConstIterator(mValues.end(), (const GridType&) *this);
    }

private:
    // not available member functions
    using AbsGrid::erase;
    using AbsGrid::eraseAllInCell;
    using AbsGrid::eraseInSphere;

    bool insertInCell(const KeyType& cell, const ValueType& v)
    {
        uint cellIndex = GridType::indexOfCell(cell);
        mValues.emplace_back(cellIndex, v);
        return true;
    }

    // not allowing to erase
    bool eraseInCell(const KeyType&, const ValueType&) { return false; };
};

/* Specialization Aliases */

template<typename ValueType, typename ScalarType = double>
using StaticGrid2 = StaticGrid<RegularGrid2<ScalarType>, ValueType>;

template<typename ValueType, typename ScalarType = double>
using StaticGrid3 = StaticGrid<RegularGrid3<ScalarType>, ValueType>;

/* Deduction guides */

template<PointIteratorConcept It>
StaticGrid(It, It) -> StaticGrid<
    RegularGrid<typename It::value_type, It::value_type::DIM>,
    typename It::value_type::ScalarType>;

template<PointIteratorConcept It, typename F>
StaticGrid(It, It, F) -> StaticGrid<
    RegularGrid<typename It::value_type, It::value_type::DIM>,
    typename It::value_type::ScalarType>;

template<VertexPointerRangeConcept Rng>
StaticGrid(Rng) -> StaticGrid<
    RegularGrid<
        // scalar type used for the grid, the same of the CoordType of the
        // Vertex
        typename std::remove_pointer_t<typename std::ranges::iterator_t<
            Rng>::value_type>::CoordType::ScalarType,
        3>, // the dimension of the Grid
    // the ValueType of the StaticGrid, which is the iterated type in the given
    // range (pointer to vertex)
    typename std::ranges::iterator_t<Rng>::value_type>;

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_GRID_STATIC_GRID_H
