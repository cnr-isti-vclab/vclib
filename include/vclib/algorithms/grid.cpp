/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "grid.h"

namespace vcl {

/**
 * @brief Returns the best sizes (number of cells per dimension) of a Grid, starting from the
 * lengths of the grid and the number of elements to place in the grid.
 *
 * @param lengths
 * @param nElements
 * @return
 */
template<PointConcept PointType>
Point<uint, PointType::DIM> bestGridSize(const PointType& lengths, uint nElements)
{
	using Scalar = typename PointType::ScalarType;

	static const int DIM = PointType::DIM;
	Point<uint, PointType::DIM> sizes;

	const uint   mincells = 1;                   // Numero minimo di celle
	const Scalar GFactor  = 1;                   // GridEntry = NumElem*GFactor
	const Scalar diag     = lengths.norm();      // Diagonale del box
	const Scalar eps      = diag * 1e-4;         // Fattore di tolleranza
	const uint   ncell    = nElements * GFactor; // Calcolo numero di voxel

	bool sanityCheckLengths = true;
	uint lessEpsDimNumber = 0;
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

			Scalar k = std::pow((ncell/product), (1.0/DIM));

			for (uint i = 0; i < DIM; i++)
				sizes(i) = int(lengths(i) * k);
		}
		// at least one lenght is less than epsilon
		else {
			for (uint i = 0; i < DIM; i++) {
				if (isLessEps[i]) {
					sizes(i) = 1;
				}
				else {
					Scalar product = 1;
					for (uint j = 0; j < DIM; j++)
						if (j != i && !isLessEps[j])
							product *= lengths(j);
					sizes(i) =
						std::pow((ncell * lengths(i) / product), (1.0 / greaterEpsDimNumber));
				}
			}
		}

		for (uint i = 0; i < DIM; i++)
			sizes(i) = std::max(sizes(i),1);
	}

	return sizes;
}

} // namespace vcl
