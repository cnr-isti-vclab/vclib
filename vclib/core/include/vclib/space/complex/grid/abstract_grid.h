/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_SPACE_COMPLEX_GRID_ABSTRACT_GRID_H
#define VCL_SPACE_COMPLEX_GRID_ABSTRACT_GRID_H

#include <vclib/algorithms/core/bounding_box.h>
#include <vclib/algorithms/core/distance/functions.h>
#include <vclib/concepts/range.h>
#include <vclib/misc/comparators.h>
#include <vclib/space/core/sphere.h>

#include <set>

namespace vcl {

/*
 * Developer Documentation
 * A class that derives from an AbstractGrid must store, in some way, an
 * association between a GridCell coordinate (a Point of unsigned integers) and
 * elements of type ValueType. Each cell may contain more than one element
 * value, and each element value may be stored in more than one cell if it
 * intersects with more than one cell.
 *
 * This class is made using CRTP static polymorphism
 * (https: *en.wikipedia.org/wiki/Curiously_recurring_template_pattern).
 * The last template parameter, is the Derived class that derives from this
 * grid. This allows to avoid calling virtual member functions, which would be
 * called very often and may cause a significant overhead during queries.
 *
 * What you need to do is to declared your Grid Data Structure as follows:
 *
 * template<typename GridType, typename ValueType>
 * class MyDSGrid :
 *     public AbstractGrid<GridType, ValueType, MyDSGrid<GridType, ValueType>>
 * { ... };
 *
 * Make sure that your class:
 * - declares the AbstractGrid class as friend (if one of the following member
 * functions is
 *   protected/private);
 * - implements iterators Iterator and ConstIterator that iterate over pairs of
 *   <KeyType, ValueType> (see below for more details)
 * - implements the following member functions:
 *   - std::pair<Iterator, Iterator> valuesInCell(const KeyType) -> returns a
 *     pair of begin-end iterators that will allow to cycle over the elements
 *     contained in a cell. this function must be overloaded both in const and
 *     non-const versions, returning proper const or non-const iterators.
 *   - bool insertInCell(const KeyType&, const ValueType&) -> inserts the
 *     element in a Grid Cell. The returned type tells if the element has been
 *     inserted (in some implementations, insertion could not happen, e.g. when
 *     duplicates are not allowed).
 *   - bool eraseInCell(const KeyType&, const ValueType&) -> erases all the
 *     element equal to the given element from the given Grid Cell. The returned
 *     type tells if the element has been erased from the given cell.
 *   - Iterator end() -> classic member function that returns the end iterator
 *
 * Iterators of your class must:
 * - Iterate over pairs of <KeyType, ValueType&>:
 *   - operator*() returns a pair having first member that is the key, and
 *     second member that is a reference to the value
 *   - operator->() return a pointer to the same pair
 * - operator+() and operator+(int), that moves the iterator in the same cell if
 *   there are more values in it, or moves in the next cell-element
 *
 * You can reimplement in your derived class all the member functions
 * implemented in this class that may take advantage of a speedup depending on
 * the internal storage. You can also hide all the member functions implemented
 * in this class that do not apply to your derived class.
 */

/**
 * @brief The AbstractGrid class describes a generic Spatial Data Structure
 * organized on a regular grid, that allows to store elements (ValueType) in a
 * particular way that will be managed by its derived classes (e.g. StaticGrid,
 * HashTableGrid).
 *
 * This class cannot be instantiated. You can only instantiate a derived class.
 *
 * This class provides only constructor and member functions that are common
 * with all the derived classes. Note that derived class can hide some of the
 * member exposed in this class (e.g. they could disallow the possibility to
 * remove elements from a grid).
 *
 * @ingroup space_complex
 */
template<typename GridType, typename ValueType, typename DerivedGrid>
class AbstractGrid : public GridType
{
public:
    /**
     * @brief The IntersectsCellFunction type is a std::function that takes as
     * input a bounding box and a value, and returns true if the value
     * intersects the bounding box.
     *
     * It is used to customize the behavior of the grid when inserting values.
     */
    using IntersectsCellFunction = std::function<bool(
        const typename GridType::BBoxType&,
        const RemoveCVRefAndPointer<ValueType>&)>;

protected:
    /* custom function that checks if a value intersects with a cell (a box)
       if not initialized, bounding box of value will be used to check if it is
       in cell */
    IntersectsCellFunction mIntersectsFun;

public:
    using KeyType = GridType::CellCoord;

