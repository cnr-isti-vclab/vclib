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

#ifndef VCL_SPACE_GRID_REGULAR_GRID_H
#define VCL_SPACE_GRID_REGULAR_GRID_H

#include <vclib/iterators/space/grid/cell_iterator.h>
#include <vclib/space/box.h>
#include <vclib/views/view.h>

namespace vcl {

template<typename Scalar, int N>
class RegularGrid
{
    static_assert(
        N > 0,
        "Number of dimensions of the regular grid must be > 1.");

public:
    using PointType = Point<Scalar, N>;
    using BBoxType  = Box<PointType>;

private:
    BBoxType       bbox;
    Point<uint, N> siz;

public:
    static const int DIM = N;
    using ScalarType     = Scalar;
    using CellCoord      = Point<uint, N>;

    using CellIterator = vcl::CellIterator<N>;
    using CellView     = vcl::View<CellIterator>;

    RegularGrid() = default;

    RegularGrid(
        const Point<Scalar, N>& min,
        const Point<Scalar, N>& max,
        const Point<uint, N>&   size) :
            bbox(min, max),
            siz(size)
    {
    }

    RegularGrid(const Box<Point<Scalar, N>>& bbox, const Point<uint, N>& size) :
            bbox(bbox), siz(size)
    {
    }

    Point<Scalar, N> min() const { return bbox.min(); }

    Point<Scalar, N> max() const { return bbox.max(); }

    /**
     * @brief Returns the edge legth of the bounding box of the grid in the d-th
     * dimension
     * @return
     */
    Scalar length(uint d) const { return bbox.dim(d); }

    /**
     * @brief Returns the edge legths of the bounding box of the grid
     * @return
     */
    Point<Scalar, N> lengths() const
    {
        Point<Scalar, N> p;
        for (size_t i = 0; i < DIM; ++i)
            p(i) = length(i);
        return p;
    }

    /**
     * @brief Returns the number of cells of the Grid in the d-th dimension
     * @param d
     * @return
     */
    uint cellNumber(uint d) const { return siz(d); }

    /**
     * @brief Returns the number of cells for each dimension of the grid
     * @return
     */
    Point<uint, N> cellNumbers() const { return siz; }

    /**
     * @brief Return an unique index that can be associated to the given cell
     * coordinate.
     * @param c
     * @return
     */
    uint indexOfCell(const CellCoord& c) const
    {
        unsigned long int ind = c[0];
        assert(c[0] < siz[0]);
        for (unsigned int i = 1; i < N; i++) {
            assert(c[i] < siz[i]);
            ind *= siz[i];
            ind += c[i];
        }
        return ind;
    }

    /**
     * @brief Returns the cell coordinate associated to the given unique index.
     * @param index
     * @return
     */
    CellCoord cellOfIndex(uint index) const
    {
        typename RegularGrid<Scalar, N>::CellCoord c;
        for (long int i = N - 1; i >= 0; i--) {
            c[i] = index % siz[i];
            index /= siz[i];
        }
        return c;
    }

    /**
     * @brief Returns the length of a cell of the grid in the d-th dimension
     * @param d
     * @return
     */
    Scalar cellLength(uint d) const { return length(d) / cellNumber(d); }

    /**
     * @brief Returns the lengths of a cell of the grid for each dimension
     * @return
     */
    Point<Scalar, N> cellLengths() const
    {
        Point<Scalar, N> p;
        for (size_t i = 0; i < DIM; ++i)
            p(i) = cellLength(i);
        return p;
    }

    Scalar cellDiagonal() const { return cellLengths().norm(); }

    uint cell(uint d, const Scalar& s) const
    {
        if (s < bbox.min()(d))
            return 0;
        if (s > bbox.max()(d))
            return cellNumber(d) - 1;
        Scalar t = s - bbox.min()(d);
        return uint(t / cellLength(d));
    }

    CellCoord cell(const Point<Scalar, N>& p) const
    {
        CellCoord c;
        for (size_t i = 0; i < DIM; ++i)
            c(i) = cell(i, p(i));
        return c;
    }

    Point<Scalar, N> cellLowerCorner(const CellCoord& c) const
    {
        Point<Scalar, N> l;
        for (size_t i = 0; i < DIM; ++i)
            l(i) = min(i) + c(i) * cellLength(i); // cellLowerCorner(i, c(i));
        return l;
    }

