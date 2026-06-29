// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_GRID_REGULAR_GRID_H
#define VCL_SPACE_COMPLEX_GRID_REGULAR_GRID_H

#include "iterators/cell_iterator.h"

#include <vclib/base.h>
#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief An N-dimensional regular grid defined by an axis-aligned bounding box
 * and a fixed number of uniformly-spaced cells along each dimension.
 *
 * The grid subdivides its bounding box into a lattice of rectangular (in 2-D)
 * or box-shaped (in 3-D and higher) cells, all of the same size.  It provides
 * utilities to:
 * - map a point in space to the cell that contains it;
 * - map a flat cell index to its N-D coordinates and vice-versa;
 * - query the geometric extent of individual cells;
 * - iterate over all cells or over a sub-range of cells.
 *
 * @tparam Scalar: Floating-point type used for coordinates (e.g. `float`,
 * `double`).
 * @tparam N: Number of spatial dimensions (must be > 0).
 *
 * @see RegularGrid2, RegularGrid3, bestGridSize()
 */
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
    BBoxType       mBBox;
    Point<uint, N> mSize;

public:
    static const int DIM = N;
    using ScalarType     = Scalar;
    using CellPos        = Point<uint, N>;

    using CellIterator = vcl::CellIterator<N>;
    using CellView     = View<CellIterator>;

    /** @brief Constructs an empty, default-initialised grid. */
    RegularGrid() = default;

    /**
     * @brief Constructs a grid from explicit corner points and a cell count.
     *
     * @param[in] min: Minimum corner of the bounding box.
     * @param[in] max: Maximum corner of the bounding box.
     * @param[in] size: Number of cells along each dimension.
     */
    RegularGrid(
        const Point<Scalar, N>& min,
        const Point<Scalar, N>& max,
        const Point<uint, N>&   size) : mBBox(min, max), mSize(size)
    {
    }

    /**
     * @brief Constructs a grid from a bounding box and a cell count.
     *
     * @param[in] bbox: Axis-aligned bounding box of the grid.
     * @param[in] size: Number of cells along each dimension.
     */
    RegularGrid(const Box<Point<Scalar, N>>& bbox, const Point<uint, N>& size) :
            mBBox(bbox), mSize(size)
    {
    }

    /**
     * @brief Constructs a grid from a bounding box, per-dimension cell lengths,
     * and a flag controlling how the bounding box is adapted.
     *
     * The bounding box is always adapted so that it covers exactly @f$ n_i @f$
     * full cells in every dimension, guaranteeing that `cellLengths()` returns
     * exactly @p cellLens after construction.
     *
     * The flag @p extendBBox controls the rounding direction used to derive
     * @f$ n_i @f$ from the input bounding box:
     *
     * - `true` (default) — round **up**: the cell count is
     *   @f$ n_i = \lceil \mathrm{bbox.dim}(i) \;/\; \mathrm{cellLens}(i) \rceil
     *   @f$ and the maximum corner is extended outward so that the grid covers
     *   at least the original bounding box.
     *
     * - `false` — round **down**: the cell count is
     *   @f$ n_i = \lfloor \mathrm{bbox.dim}(i) \;/\; \mathrm{cellLens}(i)
     *   \rfloor @f$ and the maximum corner is pulled inward so that the grid
     *   fits strictly inside the original bounding box.
     *
     * Each dimension is guaranteed to have at least 1 cell regardless of the
     * rounding direction.
     *
     * @param[in] bbox: Axis-aligned bounding box of the grid.
     * @param[in] cellLens:Side lengths of a single cell (one per dimension).
     * @param[in] extendBBox: If `true` (default), round the cell count up and
     * extend the maximum corner outward. If `false`, round the cell count down
     * and pull the maximum corner inward.
     */
    RegularGrid(
        const BBoxType&         bbox,
        const Point<Scalar, N>& cellLens,
        bool                    extendBBox = true) : mBBox(bbox)
    {
        for (uint i = 0; i < N; ++i) {
            Scalar ratio = mBBox.dim(i) / cellLens(i);
            mSize(i)     = std::max(
                extendBBox ? uint(std::ceil(ratio)) : uint(std::floor(ratio)),
                1u);
            mBBox.max()(i) = mBBox.min()(i) + mSize(i) * cellLens(i);
        }
    }

    /**
     * @brief Returns the minimum corner of the grid's bounding box.
     * @return Minimum corner point.
     */
    Point<Scalar, N> min() const { return mBBox.min(); }

    /**
     * @brief Returns the maximum corner of the grid's bounding box.
     * @return Maximum corner point.
     */
    Point<Scalar, N> max() const { return mBBox.max(); }

    /**
     * @brief Returns the edge length of the bounding box of the grid along a
     * given dimension.
     * @param[in] d: Dimension index (0-based, must be < N).
     * @return Length of the bounding box along dimension @p d.
     */
    Scalar length(uint d) const { return mBBox.dim(d); }

    /**
     * @brief Returns the edge lengths of the bounding box of the grid along
     * all dimensions.
     * @return Point whose i-th component is the length of the bounding box
     * along dimension i.
     */
    Point<Scalar, N> lengths() const
    {
        Point<Scalar, N> p;
        for (size_t i = 0; i < DIM; ++i)
            p(i) = length(i);
        return p;
    }

    /**
     * @brief Returns the number of cells along a given dimension.
     * @param[in] d: Dimension index (0-based, must be < N).
     * @return Number of cells along dimension @p d.
     */
    uint cellCount(uint d) const { return mSize(d); }

    /**
     * @brief Returns the number of cells along all dimensions.
     * @return Point whose i-th component is the cell count along dimension i.
     */
    Point<uint, N> cellCount() const { return mSize; }

    /**
     * @brief Returns the total number of cells in the grid.
     *
     * This is the product of the cell counts along all dimensions:
     * @f$ \prod_{i=0}^{N-1} \mathrm{cellCount}(i) @f$
     *
     * @return Total number of cells.
     */
    uint totalCellCount() const
    {
        uint total = 1;
        for (uint i = 0; i < N; ++i)
            total *= mSize(i);
        return total;
    }

    /**
     * @brief Returns a unique flat index associated to the given N-D cell
     * coordinates.
     *
     * The index is computed with a row-major (C-order) layout:
     * @f$ \mathrm{index} = c_0 \cdot s_1 \cdot \ldots \cdot s_{N-1}
     *     + c_1 \cdot s_2 \cdot \ldots \cdot s_{N-1} + \ldots + c_{N-1} @f$
     * where @f$ s_i @f$ is the cell count along dimension i.
     *
     * @param[in] c: N-D cell coordinates (each component must be <
     * cellCount(i)).
     * @return Flat cell index in [0, cellCount(0)*...*cellCount(N-1)).
     */
    uint indexOfCell(const CellPos& c) const
    {
        unsigned long int ind = c[0];
        assert(c[0] < mSize[0]);
        for (unsigned int i = 1; i < N; i++) {
            assert(c[i] < mSize[i]);
            ind *= mSize[i];
            ind += c[i];
        }
        return ind;
    }

    /**
     * @brief Returns the N-D cell coordinates corresponding to the given flat
     * cell index.
     *
     * This is the inverse of indexOfCell().
     *
     * @param[in] index: Flat cell index in
     * [0, cellCount(0)*...*cellCount(N-1)).
     * @return N-D cell coordinates.
     */
    CellPos cellOfIndex(uint index) const
    {
        typename RegularGrid<Scalar, N>::CellPos c;
        for (long int i = N - 1; i >= 0; i--) {
            c[i] = index % mSize[i];
            index /= mSize[i];
        }
        return c;
    }

    /**
     * @brief Returns the size of a single cell along a given dimension.
     * @param[in] d: Dimension index (0-based, must be < N).
     * @return Cell size along dimension @p d (= length(d) / cellCount(d)).
     */
    Scalar cellLength(uint d) const { return length(d) / cellCount(d); }

    /**
     * @brief Returns the sizes of a single cell along all dimensions.
     * @return Point whose i-th component is the cell size along dimension i.
     */
    Point<Scalar, N> cellLengths() const
    {
        Point<Scalar, N> p;
        for (size_t i = 0; i < DIM; ++i)
            p(i) = cellLength(i);
        return p;
    }

    /**
     * @brief Returns the length of the space diagonal of a single cell.
     * @return Euclidean norm of the cell's side-length vector.
     */
    Scalar cellDiagonal() const { return cellLengths().norm(); }

    /**
     * @brief Returns the hyper-volume of a single cell.
     *
     * In 2-D this equals the cell area; in 3-D the cell volume; in general it
     * is the product of the cell lengths along all dimensions:
     * @f$ \prod_{i=0}^{N-1} \mathrm{cellLength}(i) @f$
     *
     * @return Product of cell lengths along all dimensions.
     */
    Scalar cellVolume() const
    {
        Scalar vol = 1;
        for (uint i = 0; i < N; ++i)
            vol *= cellLength(i);
        return vol;
    }

    /**
     * @brief Returns the index of the cell that contains the coordinate @p s
     * along dimension @p d.
     *
     * Values outside the grid are clamped to the first or last cell.
     *
     * @param[in] d: Dimension index (0-based, must be < N).
     * @param[in] s: Coordinate value along dimension @p d.
     * @return Cell index along dimension @p d.
     */
    uint cell(uint d, const Scalar& s) const
    {
        if (s < mBBox.min()(d))
            return 0;
        if (s > mBBox.max()(d))
            return cellCount(d) - 1;
        Scalar t = s - mBBox.min()(d);
        return uint(t / cellLength(d));
    }

    /**
     * @brief Returns the N-D cell coordinates of the cell that contains point
     * @p p.
     *
     * Each component is computed independently via cell(uint, Scalar), so
     * points outside the grid are clamped to the boundary cells.
     *
     * @param[in] p: Query point.
     * @return N-D cell coordinates of the cell containing @p p.
     */
    CellPos cell(const Point<Scalar, N>& p) const
    {
        CellPos c;
        for (size_t i = 0; i < DIM; ++i)
            c(i) = cell(i, p(i));
        return c;
    }

    /**
     * @brief Returns the lower corner (minimum point) of the given cell.
     * @param[in] c: N-D cell coordinates.
     * @return Minimum corner of the axis-aligned box of cell @p c.
     */
    Point<Scalar, N> cellLowerCorner(const CellPos& c) const
    {
        Point<Scalar, N> l;
        for (size_t i = 0; i < DIM; ++i)
            l(i) = min(i) + c(i) * cellLength(i); // cellLowerCorner(i, c(i));
        return l;
    }

    /**
     * @brief Returns the center point of the given cell.
     * @param[in] c: N-D cell coordinates.
     * @return Center of cell @p c (= cellLowerCorner(c) + 0.5 * cellLengths()).
     */
    Point<Scalar, N> cellCenter(const CellPos& c) const
    {
        Point<Scalar, N> p;
        for (size_t i = 0; i < DIM; ++i)
            p(i) = mBBox.min()(i) + (c(i) + Scalar(0.5)) * cellLength(i);
        return p;
    }

    /**
     * @brief Returns the axis-aligned bounding box of the given cell.
     * @param[in] c: N-D cell coordinates.
     * @return Bounding box of cell @p c.
     */
    BBoxType cellBox(const CellPos& c) const
    {
        BBoxType b;

        Point<Scalar, N> p;
        for (size_t i = 0; i < DIM; ++i)
            p(i) = (Scalar) c(i) * cellLength(i);
        p += mBBox.min();
        b.min() = p;
        b.max() = (p + cellLengths());

        return b;
    }

    /**
     * @brief Returns an iterator to the first cell of the grid.
     *
     * Iteration proceeds in row-major (C-order) over all N-D cell coordinates.
     *
     * @return Iterator pointing to cell (0, 0, ..., 0).
     */
    CellIterator cellBegin() const { return CellIterator(CellPos(), mSize); }

    /**
     * @brief Returns an iterator to the first cell of a sub-range of cells.
     *
     * @param[in] first N-D coordinates of the first cell in the sub-range
     * (inclusive).
     * @param[in] last: N-D coordinates of the last cell in the sub-range
     * (inclusive).
     * @return Iterator pointing to cell @p first.
     */
    CellIterator cellBegin(const CellPos& first, const CellPos& last) const
    {
        return CellIterator(first, last + 1);
    }

    /**
     * @brief Returns the past-the-end cell iterator.
     * @return End iterator.
     */
    CellIterator cellEnd() const { return CellIterator(); }

    /**
     * @brief Returns a view over all cells in the grid.
     *
     * The view can be used in range-based for loops:
     * @code
     * for (const auto& cellPos : grid.cells()) { ... }
     * @endcode
     *
     * @return View over all N-D cell coordinates.
     */
    CellView cells() const { return CellView(cellBegin(), cellEnd()); }

    /**
     * @brief Returns a view over a rectangular sub-range of cells.
     *
     * @param[in] first: N-D coordinates of the first cell in the sub-range
     * (inclusive).
     * @param[in] last: N-D coordinates of the last cell in the sub-range
     * (inclusive).
     * @return View over the specified sub-range of N-D cell coordinates.
     */
    CellView cells(const CellPos& first, const CellPos& last) const
    {
        return CellView(cellBegin(first, last), cellEnd());
    }