    /**
     * @brief The QueryDistFunction type is a std::function that takes as input
     * a query value and a value stored in the grid, and returns the distance
     * between the two values.
     *
     * It is used to customize the behavior of the grid when querying values.
     */
    template<typename QueryValueType>
    using QueryDistFunction = std::function<typename GridType::ScalarType(
        const QueryValueType&,
        const RemoveCVRefAndPointer<ValueType>&)>;

    /**
     * @brief The QueryBoundedDistFunction type is a std::function that takes as
     * input a query value, a value stored in the grid, and a maximum distance
     * between the two values, and returns the distance between the two values.
     * If the distance is greater than the maximum distance, the function must
     * return a value greater than the maximum distance.
     *
     * It is used to customize the behavior of the grid when querying values,
     * and it allows to speedup the query by avoiding to compute some distances
     * that are greater than the maximum distance.
     */
    template<typename QueryValueType>
    using QueryBoundedDistFunction =
        std::function<typename GridType::ScalarType(
            const QueryValueType&,
            const RemoveCVRefAndPointer<ValueType>&,
            typename GridType::ScalarType)>;

    bool cellEmpty(const KeyType& k) const
    {
        auto p = derived()->valuesInCell(k);
        return p.first == p.second;
    }

    std::size_t countInCell(const KeyType& k) const
    {
        auto p = derived()->valuesInCell(k);
        return std::distance(p.first, p.second);
    }

    /**
     * @brief Inserts the given element in the AbstractGrid.
     *
     * If the ValueType is Puntual (a Point or a Vertex), the element will be
     * inserted in just one cell of the grid. If the element is a spatial object
     * having a bounding box with min != max, the element will be stored in all
     * the cells where its bounding box lies.
     *
     * @param v
     * @return
     */
    bool insert(const ValueType& v)
    {
        const VT* vv = addressOfObj(v);

        // if vv is a valid pointer (ValueType, or ValueType* if ValueType is
        // not a pointer)
        if (vv) {
            // first and last cell where insert (could be the same)
            KeyType bmin, bmax;

            // if ValueType is Point, Point*, Vertex, Vertex*
            if constexpr (PointConcept<VT> || VertexConcept<VT>) {
                typename GridType::PointType p;
                if constexpr (PointConcept<VT>)
                    p = *vv;
                else
                    p = vv->coord();
                bmin = bmax = GridType::cell(p);
            }
            else { // else, call the boundingBox function
                // bounding box of value
                typename GridType::BBoxType bb = boundingBox(*vv);

                bmin = GridType::cell(bb.min()); // first cell where insert
                bmax = GridType::cell(bb.max()); // last cell where insert
            }

            bool ins = false;

            // custom intersection function between cell and value
            if (mIntersectsFun) {
                for (const auto& cell : GridType::cells(bmin, bmax)) {
                    if (mIntersectsFun(
                            GridType::cellBox(cell), dereferencePtr(v))) {
                        ins |= derived()->insertInCell(cell, v);
                    }
                }
            }
            else {
                for (const auto& cell : GridType::cells(bmin, bmax)) {
                    ins |= derived()->insertInCell(cell, v);
                }
            }
            return ins;
        }
        return false;
    }

    /**
     * @brief Inserts all the elements from `begin` to `end`. The type
     * referenced by the iterator must be the ValueType of the AbstractGrid.
     * @param begin
     * @param end
     * @return The number of inserted elements.
     */
    template<typename ObjIterator>
    uint insert(ObjIterator begin, ObjIterator end)
    {
        uint cnt = 0;
        for (ObjIterator it = begin; it != end; ++it)
            if (insert(*it))
                cnt++;
        return cnt;
    }