    BBoxType cellBox(const CellCoord& c) const
    {
        BBoxType b;

        Point<Scalar, N> p;
        for (size_t i = 0; i < DIM; ++i)
            p(i) = (Scalar) c(i) * cellLength(i);
        p += bbox.min();
        b.min() = p;
        b.max() = (p + cellLengths());

        return b;
    }

    CellIterator cellBegin() const { return CellIterator(CellCoord(), siz); }

    CellIterator cellBegin(const CellCoord& first, const CellCoord& last) const
    {
        return CellIterator(first, last + 1);
    }

    CellIterator cellEnd() const { return CellIterator(); }

    CellView cells() const { return CellView(cellBegin(), cellEnd()); }

    CellView cells(const CellCoord& first, const CellCoord& last) const
    {
        return CellView(cellBegin(first, last), cellEnd());
    }

protected:
    void set(const Box<Point<Scalar, N>>& box, const Point<uint, N>& size)
    {
        bbox = box;
        siz  = size;
    }
};

/* Specialization Aliases */

template<typename Scalar>
using RegularGrid2 = RegularGrid<Scalar, 2>;

template<typename Scalar>
using RegularGrid3 = RegularGrid<Scalar, 3>;

/* Deduction guides */

template<PointConcept PointType, typename D>
RegularGrid(PointType, PointType, D)
    -> RegularGrid<typename PointType::ScalarType, PointType::DIM>;

/* Regular Grid related functions */

/**
 * @brief Returns the best sizes (number of cells per dimension) of a Grid,
 * starting from the lengths of the grid and the number of elements to place in
 * the grid.
 *
 * @param lengths
 * @param nElements
 * @return
 */
template<PointConcept PointType>
Point<uint, PointType::DIM> bestGridSize(
    const PointType& lengths,
    uint             nElements)
{
    using Scalar = PointType::ScalarType;

    static const int DIM = PointType::DIM;

    const uint   mincells = 1;                   // Numero minimo di celle
    const Scalar GFactor  = 1;                   // GridEntry = NumElem*GFactor
    const Scalar diag     = lengths.norm();      // Diagonale del box
    const Scalar eps      = diag * 1e-4;         // Fattore di tolleranza
    const uint   ncell    = nElements * GFactor; // Calcolo numero di voxel

    Point<uint, PointType::DIM> sizes;
    sizes.setConstant(mincells);

    bool                  sanityCheckLengths = true;
    uint                  lessEpsDimNumber   = 0;
    std::array<bool, DIM> isLessEps;

    for (uint i = 0; i < DIM; i++) {
        sanityCheckLengths = sanityCheckLengths && lengths(i) > 0.0;
        if (lengths(i) < eps) {
            lessEpsDimNumber++;
            isLessEps[i] = true;
        }
    }
    uint greaterEpsDimNumber = DIM - lessEpsDimNumber;

    if (nElements > 0 && sanityCheckLengths) {
        // no lenghts less than epsilon - standard computation for sizes
        if (greaterEpsDimNumber == DIM) {
            Scalar product = 1;
            for (uint i = 0; i < DIM; i++)
                product *= lengths(i);

            Scalar k = std::pow((ncell / product), (1.0 / DIM));

            for (uint i = 0; i < DIM; i++)
                sizes(i) = int(lengths(i) * k);
        }
        // at least one lenght is less than epsilon
        else {
            for (uint i = 0; i < DIM; i++) {
                if (isLessEps[i]) { // the ith dimension is less than epsilon
                    sizes(i) = 1;
                }
                else {
                    // compute the product between all the dimensions that are
                    // not i and not less than epsilon
                    Scalar product = 1;
                    for (uint j = 0; j < DIM; j++)
                        if (j != i && !isLessEps[j])
                            product *= lengths(j);

                    // ith dimension size
                    sizes(i) = std::pow(
                        (ncell * lengths(i) / product),
                        (1.0 / greaterEpsDimNumber));
                }
            }
        }

        for (uint i = 0; i < DIM; i++)
            sizes(i) = std::max(sizes(i), (uint) 1);
    }

    return sizes;
}

} // namespace vcl

#endif // VCL_SPACE_GRID_REGULAR_GRID_H