protected:
    /**
     * @brief Resets the grid geometry.
     *
     * Intended for use by derived classes that need to reconfigure the grid
     * after construction.
     *
     * @param[in] box: New axis-aligned bounding box.
     * @param[in] size: New cell count along each dimension.
     */
    void set(const Box<Point<Scalar, N>>& box, const Point<uint, N>& size)
    {
        mBBox = box;
        mSize = size;
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
 * @brief Computes the optimal cell counts for a RegularGrid given the grid
 * extents and the number of elements to be stored in it.
 *
 * The heuristic targets one element per cell on average by distributing cells
 * proportionally to the side lengths of the bounding box.  Degenerate
 * dimensions (length smaller than 1e-4 of the diagonal) are assigned a single
 * cell, and the remaining cells are distributed among the non-degenerate
 * dimensions.
 *
 * Each dimension is guaranteed to have at least 1 cell.  If @p nElements is 0
 * or any length is non-positive, all dimensions are set to 1 cell.
 *
 * @tparam PointType: A type satisfying the PointConcept.  The scalar type and
 * dimensionality are deduced from it.
 *
 * @param[in] lengths: Side lengths of the bounding box (one per dimension).
 * @param[in] nElements: Number of elements that will be inserted into the grid.
 * @return Point whose i-th component is the recommended cell count along
 * dimension i.
 *
 * @relates RegularGrid
 */
template<PointConcept PointType>
Point<uint, PointType::DIM> bestGridSize(
    const PointType& lengths,
    uint             nElements)
{
    using Scalar = PointType::ScalarType;

    static const int DIM = PointType::DIM;

    const uint   mincells = 1; // Minimum number of cells along each dimension
    const Scalar GFactor  = 1; // GridEntry = NumElem*GFactor
    const Scalar diag     = lengths.norm(); // Diagonal of the bounding box
    const Scalar eps      = diag * 1e-4; // Threshold for degenerate dimensions
    const uint   ncell    = nElements * GFactor; // Target number of cells

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

#endif // VCL_SPACE_COMPLEX_GRID_REGULAR_GRID_H