    /**
     * @brief Inserts all the elements contained in the input range r, from
     * `begin` to `end`. The type referenced by the iterator must be the
     * ValueType of the AbstractGrid.
     * @param r: a range that satisfies the concept std::ranges::range
     * @return The number of inserted elements.
     */
    template<Range Rng>
    uint insert(Rng&& r)
    {
        return insert(std::ranges::begin(r), std::ranges::end(r));
    }

    bool erase(const ValueType& v)
    {
        const VT* vv = addressOfObj(v);

        if (vv) {
            // first and last cell where erase (could be the same)
            KeyType bmin, bmax;

            if constexpr (PointConcept<VT> || VertexConcept<VT>) {
                typename GridType::PointType p;
                if constexpr (PointConcept<VT>)
                    p = *vv;
                else
                    p = vv->coord();
                bmin = bmax = GridType::cell(p);
            }
            else {
                typename GridType::BBoxType bb = boundingBox(*vv);

                bmin = GridType::cell(bb.min);
                bmax = GridType::cell(bb.max);
            }

            bool found = false;
            for (const auto& cell : GridType::cells(bmin, bmax)) {
                found |= derived()->eraseInCell(cell, v);
            }
            return found;
        }
        return false;
    }

    bool eraseAllInCell(const KeyType& k)
    {
        bool  res = false;
        auto& p   = derived()->valuesInCell(k);
        // for each value contained in the cell
        for (auto it = p.first; it != p.second; ++it) {
            res |= derived()->eraseInCell(k, it->second);
        }
        return res;
    }

    // sphere operations
    uint countInSphere(const Sphere<typename GridType::ScalarType>& s) const
    {
        return valuesInSphere(s).size();
    }

    // vector of iterators - return type must be auto here (we still don't know
    // the iterator type)
    auto valuesInSphere(const Sphere<typename GridType::ScalarType>& s) const
    {
        using Iter    = DerivedGrid::ConstIterator;
        using IterSet = std::set<Iter, IterComparator<Iter>>;

        // will use this set only if the value type is not a point -- that is
        // when the value can occupy more than one single cell. We therefore
        // push the results in a set in order to avoid possible duplicates
        IterSet valuesSet;

        std::vector<Iter> resVec;

        // interval of cells containing the sphere
        KeyType first = GridType::cell(s.center() - s.radius());
        KeyType last  = GridType::cell(s.center() + s.radius());

        // for each cell in the interval
        for (const KeyType& c : GridType::cells(first, last)) {
            // p is a pair of iterators
            const auto& p = derived()->valuesInCell(c);
            // for each value contained in the cell
            for (auto it = p.first; it != p.second; ++it) {
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

        // if the valuetype is a point (or vertex), we already pushed in resVec
        // - faster than using set
        if constexpr (!PointConcept<VT> && !VertexConcept<VT>) {
            resVec = std::vector<Iter>(valuesSet.begin(), valuesSet.end());
        }
        return resVec;
    }

    void eraseInSphere(const Sphere<typename GridType::ScalarType>& s)
    {
        // interval of cells containing the sphere
        KeyType first = GridType::cell(s.center() - s.radius());
        KeyType last  = GridType::cell(s.center() + s.radius());

        // for each cell in the interval
        for (const KeyType& c : GridType::cells(first, last)) {
            // p is a pair of iterators
            const auto& p = derived()->valuesInCell(c);
            // for each value contained in the cell
            for (auto it = p.first; it != p.second; ++it) {
                if (valueIsInSpehere(it, s)) {
                    eraseInCell(it->first, it->second);
                }
            }
        }
    }

    // closest queries
    template<typename QueryValueType>
    auto closestValue(
        const QueryValueType&                    qv,
        QueryBoundedDistFunction<QueryValueType> distFunction,
        typename GridType::ScalarType&           dist) const
    {
        using ScalarType = GridType::ScalarType;
        using PointType  = GridType::PointType;
        using ResType    = DerivedGrid::ConstIterator;

        using QVT         = RemoveCVRefAndPointer<QueryValueType>;
        const QVT* qvv    = addressOfObj(qv);
        ResType    result = derived()->end();

        if (qvv) {
            typename GridType::ScalarType maxDist = dist;

            const ScalarType cellDiagonal = GridType::cellDiagonal();

            // bbox of query value
            typename GridType::BBoxType bb = boundingBox(*qvv);

            ScalarType centerDist = cellDiagonal;
            PointType  center     = bb.center();

            // we first look just on the cells where the query value lies

            // here, we will store also the looking interval where we need to
            // look
            Boxui currentIntervalBox;
            Boxui lastIntervalBox = currentIntervalBox;
            // first cell where look for closest
            currentIntervalBox.add(GridType::cell(bb.min()));
            // last cell where look for closest
            currentIntervalBox.add(GridType::cell(bb.max()));

            // looking just on cells where query lies
            ScalarType tmp = cellDiagonal;
            result = closestInCells(qv, tmp, currentIntervalBox, distFunction);

            // we have found (maybe) the closest value contained in the cell(s)
            // where the query value lies (if the cells were empty, we did not
            // found nothing).

            // if we found a value, we update the dist, which becames the
            // max dist value. We will use it for the final search of the
            // closest value
            if (result != derived()->end()) {
                dist       = tmp;
                centerDist = dist;
            }
            else {
                // we did not find any value in the cells where the query value
                // lies. We need to look in the neighborhood cells and increase
                // the looking distance until we find a value or we reach the
                // max distance

                bool end = false;

                do {
                    lastIntervalBox = currentIntervalBox;
                    currentIntervalBox.add(GridType::cell(center - centerDist));
                    currentIntervalBox.add(GridType::cell(center + centerDist));

                    result = closestInCells(
                        qv,
                        dist,
                        currentIntervalBox,
                        distFunction,
                        lastIntervalBox);

                    end = result != derived()->end();
                    end |= (centerDist > maxDist);
                    end |=
                        (center - centerDist < GridType::min() &&
                         center + centerDist > GridType::max());

                    // update the centerDist for the next loop (after computing
                    // the end loop condition!!)
                    centerDist += cellDiagonal;
                } while (!end);
            }

            if (result != derived()->end()) {
                // last check: look in all the cells inside the sphere of radius
                // dist, in case there is a closest value
                currentIntervalBox.add(GridType::cell(center - dist));
                currentIntervalBox.add(GridType::cell(center + dist));
                auto r = closestInCells(
                    qv,
                    dist,
                    currentIntervalBox,
                    distFunction,
                    lastIntervalBox);
                if (r != derived()->end()) {
                    result = r;
                }
            }
        }

        return result;
    }

    template<typename QueryValueType>
    auto closestValue(
        const QueryValueType&             qv,
        QueryDistFunction<QueryValueType> distFunction,
        typename GridType::ScalarType&    dist) const
    {
        QueryBoundedDistFunction<QueryValueType> boundDistFun =
            [&](const QueryValueType&                   q,
                const RemoveCVRefAndPointer<ValueType>& v,
                typename GridType::ScalarType) {
                return distFunction(q, v);
            };

        dist = std::numeric_limits<typename GridType::ScalarType>::max();

        return closestValue(qv, boundDistFun, dist);
    }

    template<typename QueryValueType>
    auto closestValue(
        const QueryValueType&             qv,
        QueryDistFunction<QueryValueType> distFunction) const
    {
        typename GridType::ScalarType maxDist =
            std::numeric_limits<typename GridType::ScalarType>::max();
        return closestValue(qv, distFunction, maxDist);
    }

    template<typename QueryValueType>
    auto closestValue(
        const QueryValueType&          qv,
        typename GridType::ScalarType& dist) const
    {
        std::function f = boundedDistFunction<
            QueryValueType,
            RemoveCVRefAndPointer<ValueType>,
            typename GridType::ScalarType>();
        return closestValue(qv, f, dist);
    }

    template<typename QueryValueType>
    auto closestValue(const QueryValueType& qv) const
    {
        std::function f = boundedDistFunction<
            QueryValueType,
            RemoveCVRefAndPointer<ValueType>,
            typename GridType::ScalarType>();
        typename GridType::ScalarType maxDist =
            std::numeric_limits<typename GridType::ScalarType>::max();
        return closestValue(qv, f, maxDist);
    }

    template<typename QueryValueType>
    auto kClosestValues(
        const QueryValueType&             qv,
        uint                              n,
        QueryDistFunction<QueryValueType> distFunction) const
    {
        using ResType = std::vector<typename DerivedGrid::ConstIterator>;
        // KClosest Types
        using KClosestPairType = std::pair<
            typename GridType::ScalarType,
            typename DerivedGrid::ConstIterator>;
        using KClosestSet = std::
            set<KClosestPairType, DistIterPairComparator<KClosestPairType>>;

        Boxui ignore; // will contain the interval of cells already visited

        KClosestSet set = valuesInCellNeighborhood(qv, n, distFunction, ignore);

        auto it = set.size() >= n ? std::next(set.begin(), n - 1) : set.end();
        // if we didn't found n values, it means that there aren't n values in
        // the grid - nothing to do
        if (it != set.end()) {
            using QVT      = RemoveCVRefAndPointer<QueryValueType>;
            const QVT* qvv = addressOfObj(qv);

            // bbox of query value
            typename GridType::BBoxType bb = boundingBox(*qvv);
            // we need to be sure that there are no values that are closest
            // w.r.t. the n-th that we have already found by looking in the cell
            // neighborhood. we extend the bb with the distance of the n-th
            // closest found value
            bb.min() -= it->first;
            bb.max() += it->first;

            // and we look in all of these cells
            Boxui currentIntervalBox;
            // first cell where look for closest
            currentIntervalBox.add(GridType::cell(bb.min()));
            // last cell where look for closest
            currentIntervalBox.add(GridType::cell(bb.max()));

            // for all the cells in the current interval box
            for (const KeyType& c : GridType::cells(
                     currentIntervalBox.min(), currentIntervalBox.max())) {
                if (!ignore.isInside(c)) {
                    // get the values of the cell c
                    const auto& p = derived()->valuesInCell(c);
                    // for each value contained in the cell c
                    for (auto it = p.first; it != p.second; ++it) {
                        auto tmp = distFunction(qv, dereferencePtr(it->second));
                        set.insert(std::make_pair(tmp, it));
                    }
                }
            }
        }

        ResType vec;
        // if there are more than n values in the set, we will return n values,
        // otherwise set.size()
        uint retNValues = std::min((uint) set.size(), n);
        vec.reserve(retNValues);
        it = set.begin();
        for (uint i = 0; i < retNValues; i++) {
            vec.push_back(it->second);
            it++;
        }
        return vec;
    }

    template<typename QueryValueType>
    auto kClosestValues(const QueryValueType& qv, uint n) const
    {
        // get the default dist function between the query value and the
        // elements of the grid
        std::function f =
            distFunction<QueryValueType, RemoveCVRefAndPointer<ValueType>>();
        return kClosestValues(qv, n, f);
    }

protected:
    /**
     * @brief Empty constructor, creates an usable AbstractGrid, since the Grid
     * is not initialized.
     */
    AbstractGrid() {}

    /**
     * @brief Creates a AbstractGrid that allows to store ValueType values on
     * the given grid.
     *
     * @param [in] grid: the grid on which the values will be stored.
     * @param [in] intersects: the function that will be used to check if a
     * inserted value intersects (lies in) a cell. If nullptr, the default
     * intersects function will be used.
     */
    AbstractGrid(
        const GridType&        grid,
        IntersectsCellFunction intersects = nullptr) :
            GridType(grid), mIntersectsFun(intersects)
    {
    }

    /**
     * @brief Creates a AbstractGrid that allows to store ValueType values on a
     * Grid having `min` as minimum coordinte of the Grid, `max` as maximum
     * coordinate of the grid, and the number of cells per dimension given by
     * `sizes`.
     *
     * @param [in] min
     * @param [in] max
     * @param [in] sizes
     * @param [in] intersects: the function that will be used to check if a
     * inserted value intersects (lies in) a cell. If nullptr, the default
     * intersects function will be used.
     */
    template<PointConcept PointType>
    AbstractGrid(
        const PointType&       min,
        const PointType&       max,
        const KeyType&         sizes,
        IntersectsCellFunction intersects = nullptr) :
            GridType(min, max, sizes), mIntersectsFun(intersects)
    {
    }

    /**
     * @brief Creates a AbstractGrid that allows to store ValueType values on a
     * Grid bounded by `bbox`, and having the number of cells per dimension
     * given by `sizes`.
     *
     * @param [in] bbox
     * @param [in] sizes
     * @param [in] intersects: the function that will be used to check if a
     * inserted value intersects (lies in) a cell. If nullptr, the default
     * intersects function will be used.
     */
    template<typename BoxType>
    AbstractGrid(
        const BoxType&         bbox,
        const KeyType&         sizes,
        IntersectsCellFunction intersects = nullptr) :
            GridType(bbox, sizes), mIntersectsFun(intersects)
    {
    }

    /**
     * @brief Creates an AbstractGrid having a proper Grid to store the
     * elements.
     *
     * The bounding box and the sizes of the Grid are automatically computed.
     * Bounding box is computed starting from the bounding box of all the
     * iterated elements, and then inflated. The number of cells per dimension
     * is computed using the `vcl::bestGridSize` function.
     *
     * @note This constructor **does not insert the elements** in the Grid Data
     * Structure. Pure virtual classes cannot call pure virtual member functions
     * from constuctors. It is duty of each derived class to insert the elements
     * after calling this constructor.
     *
     * @param [in] begin
     * @param [in] end
     * @param [in] intersects: the function that will be used to check if a
     * inserted value intersects (lies in) a cell. If nullptr, the default
     * intersects function will be used.
     */
    template<typename ObjIterator>
    AbstractGrid(
        ObjIterator            begin,
        ObjIterator            end,
        IntersectsCellFunction intersects = nullptr) :
            mIntersectsFun(intersects)
    {
        using ScalarType = GridType::ScalarType;
        using BBoxType   = GridType::BBoxType;
        using CellCoord  = GridType::CellCoord;

        BBoxType bbox      = boundingBox(begin, end);
        uint     nElements = std::distance(begin, end);

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
     * @brief Creates an AbstractGrid having a proper Grid to store the
     * elements.
     *
     * The bounding box and the sizes of the Grid are automatically computed.
     * Bounding box is computed starting from the bounding box of all the
     * iterated elements, and then inflated. The number of cells per dimension
     * is computed using the `vcl::bestGridSize` function.
     *
     * @note This constructor **does not insert the elements** in the Grid Data
     * Structure. Pure virtual classes cannot call pure virtual member functions
     * from constuctors. It is duty of each derived class to insert the elements
     * after calling this constructor.
     *
     * @param [in] r: a range that satisfies the concept std::ranges::range
     * @param [in] intersects: the function that will be used to check if a
     * inserted value intersects (lies in) a cell. If nullptr, the default
     * intersects function will be used.
     */
    template<Range Rng>
    AbstractGrid(Rng&& r, IntersectsCellFunction intersects = nullptr) :
            AbstractGrid(std::ranges::begin(r), std::ranges::end(r), intersects)
    {
    }

private:
    /* ValueType could be anything. We need to understand if it is a pointer, a
       reference or not, in order to make proper optimized operations.
       Therefore, we declare VT, that is used internally in this class. VT is
       ValueType without pointers or references: */
    using VT = RemoveCVRefAndPointer<ValueType>;

    using Boxui = Box<Point<uint, GridType::DIM>>;

    template<typename Iter>
    struct IterComparator
    {
        bool operator()(const Iter& i1, const Iter& i2) const
        {
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

    DerivedGrid* derived() { return static_cast<DerivedGrid*>(this); }

    const DerivedGrid* derived() const
    {
        return static_cast<const DerivedGrid*>(this);
    }

    // std::deque<ValueType> values;

    /**
     * This function is meant to be called by another function of the
     * AbstractGrid. Checks if the current iterated value is inside a sphere
     */
    template<typename Iterator>
    bool valueIsInSpehere(
        const Iterator&                              it,
        const Sphere<typename GridType::ScalarType>& s) const
    {
        const VT* vv = addressOfObj(it->second);

        bool test = false;
        if constexpr (PointConcept<VT> || VertexConcept<VT>) {
            typename GridType::PointType p;
            if constexpr (PointConcept<VT>)
                p = *vv;
            else
                p = vv->coord();
            test = vv && s.isInside(p);
        }
        else { // check if the bbox of the value intersects the sphere
            test = vv && s.intersects(boundingBox(*vv));
        }
        return test;
    }

    /**
     * This function is meant to be called by another function of the
     * AbstractGrid. Returns the closest Value (if any) to the given query value
     * contained in the given interval
     */
    template<typename QueryValueType>
    auto closestInCells(
        const QueryValueType&                    qv,
        typename GridType::ScalarType&           dist,
        const Boxui&                             interval,
        QueryBoundedDistFunction<QueryValueType> distFunction,
        const Boxui&                             ignore = Boxui()) const
    {
        using ResType = DerivedGrid::ConstIterator;
        ResType res   = derived()->end();

        // for each cell in the interval
        for (const KeyType& c :
             GridType::cells(interval.min(), interval.max())) {
            if (!ignore.isInsideStrict(c)) {
                // p is a pair of iterators
                const auto& p = derived()->valuesInCell(c);
                // for each value contained in the cell
                for (auto it = p.first; it != p.second; ++it) {
                    auto tmp =
                        distFunction(qv, dereferencePtr(it->second), dist);
                    if (tmp < dist) {
                        dist = tmp;
                        res  = it;
                    }
                }
            }
        }
        return res;
    }

    template<typename QueryValueType>
    auto valuesInCellNeighborhood(
        const QueryValueType&             qv,
        uint                              n,
        QueryDistFunction<QueryValueType> distFunction,
        Boxui&                            ignore) const
    {
        // types used for K closest neighbors queries
        using KClosestPairType = std::pair<
            typename GridType::ScalarType,
            typename DerivedGrid::ConstIterator>;
        using KClosestSet = std::
            set<KClosestPairType, DistIterPairComparator<KClosestPairType>>;

        KClosestSet res;

        using QVT      = RemoveCVRefAndPointer<QueryValueType>;
        const QVT* qvv = addressOfObj(qv);

        if (qvv) {
            Boxui currentIntervalBox;
            // bbox of query value
            typename GridType::BBoxType bb = boundingBox(*qvv);
            // first cell where look for closest
            currentIntervalBox.add(GridType::cell(bb.min()));
            // last cell where look for closest
            currentIntervalBox.add(GridType::cell(bb.max()));

            ignore.setNull();
            while (res.size() < n && currentIntervalBox != ignore) {
                // for each cell in the interval
                for (const KeyType& c : GridType::cells(
                         currentIntervalBox.min(), currentIntervalBox.max())) {
                    if (!ignore.isInside(c)) {
                        const auto& p = derived()->valuesInCell(c);

                        // for each value contained in the cell
                        for (auto it = p.first; it != p.second; ++it) {
                            auto tmp =
                                distFunction(qv, dereferencePtr(it->second));
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
};

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_GRID_ABSTRACT_GRID_H
